#include "response.h"
#include "server.h"
#include "http.h"
#include "formatter.h"
#include "log/log.h"

const char *http_status_array[] = {
  "GET",
  "HEAD",
  "POST",
  "PUT",
  "DELETE",
  "UNSUPORTED"
};


void ok(response_t *response, connection_instance_t *connection)
{
    log_trace("Worker %d send OK", connection->worker_id);

    response->header.status = OK;
    response->header.server = SERVER_NAME;

    if (response->body && response->header.content_length == 0){
        response->header.content_length = strlen(response->body);
    }

    if (response->header.content_type == NULL) {
        response->header.content_type = APPLICATION_JSON;
    }

    send_response(response, connection);
}

void not_found(response_t *response, connection_instance_t *connection)
{
    log_trace("Worker %d send NOT_FOUND", connection->worker_id);

    const char* body_content = "Ressource not found\0";
    response->body = malloc(strlen(body_content));

    if (!(response->body))
    {
        log_error("malloc() in not_found() for worker %d", connection->worker_id);
        free_response(response);
        return;
    }

    strcpy(response->body, body_content);
    response_header_t header = {
        .status = NOT_FOUND,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
        .content_length = strlen(response->body),
    };

    response->header = header;
    log_trace("Worker \"%s\" response body", response->body);

    send_response(response, connection);
}

void internal_server_error(response_t *response, connection_instance_t *connection)
{
    log_trace("Worker %d send SERVER ERROR", connection->worker_id);

    const char* body_content = "Internal server error !\0";
    response->body = malloc(strlen(body_content));
    strcpy(response->body, body_content);

    response_header_t header = {
        .status = SERVER_ERROR,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
        .content_length = strlen(response->body),
    };

    response->header = header;

    send_response(response, connection);
}

void bad_request(response_t *response, connection_instance_t *connection)
{
    log_trace("Worker %d send Bad Request", connection->worker_id);

    const char* body_content = "Bad Request\0";
    response->body = malloc(strlen(body_content));

    if (!(response->body))
    {
        log_error("malloc() in not_found() for worker %d", connection->worker_id);
        free_response(response);
        return;
    }

    strcpy(response->body, body_content);
    response_header_t header = {
        .status = BAD_REQUEST,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
        .content_length = strlen(response->body),
    };

    response->header = header;
    log_trace("Worker \"%s\" response body", response->body);

    send_response(response, connection);
}

void not_implemented(response_t *response, connection_instance_t *connection)
{
    log_trace("Worker %d send NOT_FOUND", connection->worker_id);

    const char* body_content = "Not Implemented\0";
    response->body = malloc(strlen(body_content));

    if (!(response->body))
    {
        log_error("malloc() in not_found() for worker %d", connection->worker_id);
        free_response(response);
        return;
    }

    strcpy(response->body, body_content);
    response_header_t header = {
        .status = NOT_IMPLEMENTED,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
        .content_length = strlen(response->body),
    };

    response->header = header;
    log_trace("Worker \"%s\" response body", response->body);

    send_response(response, connection);
}

void send_response(response_t *response, connection_instance_t *connection)
{
    char *response_content;
    int size = 0;
    if (set_current_time(&(response->header)) != 0)
    {
        log_error("Couldn't set time in response header");
    }

    size = format_response(response, &response_content);
    write(connection->client_fd, response_content , size);

    log_trace("send %s", response_content);

    free(response_content);
    free_response(response);
}

void free_response(response_t *response)
{
    free(response->body);
    free(response);
}

int set_current_time(response_header_t *header)
{
    char buf[100] = {0};
    time_t rawtime = time(NULL);
    
    if (rawtime == -1) {
        return -1;
    }
    
    struct tm *ptm = localtime(&rawtime);
    
    if (ptm == NULL) {  
        return -1;
    }

    strftime(buf, 300, "%d/%m/%Y", ptm);

    header->date = malloc(sizeof(buf + 1));
    strcpy(header->date, buf);

    return 0;
}