#ifndef HTTP_H
#define HTTP_H

#define SERVER_NAME "cherokee"
#define APPLICATION_JSON "application/json"

typedef enum
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE

} http_method;

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
    http_method method;
    int content_length;
} request_header_t;


typedef struct response_header
{
    status_code status;
    char *content_type;
    char *date;
    char *server;
} response_header_t;


typedef struct body
{
    /* data */
} body_t;

typedef struct response
{
    response_header_t header;
    body_t body;
} response_t;


typedef struct request
{
    char *url;
    request_header_t header;
    body_t body;

} request_t;


void send_bad_request(int fd);
void handle_request(char *data, int fd);
void get_file(char *filename);
void send_bad_request(int fd);
void format_response_header(response_header_t response);

#endif