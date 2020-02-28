#include <time.h>
#include "http.h"
#include "parser.h"
#include "log.h"

void handle_request(char *data, int fd)
{
    request_t request = parse_request(data);

    if (request.header.content_length == 0)
    {
        send_bad_request(fd);
    }
}

void get_file(char *filename) 
{
    log_trace(filename);
}

void send_bad_request(int fd)
{   
    response_header_t header = {
        .status = BAD_REQUEST,
        .content_type = APPLICATION_JSON,
        .date = "",
        .server = SERVER_NAME,
    };

    response_t bad_request_response = {
        header = header,
    };

    log_trace("response bad request : %d", bad_request_response.header.status);
    log_trace("fd : %d", fd);
}

void send_response(int fd, response_t response)
{
    log_trace("fd : %d", fd);
    log_trace("response status %s", response.header.status);
}


void format_response_header(response_header_t response)
{
    char buf[300] = {0};
    
    time_t rawtime = time(NULL);
    
    if (rawtime == -1) {
        
        puts("The time() function failed");
    }
    
    struct tm *ptm = localtime(&rawtime);
    
    if (ptm == NULL) {
     
        puts("The localtime() function failed");
    }

    strftime(buf, 300, "%d/%m/%Y", ptm);
    puts(buf);

    response.date = buf;

    log_trace("date : %s", response.date);

}

