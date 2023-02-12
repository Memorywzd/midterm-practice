#include "process.h"

void list_interfaces() {
	pcap_if_t* alldevs;
	char errbuf[PCAP_ERRBUF_SIZE];
	if (pcap_findalldevs(&alldevs, errbuf) < 0)
	{
		fprintf(stderr, "%s", errbuf);
		exit(1);
	}
	while (alldevs != NULL)
	{
		printf("%s\n", alldevs->name);
		alldevs = alldevs->next;
	}
	exit(0);
}
	

void help(char* name) {
	{
		cout << "By default, the program will capture HTTP packets from the first available interface." << endl;
		cout << "Usage: ./" << name;
		cout<< "[-n <number> ][-f <expression>][-i <interface>][-l][-h]" << endl;
		cout << "Options:" << endl;
		cout << "  -n <number>     Number of packets to capture." << endl;
		cout << "  -f <expression> Capture only packets that satisfy the filter <expression>." << endl;
		cout << "  -i <interface>  Listen on <interface> for packets." << endl;
		cout << "  -l              List all network interfaces." << endl;
		cout << "  -h              Print this help message." << endl;
	}
	exit(0);
}