#pragma once

#include <iostream>
#include <sys/types.h>

// TODO
// 1. HTTP Payload Parser
// 2. �������Ż������Ǻ�������ֵ���ж������ɹ�/ʧ�ܣ��쳣����

//HTTP Header
struct sniff_http
{

};

void parse_http_payload(const u_char* payload, int len);