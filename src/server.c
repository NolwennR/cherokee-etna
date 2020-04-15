#include "server.h"
#include "manager.h"
#include "banner.h"
#include "log/log.h"
#include "inih/ini.h"
#include "config.h"

int main (int argc, char **argv)
{
    int         exit_status;

    init_configuration();
    
    configuration_t *config = get_configuration();

    exit_status   = 0;

    write_banner();

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
