#pragma once

#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <pcap.h>

#include "daemonize.h"

using namespace std;

void list_interfaces();

void help(char*);