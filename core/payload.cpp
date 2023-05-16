#include "logger.h"
#include "payload.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

#include <sys/stat.h>
#include <sys/types.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>

using namespace std;

void print_hex_ascii_line(const u_char* payload, int len, int offset) {
    int i;
    int gap;
    const u_char* ch;

    ostringstream output;

    /* offset */
    output << setw(5) << setfill('0') << offset << "   ";

    /* hex */
    ch = payload;
    for (i = 0; i < len; i++) {
        output << hex << setw(2) << setfill('0') << static_cast<int>(*ch) << " ";
        ch++;
        /* add extra space after 8th byte for visual aid */
        if (i == 7)
            output << " ";
    }
    /* add space to handle line less than 8 bytes */
    if (len < 8)
        output << " ";

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            output << "   ";
        }
    }
    output << "   ";

    /* ascii (if printable) */
    ch = payload;
    for (i = 0; i < len; i++) {
        if (isprint(*ch))
            output << *ch;
        else
            output << ".";
        ch++;
    }

    output << endl;

    logger(LOG_INFO, output.str().c_str());
}

void print_payload(const u_char* payload, int len) {
    int len_rem = len;
    int line_width = 16;                /* number of bytes per line */
    int line_len;
    int offset = 0;                     /* zero-based offset counter */
    const u_char* ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width) {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for (;;) {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if (len_rem <= line_width) {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }
}

void split_lines(const string& s, vector<string>& lines) {
    lines.clear();
    size_t start = 0, end = 0;
    while ((end = s.find("\r\n", start)) != string::npos) {
        lines.push_back(s.substr(start, end - start));
        start = end + 2;
    }
}

void init_request_struct(http_request*& request) {
    request->request_line = "";
    request->host = "";
    request->uri = "";
	request->content_length = 0;
    request->real_length = 0;
	request->content_type = "";
	request->user_agent = "";
	request->cookie = "";
	request->connection = "";
}

void init_response_struct(http_response*& response) {
    response->status_line = "";
    response->content_length = 0;
    response->real_length = 0;
    response->content_type = "";
    response->content_encoding = "";
    response->server = "";
    response->set_cookie = "";
    response->cache_control = "";
    response->if_modified_since = "";
}

bool parse_request(const string& payload, http_request*& request,
    u_char* origin_payload, int lenth) {
    vector<string> lines;
    split_lines(payload, lines);
    if (lines.empty()) {
        return false;
    }

    // Parse request line
    size_t space1 = lines[0].find(' ');
    size_t space2 = lines[0].rfind(' ');
    if (space1 == string::npos || space2 == string::npos) {
        return false;
    }
    size_t pos = payload.find("\r\n\r\n");
    if (pos == string::npos) {
        return false;
    }
    request->real_length = lenth - pos - 4;
    string method_str = lines[0].substr(0, space1);
    if (method_str == "GET" ||
        method_str == "POST" ||
        method_str == "HEAD" ||
        method_str == "PUT" ||
        method_str == "DELETE" ||
        method_str == "CONNECT" ||
        method_str == "OPTIONS" ||
        method_str == "TRACE") {
        request->request_line = lines[0];
    }
    else {
        return false;
    }
    request->uri = lines[0].substr(space1 + 1, space2 - space1 - 1);

    // Parse headers and body
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << lines[i] << endl;
    }
    lines.clear();
    split_lines(payload.substr(0, pos + 2), lines);
    request->content_length = 0;
    request->content_type = "";
   for (size_t i = 1; i < lines.size(); ++i) {
        size_t colon = lines[i].find(':');
        if (colon == string::npos) {
            return false;
        }
        http_header* header = new http_header;
        header->name = lines[i].substr(0, colon);
        header->value = lines[i].substr(colon + 2);
        if (header->name == "Content-Length" ||
            header->name == "Content-length" ||
            header->name == "content-length") {
            request->content_length = stoi(header->value);
        }
        else if (header->name == "Content-Type" ||
            header->name == "Content-type" ||
            header->name == "content-type") {
            request->content_type = header->value;
        }
        else if (header->name == "Host" ||
            header->name == "host") {
			request->host = header->value;
		}
        else if (header->name == "User-Agent" ||
            header->name == "User-agent" ||
            header->name == "user-agent") {
            request->user_agent = header->value;
        }
        else if (header->name == "Cookie" ||
            header->name == "cookie") {
			request->cookie = header->value;
		}
        else if (header->name == "Connection" ||
            header->name == "connection") {
			request->connection = header->value;
		}
        else request->headers.push_back(*header);
        delete header;
    }
    request->body = strtok((char*)origin_payload, "\r\n\r\n") + 4;
    return true;
}

bool parse_response(const string& payload, http_response*& response,
    u_char* origin_payload, int lenth) {
    vector<string> lines;
    split_lines(payload, lines);
    if (lines.empty()) {
        return false;
    }
    
    // Parse status line
    size_t space1 = lines[0].find(' ');
    size_t space2 = lines[0].rfind(' ');
    if (space1 == string::npos || space2 == string::npos) {
        return false;
    }
    size_t pos = payload.find("\r\n\r\n");
    if (pos == string::npos) {
        return false;
    }
    response->status_line = lines[0];
    response->real_length = lenth - pos - 4;
    // Parse headers and body
    lines.clear();
    split_lines(payload.substr(0, pos + 2), lines);
    response->content_length = 0;
    response->content_type = "";
    for (size_t i = 1; i < lines.size(); ++i) {
        size_t colon = lines[i].find(':');
        if (colon == string::npos) {
            return false;
        }
        http_header *header = new http_header;
        header->name = lines[i].substr(0, colon);
        header->value = lines[i].substr(colon + 2);
        if (header->name == "Content-Length" ||
            header->name == "Content-length" ||
            header->name == "content-length") {
			response->content_length = stoi(header->value);
		}
        else if (header->name == "Content-Type" ||
            header->name == "Content-type" ||
            header->name == "content-type") {
            response->content_type = header->value;
        }
        else if (header->name == "Content-Encoding" ||
            header->name == "Content-encoding" ||
            header->name == "content-encoding") {
			response->content_encoding = header->value;
		}
        else if (header->name == "Server" ||
            header->name == "server") {
			response->server = header->value;
		}
        else if (header->name == "Set-Cookie" ||
            header->name == "Set-cookie" ||
            header->name == "set-cookie") {
			response->set_cookie = header->value;
		}
        else if (header->name == "Cache-Control" ||
            header->name == "Cache-control" ||
            header->name == "cache-control") {
			response->cache_control = header->value;
		}
        else if (header->name == "If-Modified-Since" ||
            header->name == "If-modified-since" ||
            header->name == "if-modified-since") {
			response->if_modified_since = header->value;
		}
        response->headers.push_back(*header);
        delete header;
    }
    response->body = strstr((char*)origin_payload, "\r\n\r\n") + 4;
    return true;
 }

void get_payload(u_char* origin_payload, int len, int count, MYSQL* mysql)
{
    string payload((char*)origin_payload);
    http_request *request = new http_request;
    http_response *response = new http_response;

    bool is_http = false;

    if (parse_request(payload, request, origin_payload, len)) {
        is_http = true;
        cout << "request_line: " << request->request_line << endl;
        cout << "host: " << request->host << endl;
        cout << "uri: " << request->uri << endl;
        for (const auto& header : request->headers) {
            cout << "Header: " << header.name << " = " << header.value << endl;
        }
        if (request->real_length > 0) {
			logger(LOG_INFO, "Body:\n");
			print_payload((u_char*)request->body, request->real_length);
		}

        //query to mysql
        char query[8192];
        sprintf(query, "INSERT INTO sniff_http_request values(%d,'%s','%s','%s','%s','%s',%d,'%s','%s')",
            count, request->request_line.c_str(), request->host.c_str(), request->uri.c_str(), request->user_agent.c_str(), request->cookie.c_str(),
            request->content_length, request->content_type.c_str(), request->connection.c_str());
        int n = mysql_real_query(mysql, query, strlen(query));
        if (n) {
            cout << "Failed to insert the request:" << mysql_error(mysql) << endl;
        }
    }
    else if (parse_response(payload, response, origin_payload, len)) {
        is_http = true;
        cout << "Status_line: " << response->status_line << endl;
        for (const auto& header : response->headers) {
            cout << "Header: " << header.name << " = " << header.value << endl;
        }
        if (response->real_length > 0) {
            logger(LOG_INFO, "Body:\n");
			print_payload((u_char*)response->body, response->real_length);
		}

        //query to mysql
        char query[8192];
        sprintf(query, "INSERT INTO sniff_http_response values(%d,'%s','%s',%d,'%s','%s','%s','%s','%s')", count,
            response->status_line.c_str(), response->server.c_str(), response->content_length, response->content_type.c_str(), response->content_encoding.c_str(),
            response->set_cookie.c_str(), response->cache_control.c_str(), response->if_modified_since.c_str());
        int n = mysql_real_query(mysql, query, strlen(query));
        if (n) {
            cout << "Failed to insert the  response:" << mysql_error(mysql) << endl;
        }
    }

    // save payload to file
    if (is_http && response->real_length != 0) {
        if (response->content_type == "text/html") {
            string filepath = "/root/payload/" + to_string(count) + ".html";
            ofstream outfile(filepath, ios::out | ios::binary);
            if (!outfile.is_open()) {
                mkdir("/root/payload", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                outfile.open(filepath);
            }
            if (outfile.is_open()) {
                outfile.write(response->body, response->real_length);
                outfile.close();
            }
        }
        else if (response->content_type == "text/plain") {
            string filepath = "/root/payload/" + to_string(count) + ".txt";
            ofstream outfile(filepath, ios::out | ios::binary);
            if (!outfile.is_open()) {
                mkdir("/root/payload", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                outfile.open(filepath);
            }
            if (outfile.is_open()) {
                outfile.write(response->body, response->real_length);
                outfile.close();
            }
        }
        else if (response->content_type == "image/jpeg") {
            string filepath = "/root/payload/" + to_string(count) + ".jpg";
            ofstream outfile(filepath, ios::out | ios::binary);
            if (!outfile.is_open()) {
                mkdir("/root/payload", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                outfile.open(filepath);
            }
            if (outfile.is_open()) {
                outfile.write(response->body, response->real_length);
                outfile.close();
            }
        }
        else {
            string filepath = "/root/payload/" + to_string(count) + ".bin";
            ofstream outfile(filepath, ios::out | ios::binary);
            if (!outfile.is_open()) {
                mkdir("/root/payload", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                outfile.open(filepath);
            }
            if (outfile.is_open()) {
                outfile.write(response->body, response->real_length);
                outfile.close();
            }
        }

	}

    delete request;
	delete response;
}
