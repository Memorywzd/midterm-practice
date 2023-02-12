#pragma once

#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <pcap.h>

using namespace std;

void ctrl_c();

void list_interfaces();

void help(char*);