#include "sniffer.h"
#include "process.h"


int main(int argc, char *argv[]) {
	signal(SIGINT, ctrl_c);
	
	char opt;
	while ((opt = getopt(argc, argv, "dlhn:f:i:")) != -1) {
		switch (opt) {
		case 'd':
			daemonize(argv[0]);
			break;
		case 'l':
			list_interfaces();
			break;
		case 'h':
			help(argv[0]);
			break;
		case 'n':
			set_num(atoi(optarg));
			break;
		case 'f':
			set_exp(optarg);
			break;
		case 'i':
			set_device(optarg);
			break;
		default:
			help(argv[0]);
			break;
		}
	}
	
	do_capture();
	
	return 0;
}