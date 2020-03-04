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
    data += parse_method(&request->header, data); //move
    log_trace("data after method parsing :\n%s", data);
    log_trace("Method : %s", http_method_array[request->header.method]);

    /* URI */
    data += parse_uri(request, data); // move
    log_trace("Url: %s", request->header.url);

    /* HTTP-Version */
    data += check_http_version(request, data); // move \n\r
    log_trace("Method after version checking: %s", http_method_array[request->header.method]);
}

int parse_uri(request_t *request, char *data)
{
    size_t url_length = strcspn(data, " ");
    request->header.url = malloc(url_length + 1);
    
    if (!request->header.url) 
    {
        log_error("Worker Malloc failed for request url"); /*TODO : get worker id */
        free(request);
    }

    memcpy(request->header.url, data, url_length);
    request->header.url[url_length] = '\0';

    return url_length + 1;
}

int check_http_version(request_t *request, char *data)
{
    size_t version_length = strcspn(data, "\r\n");

    char version[version_length];
    memcpy(version, data, version_length );
    version[version_length] = '\0';

    log_trace("version: %s", version);

    if (strcmp(version, "HTTP/1.1") != 0)
    {
        request->header.method = UNSUPORTED;
    }

    return version_length + 2;
}

// int parse_content_length(request_header_t *header, char *data)
// {

// }

int parse_method(request_header_t *header, char *data)
{
    size_t meth_len = strcspn(data, " ");

    if (memcmp(data, "GET", strlen("GET")) == 0) 
    {
        (*header).method = GET;
    } 
    else if (memcmp(data, "POST", strlen("POST")) == 0) 
    {
        (*header).method = POST;
    }
    else if (memcmp(data, "PUT", strlen("PUT")) == 0) 
    {
        (*header).method = PUT;
    }
    else if (memcmp(data, "DELETE", strlen("DELETE")) == 0) 
    {
        (*header).method = DELETE;
    } 
    else 
    {
        (*header).method = UNSUPORTED;
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