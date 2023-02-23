#pragma once

#include "storage.h"

#define NUM_CHILDREN 5

void set_num(int num);
void set_exp(char* exp);
void set_device(char* device);
void set_filename(char* file);

void dispatch();

void ctrl_c(int);
