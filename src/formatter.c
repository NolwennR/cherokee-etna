#include "formatter.h"
#include "http.h"
#include "log/log.h"

const char *format_response_to_string(response_t response)
{
    log_trace("reponse status %d", response.header.status);
    char *content;
    content =  "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world";

    return content;
}

char *format_response(response_t *response)
{
    char status_str[10];
    char *status_line;

    char *http_version = "HTTP/1.1 ";
    char *reason_phrase;
    
    add_reason_phrase(response->header.status, &reason_phrase);

    sprintf(status_str, "%d", response->header.status);
    status_line = malloc((sizeof(*http_version) + sizeof(status_str) + sizeof(reason_phrase)));
    strcpy(status_line, http_version);
    strncat(status_line, status_str, strlen(status_str));
    strncat(status_line, reason_phrase, strlen(reason_phrase));

    log_trace("response content: %s", status_line);

    return status_line;
}

void add_reason_phrase(status_code status, char **reason_phrase)
{
    switch (status)
    {
    case OK:
        *reason_phrase = " Ok\n";
        break;
    case NOT_FOUND:
        *reason_phrase = " Not found\n";
        break;
    case CREATED:
        *reason_phrase = " Created\n";
        break;
    default:
        break;
    }
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
    log_trace("Current time: %s", buf);

    header->date = buf;

    return 0;
}