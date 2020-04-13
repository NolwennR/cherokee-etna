#include "log/log.h"
#include "http.h"
#include "parser.h"
#include "formatter.h"
#include "static_file.h"
#include "server.h"

const char *http_method_array[] = {
  "GET",
  "HEAD",
  "POST",
  "PUT",
  "DELETE",
  "UNSUPORTED"
}; 

void handle_request(char *data, connection_instance_t *connection, configuration_t *config)
{
    request_t *request = malloc(sizeof(request_t));

    if (request == NULL) {
        log_error("Worker %d in malloc request", connection->worker_id);
        return;
    }

    parse_request(request, data);

    handle_method(request, connection, config);  

    free_request(request);
}

void handle_method(request_t *request, connection_instance_t *connection, configuration_t *config)
{
    switch (request->method)
    {
        case GET:
            get_on_url(request, connection, config);
            break;
        case POST:
            post_on_url(request, connection, config);
            break;
        case PUT:
            put_on_url(request, connection, config);
            break;
        case DELETE:
            delete_on_url(request, connection, config);
            break;
        case HEAD:
            head_on_url(request, connection, config);
            break;
        case UNSUPORTED:
            break;
        default:
            break;
    }
}

void get_on_url(request_t *request, connection_instance_t *connection, configuration_t *config)
{
    remove_argument(&request->url);
    serve_static_file(request, connection, config);
}

void post_on_url(request_t *request, connection_instance_t *connection, configuration_t *config)
{
    
}

void put_on_url(request_t *request, connection_instance_t *connection, configuration_t *config)
{

}

void delete_on_url(request_t *request, connection_instance_t *connection, configuration_t *config)
{

}

void head_on_url(request_t *request, connection_instance_t *connection, configuration_t *config)
{

}

void remove_argument(char **path)
{
    char *argument = strchr(*path, '?');
    
    if (!argument)
        return;

    int split_pos = (argument - *path);
    (*path)[split_pos] = '\0';
}

void free_request(request_t *request)
{
    free(request->url);
    free(request->body);
    free(request);
}

void clear_client(connection_instance_t *epoll_instance)
{
    if (epoll_ctl(epoll_instance->epoll_fd, EPOLL_CTL_DEL, epoll_instance->client_fd, epoll_instance->event)) {
        log_error("Worker %d in epoll_ctl", epoll_instance->worker_id);
        perror("In epoll_ctl");
    }

    close(epoll_instance->client_fd);
}







