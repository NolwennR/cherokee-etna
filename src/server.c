#include "server.h"
#include "manager.h"
#include "banner.h"
#include "log/log.h"
#include "inih/ini.h"

int main (int argc, char **argv)
{
    int         exit_status;
    configuration_t *config;

    exit_status   = 0;


    write_banner();

    exit_status = check_static_file_folder();

    config = malloc(sizeof(configuration_t));
    if (!config){
        log_error("malloc failed for config");
        return 1;
    }

    if (ini_parse("config.ini", handler, config) < 0) {
        log_fatal("Can't load 'config.ini'\n");
        return 1;
    }

    log_info("Config loaded from 'config.ini': port=%d, workers=%d, log_level=%d, static file folder=%s\n",
        config->port, config->workers, config->log_level, config->static_file_folder);

    parse_argument(argc, argv, config);

    if (config->log_level > 5 || config->log_level < 0) {
        log_fatal("Log level must be between 0 and 5\n");
        return 1;
    }

    log_set_level(config->log_level);

    if (exit_status == 1)
        return exit_status;

    exit_status = run_server(config);

    return exit_status;
}

/* Override config.ini if arguments exist */
void parse_argument(int argc, char *argv[], configuration_t *config) {

    if (argc >= 2)
        config->workers = atoi(argv[1]);
    if (argc >= 3)
        config->port = atoi(argv[2]);
}

int check_static_file_folder()
{
    log_info("Checking for static files folder...");

    struct stat s;
    int err = stat("./files", &s);

    if(-1 == err) 
    {
        log_fatal("Couldn't find directory for static files \"file\". It's mandatory for programme startup.");
        return 1;
    }

    return 0;
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
    } else if (MATCH("global", "python_file")) {
        pconfig->python_file = strdup(value);
    } else if (MATCH("global", "static_file_folder")) {
        pconfig->static_file_folder = strdup(value);
    } else if (MATCH("get", "url")) {
        pconfig->get_conf.url = strdup(value);
    } else if (MATCH("get", "function_name")) {
        pconfig->get_conf.function_name = strdup(value);
    } else if (MATCH("post", "url")) {
        pconfig->post_conf.url = strdup(value);
    } else if (MATCH("post", "function_name")) {
        pconfig->post_conf.function_name = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}