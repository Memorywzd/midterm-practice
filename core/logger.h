#pragma once

#include <syslog.h>

void setDaemon(bool);

void logger(int priority, const char* format);