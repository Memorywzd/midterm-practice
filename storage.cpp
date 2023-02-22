#include "storage.h"
#include "logger.h"
#include "parse.h"

#include <iostream>
#include <sstream>
#include <syslog.h>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#define LOCKFILE "/var/run/snifferstorage.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

using namespace std;

bool wired = false;
void set_wired(bool w) {
    wired = w;
}

void print_hex_ascii_line(const u_char* payload, int len, int offset) {
    int i;
    int gap;
    const u_char* ch;

    ostringstream output;

    /* offset */
    output << setw(5) << setfill('0') << offset << "   ";

    /* hex */
    ch = payload;
    for (i = 0; i < len; i++) {
        output << hex << setw(2) << setfill('0') << static_cast<int>(*ch) << " ";
        ch++;
        /* add extra space after 8th byte for visual aid */
        if (i == 7)
            output << " ";
    }
    /* add space to handle line less than 8 bytes */
    if (len < 8)
        output << " ";

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            output << "   ";
        }
    }
    output << "   ";

    /* ascii (if printable) */
    ch = payload;
    for (i = 0; i < len; i++) {
        if (isprint(*ch))
            output << *ch;
        else
            output << ".";
        ch++;
    }

    output << endl;

    logger(LOG_INFO, output.str().c_str());
}

void print_payload(const u_char* payload, int len) {
    int len_rem = len;
    int line_width = 16;                /* number of bytes per line */
    int line_len;
    int offset = 0;                     /* zero-based offset counter */
    const u_char* ch = payload;

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

//unused params : u_char* args, const struct pcap_pkthdr* header
void got_packet(int count, const u_char* packet) {
    /* got lock */
    int fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
	if (fd < 0) {
		logger(LOG_ERR, "Unable to open lock file");
		exit(EXIT_FAILURE);
	}

    struct flock f1;
    f1.l_type = F_WRLCK;
    f1.l_start = 0;
    f1.l_whence = SEEK_SET;
    f1.l_len = 0;
	if (fcntl(fd, F_SETLKW, &f1) < 0) {
		syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
		exit(EXIT_FAILURE);
	}
    ftruncate(fd, 0);
    char buf[16];
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);

    /* declare pointers to packet headers */
    const struct sniff_ethernet* ethernet;      /* The ethernet header [1] */
    const struct sniff_ip* ip;                  /* The IP header */
    const struct sniff_tcp* tcp;                /* The TCP header */
    u_char* payload;                            /* Packet payload */

    int size_ip;
    int size_tcp;
    int size_payload;

    string* result = new string("");
    result->append("Packet number " + to_string(count) + ":\n");

    /* 检查网络类型，获取ip载荷 */
    if (wired) {
        /* define ethernet header */
        ethernet = (struct sniff_ethernet*)(packet);

        /* print ehternet header */
        result->append("Ethernet header: \n");
        char* buf = new char[100];
        sprintf(buf, "  |-Source Address: %02x:%02x:%02x:%02x:%02x:%02x \n", ethernet->ether_shost[0], ethernet->ether_shost[1], ethernet->ether_shost[2], ethernet->ether_shost[3], ethernet->ether_shost[4], ethernet->ether_shost[5]);
        result->append(string(buf));
        sprintf(buf, "  |-Destination Address: %02x:%02x:%02x:%02x:%02x:%02x \n", ethernet->ether_dhost[0], ethernet->ether_dhost[1], ethernet->ether_dhost[2], ethernet->ether_dhost[3], ethernet->ether_dhost[4], ethernet->ether_dhost[5]);
        result->append(string(buf));
        delete[] buf;
        result->append("  |-Ethernet Protocol: " + to_string(ethernet->ether_type) + "\n");

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
        result->append("   * Invalid IP header length: " + to_string(size_ip) + " bytes\n");
        logger(LOG_INFO, result->c_str());
        return;
    }

    /* print source and destination IP addresses */
    result->append("       From: " + string(inet_ntoa(ip->ip_src)) + '\n');
    result->append("         To: " + string(inet_ntoa(ip->ip_dst)) + '\n');

    /* determine protocol */
    switch (ip->ip_p) {
    case IPPROTO_TCP:
        result->append("   Protocol: TCP\n");
        break;
    case IPPROTO_UDP:
        result->append("   Protocol: UDP\n");
        logger(LOG_INFO, result->c_str());
        return;
    case IPPROTO_ICMP:
        result->append("   Protocol: ICMP\n");
        logger(LOG_INFO, result->c_str());
        return;
    case IPPROTO_IP:
        result->append("   Protocol: IP\n");
        logger(LOG_INFO, result->c_str());
        return;
    default:
        result->append("   Protocol: unknown\n");
        logger(LOG_INFO, result->c_str());
        return;
    }

    /*
     *  OK, this packet is TCP.
     */

     /* define/compute tcp header offset */
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp) * 4;
    if (size_tcp < 20) {
        result->append("   * Invalid TCP header length: " + to_string(size_tcp) + " bytes\n");
        return;
    }

    result->append("   Src port: " + to_string(ntohs(tcp->th_sport)) + "\n");
    result->append("   Dst port: " + to_string(ntohs(tcp->th_dport)) + "\n");

    /* define/compute tcp payload (segment) offset */
    payload = (u_char*)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

    /*
     * Print payload data; it might be binary, so don't just
     * treat it as a string.
     */
    if (size_payload > 0) {
        result->append("   Payload (" + to_string(size_payload) + " bytes):\n");
        logger(LOG_INFO, result->c_str());
        print_payload(payload, size_payload);
        parse_http_payload(payload, size_payload,count);
    }
    else {
        result->append("No payload\n");
        logger(LOG_INFO, result->c_str());
    }
	delete result;

	f1.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &f1) < 0) {
		syslog(LOG_ERR, "can't unlock %s: %s", LOCKFILE, strerror(errno));
		exit(EXIT_FAILURE);
	}
    close(fd);
}