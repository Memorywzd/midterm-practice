#pragma once

#include <iostream>
#include <sys/types.h>

//TODO
// 连接数据库、将数据包存入文件
//Save HTTP Payload

void save_http_payload(const u_char* payload, int len);
