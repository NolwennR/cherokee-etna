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

#define SERVER_PORT 8070

typedef struct method_conf
{
    const char *url;
    const char *function_name;
} method_conf_t;

typedef struct configuration
{
    int port;
    int workers;
    int log_level;
    const char *python_file;
    const char *static_file_folder;
    method_conf_t get_conf;
    method_conf_t post_conf;
    method_conf_t put_conf;
    method_conf_t delete_conf;
    method_conf_t head_conf;

} configuration_t;

void parse_argument();
int check_static_file_folder();
int handler(void* user, const char* section, const char* name, const char* value);

#endif