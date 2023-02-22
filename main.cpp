#include "sniffer.h"
#include "process.h"

int main(int argc, char *argv[]) {
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
	
	if (isDaemon) {
		daemonize(argv[0]);
		if (already_running()) {
			logger(3, "Another instance is running, exit.");
			exit(1);
		}
	}
	dispatch();
	
	return 0;
}