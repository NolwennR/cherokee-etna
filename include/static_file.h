#ifndef STATIC_FILE_H
#define STATIC_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> 
#include <stdlib.h>
#include "http.h"

void serve_static_file(request_t *request, connection_instance_t *connection);

#endif