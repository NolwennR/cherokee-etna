#ifndef FORMATTER_H
#define FORMATTER_H

#include <stdlib.h>
#include <string.h>
#include "http.h"

char *format_response_to_string();
void add_reason_phrase(status_code status, char **reason_phrase);
void format_response(response_t *response, char **formatted_response);
void add_status_line(status_code status,char **status_line);
void add_date_header(char *date, char **date_header);
void add_server_header(char *server, char **server_header);
void add_content_type_header(char *content_type, char **content_type_header);
void add_content_length_header(int *content_length, char **content_length_header);

#endif