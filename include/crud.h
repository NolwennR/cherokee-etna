#ifndef CRUD_H
#define CRUD_H

#define PY_SSIZE_T_CLEAN

#include <python3.6m/Python.h>
#include "config.h"
#include "http.h"
#include "response.h"

typedef struct return_value {
  long int code;
  const char* body;
} return_value_t;

void handle_py_call(response_t* response, connection_instance_t* connection, method_conf_t* method_conf);

#endif