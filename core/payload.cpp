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

bool parse_request(const string& payload, http_request*& request, u_char* origin_payload) {
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
    string method_str = lines[0].substr(0, space1);
    if (method_str == "GET") {
        request->method = http_mthd::HTTP_MT_GET;
    }
    else if (method_str == "POST") {
        request->method = http_mthd::HTTP_MT_POST;
    }
    else if (method_str == "PUT") {
        request->method = http_mthd::HTTP_MT_PUT;
    }
    else if (method_str == "DELETE") {
        request->method = http_mthd::HTTP_MT_DELETE;
    }
    else {
        return false;
    }
    request->uri = lines[0].substr(space1 + 1, space2 - space1 - 1);
    request->version = lines[0].substr(space2 + 1);

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
        else request->headers.push_back(*header);
        delete header;
    }
    request->body = strtok((char*)origin_payload, "\r\n\r\n") + 4;
    return true;
}

bool parse_response(const string& payload, http_response*& response, u_char* origin_payload) {
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
    response->version = lines[0].substr(0, space1);
    response->status_code = stoi(lines[0].substr(space1 + 1, space2 - space1 - 1));
    response->status_text = lines[0].substr(space2 + 1);
    
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
        else response->headers.push_back(*header);
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

    if (parse_request(payload, request, origin_payload)) {
        is_http = true;
        cout << "Method: " << static_cast<int>(request->method) << endl;
        cout << "URI: " << request->uri << endl;
        cout << "Version: " << request->version << endl;
        if (request->content_length > 0) {
            cout << "Content length: " << request->content_length << endl;
            cout << "Content type: " << request->content_type << endl;
        }
        for (const auto& header : request->headers) {
            cout << "Header: " << header.name << " = " << header.value << endl;
        }
        if (request->content_length > 0) {
			logger(LOG_INFO, "Body:\n");
			print_payload((u_char*)request->body, request->content_length);
		}
    }
    else if (parse_response(payload, response, origin_payload)) {
        is_http = true;
        cout << "Version: " << response->version << endl;
        cout << "Status code: " << response->status_code << endl;
        cout << "Status text: " << response->status_text << endl;
        if (response->content_length > 0) {
			cout << "Content length: " << response->content_length << endl;
            cout << "Content type: " << response->content_type << endl;
		}
        for (const auto& header : response->headers) {
            cout << "Header: " << header.name << " = " << header.value << endl;
        }
        if (response->content_length > 0) {
            logger(LOG_INFO, "Body:\n");
			print_payload((u_char*)response->body, response->content_length);
		}
    }

    // save payload to file
    /*if (is_http) {
        string filepath = "path/to/file.txt";
        ofstream outfile(filepath, ios::out | ios::binary);
        if (!outfile.is_open()) {
            mkdir("test", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            outfile.open(filepath);
        }
        if (outfile.is_open()) {
            outfile.write(response->body.c_str(), response->body.length());
            outfile.close();
        }
	}*/

    delete request;
	delete response;
}
