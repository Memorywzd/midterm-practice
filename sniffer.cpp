#include "sniffer.h"
#include "logger.h"

#include <iostream>
#include <cstring>
#include <syslog.h>

using namespace std;

int pkt_num = -1;
char* filter_exp = "tcp and port 80";
char* dev = nullptr;

pcap_t* handle;                     /* packet capture handle */

struct bpf_program fp;              /* compiled filter program (expression) */

void set_num(int num) {
	pkt_num = num;
}
void set_exp(char* exp) {
	filter_exp = exp;
}
void set_device(char* device) {
	dev = device;
}

void do_capture() {
    string devtype;
    char errbuf[PCAP_ERRBUF_SIZE];      /* error buffer */
    bpf_u_int32 mask;                   /* subnet mask */
    bpf_u_int32 net;                    /* ip */
    int num_packets = pkt_num;          /* number of packets to capture */
    string* result = new string("");

    /* check for capture device name on command-line */
    if (dev == nullptr) {
        /* find a capture device if not specified on command-line */
        dev = pcap_lookupdev(errbuf);
        //dev = pcap_findalldevs();
        if (dev == nullptr) {
			logger(LOG_ERR, "Couldn't find default device: " + *errbuf + '\n');
            exit(EXIT_FAILURE);
        }
    }

    /* get network number and mask associated with capture device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		logger(LOG_ERR, "Couldn't get netmask for device " + *dev + ':' + *errbuf + '\n');
        net = 0;
        mask = 0;
    }

    /* open capture device */
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
		logger(LOG_ERR, "Couldn't open device " + *dev + ':' + *errbuf + '\n');
        exit(EXIT_FAILURE);
    }

    /* make sure we're capturing on an Ethernet device or Wi-Fi device [2] */
    if (pcap_datalink(handle) != DLT_EN10MB &&
		pcap_datalink(handle) != DLT_IEEE802_11 &&
		pcap_datalink(handle) != DLT_IEEE802_11_RADIO &&
		pcap_datalink(handle) != DLT_PRISM_HEADER &&
		pcap_datalink(handle) != DLT_IEEE802_11_RADIO_AVS) {
		logger(LOG_ERR, "%s is neither Ethernet nor Wi-Fi" + *dev + '\n');
        exit(EXIT_FAILURE);
    }
	
	/* check whether the connection is wired or wireless  */
	if (pcap_datalink(handle) == DLT_EN10MB) {
        set_wired(true);
		devtype = "ethernet";
	}
	else devtype = "wireless";

    /* print capture info */
	result->append("Device: " + string(dev) + '\n');
	result->append("Device type: " + devtype + '\n');
    char str[16];
	u_int32_t anet = ntohl(net);
	u_int32_t amask = ntohl(mask);
    memset(str, 0, sizeof(str));
    sprintf(str, "%d.%d.%d.%d", (anet >> 24) & 0xff, (anet >> 16) & 0xff, (anet >> 8) & 0xff, anet & 0xff);
    result->append("Device net&mask: " + string(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%d.%d.%d.%d", (amask >> 24) & 0xff, (amask >> 16) & 0xff, (amask >> 8) & 0xff, amask & 0xff);
	result->append(" " + string(str) + '\n');
    result->append("Number of packets: " + to_string(num_packets) + '\n');
    result->append("Filter expression:  " + string(filter_exp) + '\n');
	logger(LOG_INFO, result->c_str());
	delete result;

    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		logger(LOG_ERR, "Couldn't parse filter " + *filter_exp + ':' + *pcap_geterr(handle) + '\n');
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1) {
		logger(LOG_ERR, "Couldn't install filter " + *filter_exp + ':' + *pcap_geterr(handle) + '\n');
        exit(EXIT_FAILURE);
    }

    /* now we can set our callback function */
    pcap_loop(handle, num_packets, got_packet, nullptr);
}

void ctrl_c(int sig) {
    cout << "\nKeyboard interrupt detected, stop capturing..." << endl;
    pcap_breakloop(handle);
    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);
    exit(0);
}