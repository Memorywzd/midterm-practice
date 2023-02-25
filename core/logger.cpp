#include "logger.h"

#include <iostream>
#include <syslog.h>

bool isDaemon = false;
void setDaemon(bool daemon) {
	isDaemon = daemon;
}

void logger(int priority, const char* format) {
	if (isDaemon) {
		syslog(priority, format);
	}
	else {
		std::cout << format;
	}
}