#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdlib.h>
#include "server.h"
#include "config.h"

#define MAX_EVENTS 1000

void handle_connection(int id, int server_fd, configuration_t *config);
int set_non_block(int fd);

#endif