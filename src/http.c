#include "log/log.h"
#include "http.h"
#include "parser.h"
#include "formatter.h"
#include "static_file.h"
#include "server.h"
#include "crud.h"
#include "response.h"

const char *http_method_array[] = {
  "GET",
  "HEAD",
  "POST",
  "PUT",
  "DELETE",
  "UNSUPORTED"
}; 

void init_header(response_header_t* header){
  header->content_length = 0;
  header->content_type = NULL;
  header->status = UNDEFINED;
  header->server = NULL;
  header->date = NULL;
};

void handle_request(char *data, connection_instance_t *connection, configuration_t *config)
{
    request_t *request = malloc(sizeof(request_t));
    log_trace("request address: %p", request);


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
  response_t *response = malloc(sizeof(response_t));

  if (!response) {
    log_error("malloc() response_t failed in static file response for worker %d", connection->worker_id);
    return;
  }

  remove_argument(&request->url);
  init_header(&(response->header));
  response->body = NULL;
  
  switch (request->method)
  {
      case GET: {
          if(strcmp(config->get_conf.url, request->url) == 0)
            handle_py_call(response, connection, &config->get_conf);
          else 
            get_on_url(request, connection, response, config);
          break;
      }
      case POST: {
          if(strcmp(config->get_conf.url, request->url) == 0)
            handle_py_call(response, connection, &config->post_conf);
          break;
      }
      case PUT: {
          if(strcmp(config->get_conf.url, request->url) == 0)
            handle_py_call(response, connection, &config->put_conf);
          break;
      }
      case DELETE: {
          if(strcmp(config->get_conf.url, request->url) == 0)
            handle_py_call(response, connection, &(config->delete_conf));
          break;
      }
      case HEAD: {
          if(strcmp(config->get_conf.url, request->url) == 0)
            handle_py_call(response, connection, &config->head_conf);
          break;
      }
      case UNSUPORTED:
          break;
      default:
          break;
  }

}

void get_on_url(request_t *request, connection_instance_t *connection, response_t *response, configuration_t* config)
{
  serve_static_file(request, connection, response, config);  
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
    log_trace("request address before free: %p", request);

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







