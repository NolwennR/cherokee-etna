#include "formatter.h"
#include "http.h"
#include "log/log.h"

char *format_response_to_string(response_t response)
{
    log_trace("reponse status %d", response.header.status);
    char *content;
    content =  "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world";

    return content;
}

int format_response(response_t *response, char **formatted_response)
{
    int size;
    char *status_line;
    char *date_header;
    char *server_header;
    char *content_type_header;
    char *content_length_header;
    char *body_separator = "\r\n\0";
    size = 0;

    size = strlen(body_separator);
    add_status_line(response->header.status, &status_line);
    size += strlen(status_line);  
    add_date_header(response->header.date, &date_header);
    size += strlen(date_header);  

    if (response->body){
        size += response->header.content_length;
    }
    if (response->header.server){
        add_server_header(response->header.server, &server_header);
        size += strlen(server_header);
    }
    if (response->header.content_type){
        add_content_type_header(response->header.content_type, &content_type_header);
        size += strlen(content_type_header);
    }
    if (response->header.content_length)
    {
        add_content_length_header(&response->header.content_length, &content_length_header);
        size += strlen(content_length_header);
    }

    *formatted_response = malloc(size + 1);

    strcpy(*formatted_response, status_line);
    strcat(*formatted_response, date_header);

    if (response->header.server){
        strcat(*formatted_response, server_header);
    }
    if (response->header.content_type){
        strcat(*formatted_response, content_type_header);
    }
    if (response->header.content_length){
        strcat(*formatted_response, content_length_header);
    }
    strcat(*formatted_response, body_separator);
    if (response->body){
        memcpy((*formatted_response + size - response->header.content_length), response->body, response->header.content_length);
    }

    free(status_line);
    free(date_header);
    free(server_header);
    free(content_type_header);
    free(content_length_header);

    return size;
}

void add_status_line(status_code status,char **status_line)
{
    char *http_version = "HTTP/1.1 \0";
    char status_str[10];
    char *reason_phrase;

    switch (status)
    {
    case OK:
        reason_phrase = " Ok\n\0";
        break;
    case NOT_FOUND:
        reason_phrase = " Not found\n\0";
        break;
    case CREATED:
        reason_phrase = " Created\n\0";
        break;
    default:
        break;
    }

    sprintf(status_str, "%d", status);
    status_str[9] = '\0';

    *status_line = malloc(strlen(http_version) + strlen(reason_phrase) + strlen(status_str) + 1);

    strcpy(*status_line, http_version);
    strcat(*status_line, status_str);
    strcat(*status_line, reason_phrase);
}

void add_date_header(char *date, char **date_header)
{
    char *prop = "Date: ";

    *date_header = malloc(strlen(prop) + strlen(date) + strlen("\n\0") + 1);
    strcpy(*date_header, prop);
    strcat(*date_header, date);
    strcat(*date_header, "\n");
}

void add_server_header(char *server, char **server_header)
{
    char *prop = "Server: ";
    
    *server_header = malloc(strlen(prop) + strlen(server) + strlen("\n\0") + 1);
    strcpy(*server_header, prop);
    strcat(*server_header, server);
    strcat(*server_header, "\n");
}

void add_content_type_header(char *content_type, char **content_type_header)
{
    char *prop = "Content-type: ";

    *content_type_header = malloc(strlen(prop) + strlen(content_type) + strlen("\n\0") + 1);
    strcpy(*content_type_header, prop);
    strcat(*content_type_header, content_type);
    strcat(*content_type_header, "\n");
}

void add_content_length_header(int *content_length, char **content_length_header)
{
    char content_length_str[10];

    sprintf(content_length_str, "%d", *content_length);

    char *prop = "Content-lenght: ";
    *content_length_header = malloc(sizeof(*prop) + sizeof(*content_length_str) + sizeof("\n" + 1));
    strcpy(*content_length_header, prop);
    strcat(*content_length_header, content_length_str);
    strcat(*content_length_header, "\n");
}

