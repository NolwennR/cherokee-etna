#include "config.h"

static configuration_t* config;

configuration_t* get_configuration(){
  return config;
}

int handler(void* user, const char* section, const char* name, const char* value)
{
    configuration_t* pconfig = (configuration_t*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("global", "port")) {
        pconfig->port = atoi(value);
    } else if (MATCH("global", "workers")) {
        pconfig->workers = atoi(value);
    } else if (MATCH("global", "log_level")) {
        pconfig->log_level = atoi(value);
    } else if (MATCH("global", "python_folder")) {
        pconfig->python_folder = strdup(value);
    } else if (MATCH("global", "static_file_folder")) {
        pconfig->static_file_folder = strdup(value);
    } else if (MATCH("get", "url")) {
        pconfig->get_conf.url = strdup(value);
    } else if (MATCH("get", "file_name")) {
        pconfig->get_conf.file_name = strdup(value);
    } else if (MATCH("get", "function_name")) {
        pconfig->get_conf.function_name = strdup(value);
    } else if (MATCH("post", "url")) {
        pconfig->post_conf.url = strdup(value);
    } else if (MATCH("post", "file_name")) {
        pconfig->post_conf.file_name = strdup(value);
    } else if (MATCH("post", "function_name")) {
        pconfig->post_conf.function_name = strdup(value);
    } else if (MATCH("put", "url")) {
        pconfig->put_conf.url = strdup(value);
    } else if (MATCH("put", "file_name")) {
        pconfig->put_conf.file_name = strdup(value);
    } else if (MATCH("put", "function_name")) {
        pconfig->put_conf.function_name = strdup(value);
    } else if (MATCH("delete", "url")) {
        pconfig->delete_conf.url = strdup(value);
    } else if (MATCH("delete", "file_name")) {
        pconfig->delete_conf.file_name = strdup(value);
    } else if (MATCH("delete", "function_name")) {
        pconfig->delete_conf.function_name = strdup(value);
    } else if (MATCH("head", "url")) {
        pconfig->head_conf.url = strdup(value);
    } else if (MATCH("head", "file_name")) {
        pconfig->head_conf.file_name = strdup(value);
    } else if (MATCH("head", "function_name")) {
        pconfig->head_conf.function_name = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void init_configuration() {
    if(!config){
      config = malloc(sizeof(configuration_t));
    }
    if (!config){
        log_error("malloc failed for config");
        exit(1);
    }

    if (ini_parse("config.ini", handler, config) < 0) {
        log_fatal("Can't load 'config.ini'\n");
        exit(2);
    }
}