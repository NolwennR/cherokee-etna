#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdlib.h>
#include <string.h>

#include "http.h"

void ok(response_t *response, connection_instance_t *connection);
void not_found(response_t *response, connection_instance_t *connection);
void internal_server_error(response_t *response, connection_instance_t *connection);
void bad_request(response_t *response, connection_instance_t *connection);
void not_implemented(response_t *response, connection_instance_t *connection);

void send_response(response_t *response, connection_instance_t *connection);

void free_response(response_t *response);
int set_current_time(response_header_t *header);

#endif // !REPONSE_H