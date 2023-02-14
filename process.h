#pragma once

#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <pcap.h>

#include "daemonize.h"

void list_interfaces();

void help(char*);