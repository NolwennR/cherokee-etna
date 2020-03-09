#include "server.h"
#include "http.h"
#include "log/log.h"

void ok(response_t *response, connection_instance_t *connection)
{
    response_header_t header = {
        .status = OK,
        .server = SERVER_NAME,
    };

    if (header.content_type == NULL) {
        header.content_type = APPLICATION_JSON;
    }

    response_t response = {
        header = header,
    };

    send_response(response, connection);
}

void not_found(response_t *response, connection_instance_t *connection)
{
    response_header_t header = {
        .status = NOT_FOUND,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
    };

    response_t response = {
        header = header,
        "Ressource not found"
    };

    send_response(response, connection);
}

void internal_server_error(response_t *response, connection_instance_t *connection)
{
    response_header_t header = {
        .status = SERVER_ERROR,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
    };

    response_t response = {
        header = header,
        "Internal server error !"
    };

    send_response(response, connection);
}

void bad_request(response_t *response, connection_instance_t *connection)
{
    response_header_t header = {
        .status = BAD_REQUEST,
        .content_type = APPLICATION_JSON,
        .server = SERVER_NAME,
    };

    response_t bad_request_response = {
        header = header,
        "Bad Request !"
    };

    send_response(response, connection);
}

void send_response(response_t *response, connection_instance_t *connection)
{
    const char *response_content;

    if (set_current_time(&(response->header)) != 0)
    {
        log_error("Couldn't set time in response header");
    }

    response_content = format_response_to_string(response);
    write(connection->client_fd, response_content , strlen(response_content));

    log_trace("send %s", response_content);

    free_response(response);
}

void free_response(response_t *response)
{
    free(response->body);
    free(response->header.date);
    free(response->header.server);
    free(response);
}

int set_current_time(response_header_t *header)
{
    char buf[300] = {0};
    time_t rawtime = time(NULL);
    
    if (rawtime == -1) {
        return -1;
    }
    
    struct tm *ptm = localtime(&rawtime);
    
    if (ptm == NULL) {  
        return -1;
    }

    strftime(buf, 300, "%d/%m/%Y", ptm);
    puts(buf);
    header->date = buf;

    return 0;
}