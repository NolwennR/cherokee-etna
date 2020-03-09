#ifndef RESPONSE_H
#define RESPONSE_H


#include "http.h"

void send_response(response_t response, connection_instance_t *connection);
void ok(response_t *response, connection_instance_t *connection);
void not_found(response_t *response, connection_instance_t *connection);
void internal_server_error(response_t *response, connection_instance_t *connection);
void bad_request(response_t *response, connection_instance_t *connection);
void format_response_header(response_header_t *response);


#endif // !REPONSE_H