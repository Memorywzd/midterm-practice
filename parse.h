#pragma once

#include <iostream>
#include <sys/types.h>

// TODO
// 1. HTTP Payload Parser
// 2. （后期优化）考虑函数返回值，判定解析成功/失败；异常捕获

//HTTP Header
struct sniff_http
{

};

void parse_http_payload(const u_char* payload, int len);