#include <stdlib.h>
#include <unistd.h>

#include "log/log.h"
#include "http.h"
#include "parser.h"
#include "formatter.h"
#include "static_file.h"

const char *http_method_array[] = {
  "GET",
  "HEAD",
  "POST",
  "PUT",
  "DELETE",
  "UNSUPORTED"
}; 

void handle_request(char *data, connection_instance_t *connection)
{
    request_t *request = malloc(sizeof(request_t));

    if (request == NULL) {
        log_error("Worker %d in malloc request", connection->worker_id);
        return;
    }

    parse_request(request, data);

    /* log_trace("Url: %s", request->url);
    log_trace("Method : %s", http_method_array[request->method]);
    log_trace("Content-Length : %d", request->header.content_length); */

    handle_method(request, connection);  

    clear_client(connection);
    free_request(request);
}

void handle_method(request_t *request, connection_instance_t *connection)
{
    switch (request->method)
    {
        case GET:
            get_on_url(request, connection);
            break;
        case POST:
            break;
        case PUT:
            break;
        case DELETE:
            break;
        case HEAD:
            break;
        case UNSUPORTED:
            break;
        default:
            break;
    }
}

void get_on_url(request_t *request, connection_instance_t *connection)
{
    remove_argument(&request->url);
    serve_static_file(request, connection);
}

void remove_argument(char **path)
{
    int split_pos = (strchr(*path, '?') - *path);
    (*path)[split_pos] = '\0';
}

void free_request(request_t *request)
{
    if (!request->url) {
        free(request->url);
    }
    if (!request->body) {
        free(request->body);
    }
    if (!request) {
        free(request);
    }
}

void clear_client(connection_instance_t *epoll_instance)
{
    if (epoll_ctl(epoll_instance->epoll_fd, EPOLL_CTL_DEL, epoll_instance->client_fd, epoll_instance->event)) {
        log_error("Worker %d in epoll_ctl", epoll_instance->worker_id);
        perror("In epoll_ctl");
    }

    close(epoll_instance->client_fd);
}







