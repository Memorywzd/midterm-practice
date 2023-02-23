#include "sniffer.h"
#include "logger.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <syslog.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <signal.h>

using namespace std;

int pkt_num = -1;
char* filter_exp = "ip and tcp and port 80";
char* dev = nullptr;
char* filename = nullptr;

pcap_t* handle;                     /* packet capture handle */

struct bpf_program fp;              /* compiled filter program (expression) */

pcap_dumper_t* out_pcap;

int writefd[NUM_CHILDREN];

void set_num(int num) {
	pkt_num = num;
}
void set_exp(char* exp) {
	filter_exp = exp;
}
void set_device(char* device) {
	dev = device;
}
void set_filename(char* file) {
	filename = file;
}

void write_pipe(u_char* args, const struct pcap_pkthdr* header, const u_char* packet) {
	if (filename && out_pcap) {
		pcap_dump((u_char*)out_pcap, header, packet);
        pcap_dump_flush(out_pcap);
	}
    int len = header->caplen;
    static int count = 1;				/* packet counter */
    write(writefd[count % NUM_CHILDREN], &count, 4);
    write(writefd[count % NUM_CHILDREN], args, 1);
    write(writefd[count % NUM_CHILDREN], &len, 4);
	write(writefd[count % NUM_CHILDREN], packet, len);
	count++;
}

void do_capture() {
    string devtype;
    char errbuf[PCAP_ERRBUF_SIZE];      /* error buffer */
    bpf_u_int32 mask;                   /* subnet mask */
    bpf_u_int32 net;                    /* ip */
    string* result = new string("");
    u_char wired_flag = '0';

    /* check for capture device name on command-line */
    if (dev == nullptr) {
        /* find a capture device if not specified on command-line */
        dev = pcap_lookupdev(errbuf);
        //dev = pcap_findalldevs();
        if (dev == nullptr) {
            ostringstream oss;
            oss << "Couldn't find default device: " << errbuf << endl;
            logger(LOG_ERR, oss.str().c_str());
            exit(EXIT_FAILURE);
        }
    }

    /* get network number and mask associated with capture device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        ostringstream oss;
        oss <<"Couldn't get netmask for device "<< errbuf << endl;
        logger(LOG_ERR, oss.str().c_str());
        net = 0;
        mask = 0;
    }

    /* open capture device */
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        ostringstream oss;
		oss << "Couldn't open device " << dev << ": " << errbuf << endl;
        logger(LOG_ERR, oss.str().c_str());
        exit(EXIT_FAILURE);
    }

    /* make sure we're capturing on an Ethernet device or Wi-Fi device [2] */
    if (pcap_datalink(handle) != DLT_EN10MB &&
		pcap_datalink(handle) != DLT_IEEE802_11 &&
		pcap_datalink(handle) != DLT_IEEE802_11_RADIO &&
		pcap_datalink(handle) != DLT_PRISM_HEADER &&
		pcap_datalink(handle) != DLT_IEEE802_11_RADIO_AVS) {
        ostringstream oss;
        logger(LOG_ERR, oss.str().c_str());
        exit(EXIT_FAILURE);
    }

	/* check whether the connection is wired or wireless  */
	if (pcap_datalink(handle) == DLT_EN10MB) {
        wired_flag = '1';
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
    result->append("Number of packets: " + to_string(pkt_num) + '\n');
    result->append("Filter expression:  " + string(filter_exp) + '\n');
	logger(LOG_INFO, result->c_str());
	delete result;

    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		ostringstream oss;
		oss << "Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << endl;
        logger(LOG_ERR, oss.str().c_str());
		exit(EXIT_FAILURE);
	}

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1) {
		ostringstream oss;
		oss << "Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << endl;
        logger(LOG_ERR, oss.str().c_str());
        exit(EXIT_FAILURE);
    }

    /* open file to save packet */
    if (filename)out_pcap = pcap_dump_open(handle, filename);
	
    /* now we can set our callback function */
    pcap_loop(handle, pkt_num, write_pipe, &wired_flag);
}

void ctrl_c(int sig) {
    cout << "\nKeyboard interrupt detected, stop capturing..." << endl;
    pcap_breakloop(handle);
    /* cleanup */
	if (filename && out_pcap) pcap_dump_close(out_pcap);
    pcap_freecode(&fp);
    pcap_close(handle);
    for (int i = 0; i < NUM_CHILDREN; i++)close(writefd[i]);
    exit(0);
}

void dispatch() {
    int pipefd[2];
    pid_t cpid;

    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        writefd[i] = pipefd[1];

        cpid = fork();
        if (cpid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (cpid == 0) {				/* Child reads from pipe */
            close(pipefd[1]);			/* Close unused write end */
            prctl(PR_SET_PDEATHSIG, SIGKILL);
            
            int* caplen = new int;
            int* count = new int;
			u_char* wired_flag = new u_char;
            u_char* buf = new u_char[2346];

            while (read(pipefd[0], count, 4)> 0) {
                read(pipefd[0], wired_flag, 1);
                if (*wired_flag == '1') set_wired(true);
                read(pipefd[0], caplen, 4);
                read(pipefd[0], buf, *caplen);
                got_packet(*count, buf);
            }
            close(pipefd[0]);
            delete wired_flag;
			delete count;
			delete caplen;
            delete[] buf;
            exit(EXIT_SUCCESS);
        }
        else {							/* Parent writes argv[1] to pipe */
            close(pipefd[0]);			/* Close unused read end */
        }
    }
    signal(SIGINT, ctrl_c);
    do_capture();
    for (int i = 0; i < NUM_CHILDREN; i++)close(writefd[i]);
}