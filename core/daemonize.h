#pragma once

#include "logger.h"

bool already_running();

void daemonize(const char* cmd);