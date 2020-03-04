#ifndef PARSER_H
#define PARSER_H

#include "http.h"

void parse_request(request_t *request,char *data);
void parse_header(request_t *request, char *data);
int parse_method(request_header_t *header, char *data);
int parse_uri(request_t *request, char *data);
int check_http_version(request_t *request, char *data);
void parse_body(request_t *request, char *data);
int parse_content_length(request_header_t *header, char *data);
// void find_body(char *data);

#endif