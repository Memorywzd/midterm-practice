#include "sniffer.h"
#include "process.h"


int main(int argc, char *argv[]) {
	signal(SIGINT, ctrl_c);
	
	char opt;
	bool isDaemon = false;
	while ((opt = getopt(argc, argv, "dlhn:f:i:")) != -1) {
		switch (opt) {
		case 'd':
			isDaemon = true;
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
	
	if (isDaemon) daemonize(argv[0]);
	do_capture();
	
	return 0;
}