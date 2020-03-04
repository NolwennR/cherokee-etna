#include <stdlib.h>

#include "http.h"
#include "parser.h"
#include "log.h"
#include "formatter.h"

void handle_request(char *data, epoll_instance_t *epoll_instance)
{
    request_t *request = malloc(sizeof(request_t));

    if (request == NULL)
    {
        log_error("Worker %d in malloc request", epoll_instance->worker_id);
        perror("In malloc");
    }

    parse_request(request, data);

    if (request->header.content_length == 0)
    {
    }

    send_bad_request(epoll_instance->client_fd);
    clear_client(epoll_instance);

    free(request);
}

void clear_client(epoll_instance_t *epoll_instance)
{
    if (epoll_ctl(epoll_instance->epoll_fd, EPOLL_CTL_DEL, epoll_instance->client_fd, epoll_instance->event))
    {
        log_error("Worker %d in epoll_ctl", epoll_instance->worker_id);
        perror("In epoll_ctl");
    }

    close(epoll_instance->client_fd);
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
        "hello wordld !"
    };

    format_response_header(header);
    log_trace("response bad request : %d", bad_request_response.header.status);
    log_trace("fd : %d", fd);

    send_response(fd, bad_request_response);
}

void send_response(int fd, response_t response)
{
    const char *response_content = format_response_to_string(response);

    log_trace("send %s", response_content);
    write(fd , response_content , strlen(response_content));
}


void format_response_header(response_header_t response)
{
    char buf[300] = {0};
    
    time_t rawtime = time(NULL);
    
    if (rawtime == -1) {
        
        puts("The time() function failed");
    }
    
    struct tm *ptm = localtime(&rawtime);
    
    if (ptm == NULL) 
    {
        puts("The localtime() function failed");
    }

    strftime(buf, 300, "%d/%m/%Y", ptm);
    puts(buf);

    response.date = buf;

    log_trace("date : %s", response.date);
}

