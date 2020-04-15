#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <log/log.h>
#include <inih/ini.h>
#include <string.h>
#include "cache.h"

typedef struct method_conf
{
    const char *url;
    const char *function_name;
    const char *file_name; 
} method_conf_t;

typedef struct configuration
{
    int port;
    int workers;
    int log_level;
    const char *python_folder;
    const char *static_file_folder;
    method_conf_t get_conf;
    method_conf_t post_conf;
    method_conf_t put_conf;
    method_conf_t delete_conf;
    method_conf_t head_conf;
    lru_cache_t *cache;
} configuration_t;

void init_configuration();

configuration_t* get_configuration();

#endif