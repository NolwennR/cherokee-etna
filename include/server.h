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

#include "cache.h"

#define SERVER_PORT 8070

void parse_argument();
int check_static_file_folder();
int handler(void* user, const char* section, const char* name, const char* value);

#endif