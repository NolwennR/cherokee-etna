#include <stdlib.h>

#include "http.h"
#include "parser.h"
#include "log.h"

/* To be able to printf http_method enum */
const char *http_method_array[] = {"GET","HEAD","POST", "PUT", "DELETE", "UNSUPORTED"}; 

void parse_request(request_t *request, char *data)
{
    log_trace("data : \n%s", data);

     // find_body(data);
    
    parse_header(request, data);

    // parse_body(request, data);
}

void parse_header(request_t *request, char *data) 
{
    /* HTTP Method */
    data += parse_method(request, data); //move
    log_trace("Method : %s", http_method_array[request->method]);

    /* URI */
    data += parse_uri(request, data); // move
    log_trace("Url: %s", request->url);

    /* HTTP-Version */
    data += check_http_version(request, data); // move \n\r
    log_trace("Method after version checking: %s", http_method_array[request->method]);

    // data += 
    parse_content_length(request, &data);
    log_trace("Content-Length : %d", request->header.content_length);

    log_trace("data after parsing :\n%s", data);
}

int parse_uri(request_t *request, char *data)
{
    size_t url_length = strcspn(data, " ");
    request->url = malloc(url_length + 1);
    
    if (!request->url) 
    {
        log_error("Worker Malloc failed for request url"); /*TODO : get worker id */
        free(request);
    }

    memcpy(request->url, data, url_length);
    request->url[url_length] = '\0';

    return url_length + 1;
}

/* Assign not supported method if version if not HTTP/1.1 */
int check_http_version(request_t *request, char *data)
{
    size_t version_length = strcspn(data, "\r\n");

    char version[version_length];
    memcpy(version, data, version_length );
    version[version_length] = '\0';

    log_trace("version: %s", version);

    if (strcmp(version, "HTTP/1.1") != 0)
    {
        request->method = UNSUPORTED;
    }

    return version_length + 2;
}

void parse_content_length(request_t *request, char **data) /* TODO: Return void, inconsistence ? */
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

        if (strcmp(header, "Content-Length") == 0)
        {
            request->header.content_length = atoi(value);
            // break;
        }
    }
}

int parse_method(request_t *request, char *data)
{
    size_t meth_len = strcspn(data, " ");

    if (memcmp(data, "GET", strlen("GET")) == 0) 
    {
        request->method = GET;
    } 
    else if (memcmp(data, "POST", strlen("POST")) == 0) 
    {
        request->method = POST;
    }
    else if (memcmp(data, "PUT", strlen("PUT")) == 0) 
    {
        request->method = PUT;
    }
    else if (memcmp(data, "DELETE", strlen("DELETE")) == 0) 
    {
        request->method = DELETE;
    } 
    else 
    {
        request->method = UNSUPORTED;
    }

    return meth_len + 1; 
}

// void find_body(char *data)
// {
//     // \r\n\r\n
// }

// void parse_body(request_t *request, char *body) 
// {
//     log_trace("body %s", body);
// }