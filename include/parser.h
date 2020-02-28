#ifndef PARSER_H
#define PARSER_H

#include "http.h"

request_t parse_request(char *data);
void parse_header();
void parser_body();

#endif