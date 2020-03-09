#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SERVER_PORT 8050

void parse_argument();
int check_static_file_folder();

#endif