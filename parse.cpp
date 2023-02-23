#include "parse.h"
using namespace std;

extern http_st_code HTTP_STATUS_CODE_ARRAY[] = {
    {100, HTTP_ST_100},
    {101, HTTP_ST_101},
    {102, HTTP_ST_102},
    {199, HTTP_ST_199},
    {200, HTTP_ST_200},
    {201, HTTP_ST_201},
    {202, HTTP_ST_202},
    {203, HTTP_ST_203},
    {204, HTTP_ST_204},
    {205, HTTP_ST_205},
    {206, HTTP_ST_206},
    {207, HTTP_ST_207},
    {299, HTTP_ST_299},
    {300, HTTP_ST_300},
    {301, HTTP_ST_301},
    {302, HTTP_ST_302},
    {303, HTTP_ST_303},
    {304, HTTP_ST_304},
    {305, HTTP_ST_305},
    {307, HTTP_ST_307},
    {399, HTTP_ST_399},
    {400, HTTP_ST_400},
    {401, HTTP_ST_401},
    {402, HTTP_ST_402},
    {403, HTTP_ST_403},
    {404, HTTP_ST_404},
    {405, HTTP_ST_405},
    {406, HTTP_ST_406},
    {407, HTTP_ST_407},
    {408, HTTP_ST_408},
    {409, HTTP_ST_409},
    {410, HTTP_ST_410},
    {411, HTTP_ST_411},
    {412, HTTP_ST_412},
    {413, HTTP_ST_413},
    {414, HTTP_ST_414},
    {415, HTTP_ST_415},
    {416, HTTP_ST_416},
    {417, HTTP_ST_417},
    {422, HTTP_ST_422},
    {423, HTTP_ST_423},
    {424, HTTP_ST_424},
    {499, HTTP_ST_499},
    {500, HTTP_ST_500},
    {501, HTTP_ST_501},
    {502, HTTP_ST_502},
    {503, HTTP_ST_503},
    {504, HTTP_ST_504},
    {505, HTTP_ST_505},
    {507, HTTP_ST_507},
    {599, HTTP_ST_599}
};

char* ret_mthd(const char* line, int linelen) {
    http_mthd mthd = http_request_method(line, linelen);
    char* method = strrchr((char*)mthd, '_');
    method++;
    return method;
}

char* ret_mthd2(const char* data, int linelen) {
    const char* ptr;
    int	index = 0;
    int prefix_len = 0;
    char* unkn;

    if (linelen >= 2) {
        if (strncmp(data, "M-", 2) == 0 || strncmp(data, "\r\n", 2) == 0) { /* \r\n necesary for bug in client POST */
            data += 2;
            linelen -= 2;
            prefix_len = 2;
        }
    }

    ptr = (const char*)data;
    /* Look for the space following the Method */
    while (index < linelen) {
        if (*ptr == ' ')
            break;
        else {
            ptr++;
            index++;
        }
    }

    /* Check the methods that have same length */
    switch (index) {
    case 3:
        if (strncmp(data, "GET", index) == 0) {
            return "GET";
        }
        else if (strncmp(data, "PUT", index) == 0) {
            return "PUT";
        }
#if 0
        else if (strncmp(data, "ICY", index) == 0) {
            return "ICY";
        }
#endif
        break;

    case 4:
        if (strncmp(data, "COPY", index) == 0) {
            return "COPY";
        }
        else if (strncmp(data, "HEAD", index) == 0) {
            return "HEAD" ;
        }
        else if (strncmp(data, "LOCK", index) == 0) {
            return "LOCK";
        }
        else if (strncmp(data, "MOVE", index) == 0) {
            return "MOVE";
        }
        else if (strncmp(data, "POLL", index) == 0) {
            return "POLL";
        }
        else if (strncmp(data, "POST", index) == 0) {
            return "POST";
        }
        break;

    case 5:
        if (strncmp(data, "BCOPY", index) == 0) {
            return "BCOPY";
        }
        else if (strncmp(data, "BMOVE", index) == 0) {
            return "BMOVE";
        }
        else if (strncmp(data, "MKCOL", index) == 0) {
            return "MKCOL";
        }
        else if (strncmp(data, "TRACE", index) == 0) {
            return "TRACE";
        }
        else if (strncmp(data, "LABEL", index) == 0) {  /* RFC 3253 8.2 */
            return "LABEL";
        }
        else if (strncmp(data, "MERGE", index) == 0) {  /* RFC 3253 11.2 */
            return "MERGE";
        }
        break;

    case 6:
        if (strncmp(data, "DELETE", index) == 0) {
            return "DELETE";
        }
        else if (strncmp(data, "SEARCH", index) == 0) {
            return "SEARCH";
        }
        else if (strncmp(data, "UNLOCK", index) == 0) {
            return "UNLOCK";
        }
        else if (strncmp(data, "REPORT", index) == 0) {  /* RFC 3253 3.6 */
            return "REPORT";
        }
        else if (strncmp(data, "UPDATE", index) == 0) {  /* RFC 3253 7.1 */
            return "UPDATE";
        }
        else if (strncmp(data, "NOTIFY", index) == 0) {
            return "NOTIFY";
        }
        break;

    case 7:
        if (strncmp(data, "BDELETE", index) == 0) {
            return "BDELETE";
        }
        else if (strncmp(data, "CONNECT", index) == 0) {
            return "CONNECT";
        }
        else if (strncmp(data, "OPTIONS", index) == 0) {
            return "OPTIONS";
        }
        else if (strncmp(data, "CHECKIN", index) == 0) {  /* RFC 3253 4.4, 9.4 */
            return "CHECKIN";
        }
        break;

    case 8:
        if (strncmp(data, "PROPFIND", index) == 0) {
            return "PROPFIND";
        }
        else if (strncmp(data, "CHECKOUT", index) == 0) { /* RFC 3253 4.3, 9.3 */
            return "CHECKOUT";
        }
        /*
        else if (strncmp(data, "CCM_POST", index) == 0) {
            return HTTP_MT_CCM_POST;
        }
        */
        break;

    case 9:
        if (strncmp(data, "SUBSCRIBE", index) == 0) {
            return "SUBSCRIBE";
        }
        else if (strncmp(data, "PROPPATCH", index) == 0) {
            return "PROPPATCH";
        }
        else  if (strncmp(data, "BPROPFIND", index) == 0) {
            return "BPROPFIND";
        }
        break;

    case 10:
        if (strncmp(data, "BPROPPATCH", index) == 0) {
            return "BPROPPATCH";
        }
        else if (strncmp(data, "UNCHECKOUT", index) == 0) {  /* RFC 3253 4.5 */
            return "UNCHECKOUT";
        }
        else if (strncmp(data, "MKACTIVITY", index) == 0) {  /* RFC 3253 13.5 */
            return "MKACTIVITY";
        }
        break;

    case 11:
        if (strncmp(data, "MKWORKSPACE", index) == 0) {  /* RFC 3253 6.3 */
            return "MKWORKSPACE";
        }
        else if (strncmp(data, "UNSUBSCRIBE", index) == 0) {
            return "UNSUBSCRIBE";
        }
        /*
        else if (strncmp(data, "RPC_CONNECT", index) == 0) {
            return HTTP_MT_RPC_CONNECT;
        }
        */
        break;

    case 15:
        if (strncmp(data, "VERSION-CONTROL", index) == 0) {  /* RFC 3253 3.5 */
            return  "VERSION-CONTROL";
        }
        break;

    case 16:
        if (strncmp(data, "BASELINE-CONTROL", index) == 0) {  /* RFC 3253 12.6 */
            return "BASELINE-CONTROL";
        }
        break;

    default:
        break;
        }
}

int ret_status(const char* line, int len) {
    http_status sta = http_response_status(line, len);
    int i, dim = sizeof(HTTP_STATUS_CODE_ARRAY) / sizeof(http_st_code);
    int status = 0;
    for (i = 0; i < dim; i++) {
        if (HTTP_STATUS_CODE_ARRAY[i].st == sta) {
            status = HTTP_STATUS_CODE_ARRAY[i].num;
            break;
        }
    }
    return status;
}

char* ret_first_line(char* payload) {
    char* ptr = payload;
    if (strcspn(ptr, "\r") + 1 == strcspn(ptr, "\n")) {
        // printf("have\n");
        int pos = strcspn(ptr, "\r");
        char* res = (char*)malloc(sizeof(char) * (pos + 1));
        snprintf(res, pos + 1, "%s", ptr);
        return res;
    }
    return NULL;
}

char* ret_info(char* keyword, char* payload) {
    char* ptr = strstr(payload, keyword);
    if (ptr == NULL) {
        return NULL;
    }
    ptr = ptr + strlen(keyword);
    if (strcspn(ptr, "\r") + 1 == strcspn(ptr, "\n")) {
        int pos = strcspn(ptr, "\r");
        char* res = (char*)malloc(sizeof(char) * (pos + 1));
        snprintf(res, pos + 1, "%s", ptr);
        return res;
    }
    return NULL;
}

char* ret_host_uri(char* payload, int linelen,char* host) {
    char* mthd = ret_mthd2(payload, linelen);
    char* uri_ver = ret_info(mthd, payload);
    char* uri = strtok(uri_ver, " ");
    if (uri != NULL) {
        strcat(host, uri);
        return host;
    }
    else {
        return NULL;
    }
}


void parse_http_payload(u_char* origin_payload, int len, int count) {
    char* payload = (char*)origin_payload;
    printf("http payload:\n%s", payload);
    char* ptr = payload;

    sniff_http_request* req = new sniff_http_request;
    sniff_http_response* res = new sniff_http_response;
    if (!(pkt_type(ptr, len))) {
        //printf("REQUEST\n\n");
        //printf("------------------TEST-PRINT\n")
        req->number = count;
        req->request_line = ret_first_line(payload);
        //if (req->request_line != NULL) {printf("fir:%s\n", req->request_line);}

        req->host = ret_info("Host:", payload);
        //cout << "Host:" << req->host << endl;
        
        if (req->host != NULL) {
            req->request_uri = ret_host_uri(payload, len, req->host);
            //cout << "req->request_uri:" << req->request_uri << endl;
        }
        req->user_agent = ret_info("User-Agent:", payload);
        //cout << "req->user_agent:" << req->user_agent << endl;
        req->cookie = ret_info("Cookie:", payload);
        // cout << "req->cookie:" << req->cookie << endl;

        char* len = ret_info("Content-Length:", payload);
        int length=0;
        if (len != NULL) {
            length = atoi(len);
        }
        req->content_length = length;


        req->content_type = ret_info("Content-Type:", payload);
        // cout << " req->content_type:" << req->content_type << endl;

        req->connection = ret_info("Connection:", payload);
        //cout << "req->connection:" << req->connection << endl;

        char query[8192];
        sprintf(query, "INSERT INTO sniff_http_request values(%d,'%s','%s','%s','%s','%s',%d,'%s','%s')",
            count, req->request_line, req->host, req->request_uri, req->user_agent, req->cookie,
            req->content_length, req->content_type, req->connection);
        //mysql_query(&mysql, query);
        int n = mysql_real_query(&mysql, query, strlen(query));
        if (n) {
            cout << "Failed to insert the request:" << mysql_error(&mysql) << endl;
        }
        delete req;
    }
    else {
        //printf("RESPOND\n\n");
        //printf("------------------TEST-PRINT\n");

        res->number = count;
        res->status_line = ret_first_line(payload);
        //printf("fir:%s\n", res->status_line);

        res->server = ret_info("Server:", payload);
        //printf("server:%s\n", res->server);

        char* len = ret_info("Content-Length:", payload);
        int length=0;
        if (len != NULL) {
            length = atoi(len);
        }
        res->content_length = length;

        res->content_type = ret_info("Content-Type:", payload);
        //cout << "res->content_type:" << res->content_type << endl;
        res->content_encoding = ret_info("Content-Encoding:", payload);
        // cout << "res->content_encoding:" << res->content_encoding << endl;
        res->set_cookie = ret_info("Set-Cookie:", payload);
        //cout << "res->set_cookie:" << res->set_cookie << endl;

        res->cache_control = ret_info("Cache-Control:", payload);
        //printf("cc:%s\n", res->cache_control);
        res->if_modified_since = ret_info("If-Modified-Since:", payload);
        //printf("ims:%s\n", res->if_modified_since);


        char query[8192];
        sprintf(query, "INSERT INTO sniff_http_response values(%d,'%s','%s',%d,'%s','%s','%s','%s','%s')", count,
            res->status_line, res->server, res->content_length, res->content_type, res->content_encoding,
            res->set_cookie, res->cache_control, res->if_modified_since);
        mysql_query(&mysql, query);
        int n = mysql_real_query(&mysql, query, strlen(query));
        if (n) {
            cout << "Failed to insert the  response:" << mysql_error(&mysql) << endl;
        }

    }
    if (res->content_type != NULL) {
        char file_path[101];
        //char* cont = find_httphdr_end(payload);
        char* cont = strstr(payload, "\r\n\r\n") + 4;
        char* type = ret_info("Content-Type:", payload);
        if (strstr(type, "text/html") != NULL) {
            sprintf(file_path, "res_content/%d.html", count);
            FILE* fp = fopen(file_path, "w");
            if (cont != NULL) {
                fputs(cont, fp);
            }
            fclose(fp);
        }
        else if (strstr(type, "text/plain") != NULL) {
            sprintf(file_path, "res_content/%d.txt", count);
            FILE* fp = fopen(file_path, "w");
            if (cont != NULL) {
                fputs(cont, fp);
            }
            fclose(fp);
        }
        else if (strstr(type, "image/jpeg") != NULL) {
            sprintf(file_path, "res_content/%d.jpg", count);
            FILE* fp = fopen(file_path, "wb");
            if (cont != NULL) {
                fwrite(cont, sizeof(char), res->content_length, fp);
            }
            fclose(fp);
        }
        else if (strstr(type, "image/gif") != NULL) {
            sprintf(file_path, "res_content/%d.gif", count);
            FILE* fp = fopen(file_path, "wb");
            if (cont != NULL) {
                fwrite(cont, sizeof(char), res->content_length, fp);
            }
            fclose(fp);
        }
        else if (strstr(type, "video/quicktime") != NULL) {
            sprintf(file_path, "res_content/%d.mov", count);
            FILE* fp = fopen(file_path, "wb");
            if (cont != NULL) {
                fwrite(cont, sizeof(char), res->content_length, fp);
            }
            fclose(fp);
        }
        else if (strstr(type, "Applicationvnd.ms-powerpoint") != NULL) {
            sprintf(file_path, "res_content/%d.ppt", count);
            FILE* fp = fopen(file_path, "w");
            if (cont != NULL) {
                fputs(cont, fp);
            }
            fclose(fp);
        }
        else {//将其他文件类型保存为.bin
            if (res->content_length != 0) {
                sprintf(file_path, "res_content/%d.bin", count);
                FILE* fp = fopen(file_path, "wb");
                if (cont != NULL) {
                    fwrite(cont, sizeof(char), req->content_length, fp);
                }
                fclose(fp);
            }
        }
    }
    delete req;
    delete res;
}

char* find_httphdr_end(char* data) {
    char* ptr;
    ptr = strstr(data, "\r\n\r\n");
    ptr = ptr + 4;
    printf("data?:%s\n", ptr);
}

bool pkt_type(const char* ptr, const int datalen)
{
    char* req = NULL;
    char* rsp = NULL;

    req = IsRequest(ptr, datalen);
    rsp = IsResponse(ptr, datalen);

    if (req != NULL) {
        return 0;
    }
    else if (rsp != NULL) {
        return 1;
    }
}

char* IsRequest(const char* ptr, const int datalen)
{
    http_mthd method = HTTP_MT_NONE;
    char* req = NULL;

    method = http_request_method(ptr, datalen);
    if (method == HTTP_MT_NONE) {
        return NULL;
    }
    else {
        int line_cnt = 0;
        req = "req";
        return req;
    }
}

char* IsResponse(const char* ptr, const int datalen)
{
    http_ver version = HTTP_VER_NONE;
    char* Res = NULL;

    if (datalen < 8) {
        return NULL;
    }

    version = http_response_version(ptr, datalen);
    if (version == HTTP_VER_NONE) {
        return NULL;
    }
    else {
        int line_cnt = 0;
        Res = "Res";
        return Res;
    }
}


static http_ver http_response_version(const char* line, int len)
{
    if (strncmp(line, "HTTP/1.0", 8) == 0)
        return HTTP_VER_1_0;

    if (strncmp(line, "HTTP/1.1", 8) == 0)
        return HTTP_VER_1_1;

    return HTTP_VER_NONE;
}

static http_mthd http_request_method(const char* data, int linelen)
{
    const char* ptr;
    int	index = 0;
    int prefix_len = 0;
    char* unkn;

    if (linelen >= 2) {
        if (strncmp(data, "M-", 2) == 0 || strncmp(data, "\r\n", 2) == 0) { /* \r\n necesary for bug in client POST */
            data += 2;
            linelen -= 2;
            prefix_len = 2;
        }
    }

    ptr = (const char*)data;
    /* Look for the space following the Method */
    while (index < linelen) {
        if (*ptr == ' ')
            break;
        else {
            ptr++;
            index++;
        }
    }

    /* Check the methods that have same length */
    switch (index) {
    case 3:
        if (strncmp(data, "GET", index) == 0) {
            return HTTP_MT_GET;
        }
        else if (strncmp(data, "PUT", index) == 0) {
            return HTTP_MT_PUT;
        }
#if 0
        else if (strncmp(data, "ICY", index) == 0) {
            return HTTP_MT_ICY;
        }
#endif
        break;

    case 4:
        if (strncmp(data, "COPY", index) == 0) {
            return HTTP_MT_COPY;
        }
        else if (strncmp(data, "HEAD", index) == 0) {
            return HTTP_MT_HEAD;
        }
        else if (strncmp(data, "LOCK", index) == 0) {
            return HTTP_MT_LOCK;
        }
        else if (strncmp(data, "MOVE", index) == 0) {
            return HTTP_MT_MOVE;
        }
        else if (strncmp(data, "POLL", index) == 0) {
            return HTTP_MT_POLL;
        }
        else if (strncmp(data, "POST", index) == 0) {
            return HTTP_MT_POST;
        }
        break;

    case 5:
        if (strncmp(data, "BCOPY", index) == 0) {
            return HTTP_MT_BCOPY;
        }
        else if (strncmp(data, "BMOVE", index) == 0) {
            return HTTP_MT_BMOVE;
        }
        else if (strncmp(data, "MKCOL", index) == 0) {
            return HTTP_MT_MKCOL;
        }
        else if (strncmp(data, "TRACE", index) == 0) {
            return HTTP_MT_TRACE;
        }
        else if (strncmp(data, "LABEL", index) == 0) {  /* RFC 3253 8.2 */
            return HTTP_MT_LABEL;
        }
        else if (strncmp(data, "MERGE", index) == 0) {  /* RFC 3253 11.2 */
            return HTTP_MT_MERGE;
        }
        break;

    case 6:
        if (strncmp(data, "DELETE", index) == 0) {
            return HTTP_MT_DELETE;
        }
        else if (strncmp(data, "SEARCH", index) == 0) {
            return HTTP_MT_SEARCH;
        }
        else if (strncmp(data, "UNLOCK", index) == 0) {
            return HTTP_MT_UNLOCK;
        }
        else if (strncmp(data, "REPORT", index) == 0) {  /* RFC 3253 3.6 */
            return HTTP_MT_REPORT;
        }
        else if (strncmp(data, "UPDATE", index) == 0) {  /* RFC 3253 7.1 */
            return HTTP_MT_UPDATE;
        }
        else if (strncmp(data, "NOTIFY", index) == 0) {
            return HTTP_MT_NOTIFY;
        }
        break;

    case 7:
        if (strncmp(data, "BDELETE", index) == 0) {
            return HTTP_MT_BDELETE;
        }
        else if (strncmp(data, "CONNECT", index) == 0) {
            return HTTP_MT_CONNECT;
        }
        else if (strncmp(data, "OPTIONS", index) == 0) {
            return HTTP_MT_OPTIONS;
        }
        else if (strncmp(data, "CHECKIN", index) == 0) {  /* RFC 3253 4.4, 9.4 */
            return HTTP_MT_CHECKIN;
        }
        break;

    case 8:
        if (strncmp(data, "PROPFIND", index) == 0) {
            return HTTP_MT_PROPFIND;
        }
        else if (strncmp(data, "CHECKOUT", index) == 0) { /* RFC 3253 4.3, 9.3 */
            return HTTP_MT_CHECKOUT;
        }
        /*
        else if (strncmp(data, "CCM_POST", index) == 0) {
            return HTTP_MT_CCM_POST;
        }
        */
        break;

    case 9:
        if (strncmp(data, "SUBSCRIBE", index) == 0) {
            return HTTP_MT_SUBSCRIBE;
        }
        else if (strncmp(data, "PROPPATCH", index) == 0) {
            return HTTP_MT_PROPPATCH;
        }
        else  if (strncmp(data, "BPROPFIND", index) == 0) {
            return HTTP_MT_BPROPFIND;
        }
        break;

    case 10:
        if (strncmp(data, "BPROPPATCH", index) == 0) {
            return HTTP_MT_BPROPPATCH;
        }
        else if (strncmp(data, "UNCHECKOUT", index) == 0) {  /* RFC 3253 4.5 */
            return HTTP_MT_UNCHECKOUT;
        }
        else if (strncmp(data, "MKACTIVITY", index) == 0) {  /* RFC 3253 13.5 */
            return HTTP_MT_MKACTIVITY;
        }
        break;

    case 11:
        if (strncmp(data, "MKWORKSPACE", index) == 0) {  /* RFC 3253 6.3 */
            return HTTP_MT_MKWORKSPACE;
        }
        else if (strncmp(data, "UNSUBSCRIBE", index) == 0) {
            return HTTP_MT_UNSUBSCRIBE;
        }
        /*
        else if (strncmp(data, "RPC_CONNECT", index) == 0) {
            return HTTP_MT_RPC_CONNECT;
        }
        */
        break;

    case 15:
        if (strncmp(data, "VERSION-CONTROL", index) == 0) {  /* RFC 3253 3.5 */
            return HTTP_MT_VERSION_CONTROL;
        }
        break;

    case 16:
        if (strncmp(data, "BASELINE-CONTROL", index) == 0) {  /* RFC 3253 12.6 */
            return HTTP_MT_BASELINE_CONTROL;
        }
        break;

    default:
        break;
    }

    return HTTP_MT_NONE;
}

static http_status http_response_status(const char* line, int len)
{
    const char* next_token;
    const char* lineend;
    http_status status;
    int tokenlen, val;
    int i, dim = sizeof(HTTP_STATUS_CODE_ARRAY) / sizeof(http_st_code);

    lineend = line + len;
    status = HTTP_ST_NONE;

    /* The first token is the protocol and version */
    tokenlen = get_token_len(line, lineend, &next_token);
    if (tokenlen == 0 || line[tokenlen] != ' ') {
        return status;
    }

    line = next_token;
    /* The next token is status value. */
    tokenlen = get_token_len(line, lineend, &next_token);
    if (tokenlen == 0 || (line[tokenlen] != ' ' && line[tokenlen] != '\r' && line[tokenlen] != '\n')) {
        return status;
    }

    /*
     * Parse response status value.
     */
    if (sscanf(line, "%i", &val) != 1) {
        return status;
    }

    /* search enum */
    for (i = 0; i < dim; i++) {
        if (HTTP_STATUS_CODE_ARRAY[i].num == val) {
            status = HTTP_STATUS_CODE_ARRAY[i].st;

            break;
        }
    }
    return status;
}

static int get_token_len(const char* linep, const char* lineend, const char** next_token)
{
    const char* tokenp;
    int token_len;

    tokenp = linep;

    /*
     * Search for a blank, a CR or an LF, or the end of the buffer.
     */
    while (linep < lineend && *linep != ' ' && *linep != '\r' && *linep != '\n')
        linep++;
    token_len = linep - tokenp;

    /*
     * Skip trailing blanks.
     */
    while (linep < lineend && *linep == ' ')
        linep++;

    *next_token = linep;

    return token_len;
}