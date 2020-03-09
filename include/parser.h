#ifndef PARSER_H
#define PARSER_H

#include "http.h"

void parse_request(request_t *request,char *data);
void parse_header(request_t *request, char *data);
void parse_method(request_t *request, char **data);
void parse_uri(request_t *request, char **data);
void check_http_version(request_t *request, char **data);
void parse_body(request_t *request, char **data);
void parse_content_length(request_t *request, char **data_ptr);

#endif