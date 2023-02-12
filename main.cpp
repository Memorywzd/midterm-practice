#include "sniffer.h"
#include "process.h"


int main(int argc, char *argv[]) {
	char opt;
	while ((opt = getopt(argc, argv, "lhn:f:i:")) != -1) {
		switch (opt) {
		case 'l':
			list_interfaces();
			break;
		case 'h':
			help(argv[0]);
			break;
		case 'n':
			set_param(atoi(optarg));
			break;
		case 'f':
			set_param(-1,optarg);
			break;
		case 'i':
			set_param(-1, "tcp and port 80", optarg);
			break;
		default:
			help(argv[0]);
			break;
		}
	}
	do_capture();
	return 0;
}