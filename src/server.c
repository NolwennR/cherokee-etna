#include "server.h"
#include "manager.h"
#include "banner.h"
#include "log/log.h"

int main (int argc, char **argv)
{
    int worker_number;
    int   exit_status;
    int   server_port;

    worker_number = 0;
    exit_status   = 0;

   /* if argument specified, uses it as number of workers */
    parse_argument(argc, argv, &worker_number, &server_port);

    write_banner();

    exit_status = check_static_file_folder();

    if (exit_status == 1)
        return exit_status;

    exit_status = run_server(worker_number, server_port);

    return exit_status;
}

void parse_argument(int argc, char *argv[], int *worker_number, int *server_port) {

    if (argc >= 2)
        *worker_number = atoi(argv[1]);
    else
        *worker_number = 1;

    if (argc >= 3)
        *server_port = atoi(argv[2]);
    else
        *server_port = SERVER_PORT;
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