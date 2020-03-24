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

    response_header_t header = {
        .status = OK,
        .server = SERVER_NAME,
    };

    if (header.content_type == NULL) {
        header.content_type = APPLICATION_JSON;
    }

    response->header = header;

    send_response(response, connection);
}

void not_found(response_t *response, connection_instance_t *connection)
{
    log_trace("Worker %d send NOT_FOUND", connection->worker_id);

    const char* body_content = "Ressource not found\0";
    response->body = malloc(sizeof(body_content));

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

    response->body = "Internal server error !\0";
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
    log_trace("Worker %d send BAD REQUEST", connection->worker_id);

    response->body = "Bad Request !\0";
    response_header_t header = {
        .status = BAD_REQUEST,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
        .content_length = strlen(response->body),
    };

    response->header = header;

    send_response(response, connection);
}

void send_response(response_t *response, connection_instance_t *connection)
{
    char *response_content;
    // const char *xml_content_type = "text/html\0";

    if (set_current_time(&(response->header)) != 0)
    {
        log_error("Couldn't set time in response header");
    }
    /*TODO: move content type */
    // response->header.content_type = malloc(sizeof(*xml_content_type));
    // strcpy(response->header.content_type, xml_content_type);

    format_response(response, &response_content);
    write(connection->client_fd, response_content , strlen(response_content));


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