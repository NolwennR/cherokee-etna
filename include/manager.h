#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include "server.h"

int run_server(configuration_t *config);

#endif