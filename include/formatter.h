#ifndef FORMATTER_H
#define FORMATTER_H

#include <stdlib.h>
#include "http.h"

const char *format_response_to_string();
void add_reason_phrase(status_code status, char **reason_phrase);
int set_current_time(response_header_t *header);
char *format_response(response_t *response);

#endif