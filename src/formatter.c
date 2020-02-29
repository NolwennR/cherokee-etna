#include "formatter.h"
#include "http.h"
#include "log.h"

const char *format_response_to_string(response_t response)
{
    log_trace("reponse status %d", response.header.status);
    char *content;
    content =  "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world";

    return content;
}