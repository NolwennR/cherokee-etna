#include "http.h"
#include "parser.h"
#include "log.h"

request_t parse_request(char *data)
{
    log_trace("data : %s", data);
    parse_header();

    parser_body();

    request_t request = {  };

    return request;
}

void parse_header() {}

void parser_body() {}