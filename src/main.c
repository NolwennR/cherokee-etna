#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <spawn.h>
#include "manager.h"

#define SERVER_PORT 8050

void parse_argument();

int main (int argc, char *argv[])
{
    int worker_number;
    int   exit_status;
    int   server_port;

    worker_number = 0;
    exit_status   = 0;

   /* if argument specified, uses it as number of workers */

    parse_argument(argc, argv, &worker_number, &server_port);

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