#ifndef HTTP_H
#define HTTP_H

#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

#define SERVER_NAME "cherokee\0"
#define APPLICATION_JSON "application/json\0"

typedef enum
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    UNSUPORTED
} http_method;

typedef struct connection_instance
{
    struct epoll_event *event;
    int epoll_fd;
    int client_fd;
    int worker_id;
} connection_instance_t;

typedef enum 
{
    OK = 200,
    CREATED = 201,
    NO_CONTENT = 204,
    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    METHOD_NOT_ALLOWED = 405,
    NOT_FOUND = 404,
    I_AM_A_TEA_POT = 418,
    SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
} status_code;

typedef struct request_header
{
    int content_length;
} request_header_t;

typedef struct response_header
{
    status_code status;
    char *content_type;
    int content_length;
    char *date;
    char *server;
} response_header_t;

typedef struct response
{
    response_header_t header;
    char *body;
} response_t;

typedef struct request
{
    char *url;
    http_method method;
    request_header_t header;
    char *body;

} request_t;

void send_bad_request(int fd);
void handle_request(char *data, connection_instance_t *connection);
void clear_client(connection_instance_t *connection);
void free_request(request_t *request);
void get_on_url(request_t *request, connection_instance_t *connection);
void handle_method(request_t *request, connection_instance_t *connection);
char *get_folder(char *path);
char *get_file_name(char *path);
void remove_argument(char **path);

#endif