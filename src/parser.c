#include <stdlib.h>

#include "http.h"
#include "parser.h"
#include "log/log.h"


void parse_request(request_t *request, char *data)
{
    request->body = NULL;
    
    int split_pos = (strstr(data, "\r\n\r\n") - data);

    data[split_pos] = '\0';

    char *header  = data, 
         *body    = (data + split_pos + 4);

    // log_trace("header: \n%s", header);
    log_trace("data: \n%s", body);
    parse_header(request, header);
}

void parse_header(request_t *request, char *data) 
{
    parse_method(request, &data); 
    parse_uri(request, &data);
    check_http_version(request, &data);
    parse_content_length(request, &data);
}

void parse_uri(request_t *request, char **data)
{
    size_t url_length = strcspn(*data, " ");
    request->url = malloc(url_length + 1);
    
    if (!request->url) {
        log_error("Worker Malloc failed for request url"); /*TODO : get worker id */
        free(request);
    }

    memcpy(request->url, *data, url_length);

    request->url[url_length] = '\0';

    *data += url_length + 1;
}

/* Assign not supported method if version if not HTTP/1.1 */
void check_http_version(request_t *request, char **data)
{
    size_t version_length = strcspn(*data, "\r\n");

    char version[version_length];

    memcpy(version, *data, version_length );

    version[version_length] = '\0';
    if (strcmp(version, "HTTP/1.1") != 0){
        request->method = UNSUPORTED;
    }

    *data += version_length + 2;
}

void parse_content_length(request_t *request, char **data)
{
    while(*data[0]!='\r' || *data[1]!='\n' || *data[0]!='\n')
    {
        size_t name_length = strcspn(*data, ": ");

        if (name_length == 0)
            break;

        char header[name_length];
        memcpy(header, *data, name_length );
        header[name_length] = '\0';
        *data += name_length + 1;

        size_t value_length = strcspn(*data, "\r\n"); // CLRF

        if (value_length == 0)
            break;

        char value[value_length];
        memcpy(value, *data, value_length );
        value[value_length] = '\0';
        *data += value_length + 2;

        if (strcmp(header, "Content-Length") == 0){
            request->header.content_length = atoi(value);
            break;
        }
    }
}

void parse_method(request_t *request, char **data)
{
    size_t meth_len = strcspn(*data, " ");

    if (memcmp(*data, "GET", strlen("GET")) == 0) {
        request->method = GET;
    } 
    else if (memcmp(*data, "POST", strlen("POST")) == 0) {
        request->method = POST;
    }
    else if (memcmp(*data, "PUT", strlen("PUT")) == 0) {
        request->method = PUT;
    }
    else if (memcmp(*data, "DELETE", strlen("DELETE")) == 0) {
        request->method = DELETE;
    }
    else if (memcmp(*data, "HEAD", strlen("HEAD")) == 0) {
        request->method = HEAD;
    } 
    else {
        request->method = UNSUPORTED;
    }

    *data += meth_len + 1; 
}
