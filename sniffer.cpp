#include "sniffer.h"

int pkt_num = -1;
char* filter_exp = "tcp and port 80";
char* dev = nullptr;

bool wired = false;

pcap_t* handle;                     /* packet capture handle */

struct bpf_program fp;              /* compiled filter program (expression) */

void set_param(int num, char* exp, char* device) {
	pkt_num = num;
	filter_exp = exp;
	dev = device;
}

void print_hex_ascii_line(const u_char *payload, int len, int offset) {
    int i;
    int gap;
    const u_char *ch;

    /* offset */
    printf("%05d   ", offset);

    /* hex */
    ch = payload;
    for (i = 0; i < len; i++) {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            printf("   ");
        }
    }
    printf("   ");

    /* ascii (if printable) */
    ch = payload;
    for (i = 0; i < len; i++) {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    printf("\n");
}

void print_payload(const u_char *payload, int len) {
    int len_rem = len;
    int line_width = 16;                /* number of bytes per line */
    int line_len;
    int offset = 0;                     /* zero-based offset counter */
    const u_char *ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width) {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for (;;) {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if (len_rem <= line_width) {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    static int count = 1;                       /* packet counter */
    /* declare pointers to packet headers */
    const struct sniff_ethernet *ethernet;      /* The ethernet header [1] */
    const struct sniff_ip *ip;                  /* The IP header */
    const struct sniff_tcp *tcp;                /* The TCP header */
    const u_char *payload;                      /* Packet payload */

    int size_ip;
    int size_tcp;
    int size_payload;

    printf("Packet number %d:\n", count);
    count++;

	/* 检查网络类型，获取ip载荷 */
    if (wired) {
        /* define ethernet header */
        ethernet = (struct sniff_ethernet*)(packet);

        /* print ehternet header */
        printf("Ethernet header: \n");
        printf("  |-Source Address: %02x:%02x:%02x:%02x:%02x:%02x \n", ethernet->ether_shost[0], ethernet->ether_shost[1], ethernet->ether_shost[2], ethernet->ether_shost[3], ethernet->ether_shost[4], ethernet->ether_shost[5]);
        printf("  |-Destination Address: %02x:%02x:%02x:%02x:%02x:%02x \n", ethernet->ether_dhost[0], ethernet->ether_dhost[1], ethernet->ether_dhost[2], ethernet->ether_dhost[3], ethernet->ether_dhost[4], ethernet->ether_dhost[5]);
        printf("  |-Ethernet Protocol: %u\n", ethernet->ether_type);

        /* define/compute ip header offset */
        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    }
    else {
		/* 使用省事处理 */
		/* define/compute ip header offset */
		ip = (struct sniff_ip*)(packet + SIZE_80211_HEADER + SIZE_LLC_HEADER + SIZE_RADIOTAP_HEADER);
    }
    
    size_ip = IP_HL(ip) * 4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }

    /* print source and destination IP addresses */
    printf("       From: %s\n", inet_ntoa(ip->ip_src));
    printf("         To: %s\n", inet_ntoa(ip->ip_dst));

    /* determine protocol */
    switch (ip->ip_p) {
        case IPPROTO_TCP:
            printf("   Protocol: TCP\n");
            break;
        case IPPROTO_UDP:
            printf("   Protocol: UDP\n");
            return;
        case IPPROTO_ICMP:
            printf("   Protocol: ICMP\n");
            return;
        case IPPROTO_IP:
            printf("   Protocol: IP\n");
            return;
        default:
            printf("   Protocol: unknown\n");
            return;
    }

    /*
     *  OK, this packet is TCP.
     */

    /* define/compute tcp header offset */
    tcp = (struct sniff_tcp *) (packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp) * 4;
    if (size_tcp < 20) {
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
        return;
    }

    printf("   Src port: %d\n", ntohs(tcp->th_sport));
    printf("   Dst port: %d\n", ntohs(tcp->th_dport));

    /* define/compute tcp payload (segment) offset */
    payload = (u_char *) (packet + SIZE_ETHERNET + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

    /*
     * Print payload data; it might be binary, so don't just
     * treat it as a string.
     */
    if (size_payload > 0) {
        printf("   Payload (%d bytes):\n", size_payload);
        print_payload(payload, size_payload);
    }
	else cout << "No payload" << endl;
}

void do_capture() {
    string devtype;
    char errbuf[PCAP_ERRBUF_SIZE];      /* error buffer */
    bpf_u_int32 mask;                   /* subnet mask */
    bpf_u_int32 net;                    /* ip */
    int num_packets = pkt_num;          /* number of packets to capture */

    /* check for capture device name on command-line */
    if (dev == nullptr) {
        /* find a capture device if not specified on command-line */
        dev = pcap_lookupdev(errbuf);
        //dev = pcap_findalldevs();
        if (dev == nullptr) {
            fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
            exit(EXIT_FAILURE);
        }
    }

    /* get network number and mask associated with capture device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* open capture device */
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }

    /* make sure we're capturing on an Ethernet device or Wi-Fi device [2] */
    if (pcap_datalink(handle) != DLT_EN10MB &&
		pcap_datalink(handle) != DLT_IEEE802_11 &&
		pcap_datalink(handle) != DLT_IEEE802_11_RADIO &&
		pcap_datalink(handle) != DLT_PRISM_HEADER &&
		pcap_datalink(handle) != DLT_IEEE802_11_RADIO_AVS) {
        fprintf(stderr, "%s is neither Ethernet nor Wi-Fi\n", dev);
        exit(EXIT_FAILURE);
    }
	
	/* check whether the connection is wired or wireless  */
	if (pcap_datalink(handle) == DLT_EN10MB) {
		wired = true;
		devtype = "ethernet";
	}
	else devtype = "wireless";

    /* print capture info */
	cout << "Device: " << dev << endl;
	cout << "Device type: " << devtype << endl;
    char str[16];
	u_int32_t anet = ntohl(net);
	u_int32_t amask = ntohl(mask);
    memset(str, 0, sizeof(str));
    sprintf(str, "%d.%d.%d.%d", (anet >> 24) & 0xff, (anet >> 16) & 0xff, (anet >> 8) & 0xff, anet & 0xff);
    cout << "Device net&mask: "<< str;
	memset(str, 0, sizeof(str));
	sprintf(str, "%d.%d.%d.%d", (amask >> 24) & 0xff, (amask >> 16) & 0xff, (amask >> 8) & 0xff, amask & 0xff);
    cout << ' ' << str << endl;
	cout << "Number of packets:  " << num_packets << endl;
	cout << "Filter expression:  " << filter_exp << endl;

    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* now we can set our callback function */
    pcap_loop(handle, num_packets, got_packet, nullptr);
}

void ctrl_c() {
    cout << "keyboard interrupt detected, stop capturing..." << endl;
    pcap_breakloop(handle);
    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);
    exit(0);
}