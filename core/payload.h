#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <mysql/mysql.h>
#include <mysql/errmsg.h>

using namespace std;

//enum class http_mthd
//{
//    HTTP_MT_OPTIONS = 0, /* RFC2616 */
//    HTTP_MT_GET,
//    HTTP_MT_HEAD,
//    HTTP_MT_POST,
//    HTTP_MT_PUT,
//    HTTP_MT_DELETE,
//    HTTP_MT_TRACE,
//    HTTP_MT_CONNECT,
//    HTTP_MT_PATCH,
//    HTTP_MT_LINK,
//    HTTP_MT_UNLINK,
//    HTTP_MT_PROPFIND,    /* RFC2518 */
//    HTTP_MT_MKCOL,
//    HTTP_MT_COPY,
//    HTTP_MT_MOVE,
//    HTTP_MT_LOCK,
//    HTTP_MT_UNLOCK,
//    HTTP_MT_POLL,        /* Outlook Web Access */
//    HTTP_MT_BCOPY,
//    HTTP_MT_BMOVE,
//    HTTP_MT_SEARCH,
//    HTTP_MT_BDELETE,
//    HTTP_MT_PROPPATCH,
//    HTTP_MT_BPROPFIND,
//    HTTP_MT_BPROPPATCH,
//    HTTP_MT_LABEL,             /* RFC 3253 8.2 */
//    HTTP_MT_MERGE,             /* RFC 3253 11.2 */
//    HTTP_MT_REPORT,            /* RFC 3253 3.6 */
//    HTTP_MT_UPDATE,            /* RFC 3253 7.1 */
//    HTTP_MT_CHECKIN,           /* RFC 3253 4.4, 9.4 */
//    HTTP_MT_CHECKOUT,          /* RFC 3253 4.3, 9.3 */
//    HTTP_MT_UNCHECKOUT,        /* RFC 3253 4.5 */
//    HTTP_MT_MKACTIVITY,        /* RFC 3253 13.5 */
//    HTTP_MT_MKWORKSPACE,       /* RFC 3253 6.3 */
//    HTTP_MT_VERSION_CONTROL,   /* RFC 3253 3.5 */
//    HTTP_MT_BASELINE_CONTROL,  /* RFC 3253 12.6 */
//    HTTP_MT_NOTIFY,            /* uPnP forum */
//    HTTP_MT_SUBSCRIBE,
//    HTTP_MT_UNSUBSCRIBE,
//    HTTP_MT_ICY,               /* Shoutcast client (forse) */
//    HTTP_MT_NONE
//};

struct http_header {
    string name;
    string value;
};

struct http_request {
    string request_line;
    string uri;
    vector<http_header> headers;

    int content_length;
    int real_length;
    string content_type;
    string host;
    string user_agent;
    string cookie;
    string connection;

    char* body;
};

struct http_response {
    string status_line;
    vector<http_header> headers;

    int content_length;
    int real_length;
    string content_type;
    string content_encoding;
    string server;
    string set_cookie;
    string cache_control;
    string if_modified_since;

    char* body;
};

void get_payload(u_char* origin_payload, int len, int count, MYSQL* mysql);

