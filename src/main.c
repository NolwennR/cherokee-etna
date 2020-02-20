#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <spawn.h>
#include "manager.h"

#define SERVER_PORT 8000

int main (int argc, char *argv[])
{
    int worker_number;
    int          exit;

    worker_number = 0;
    exit = 0;

    if (argc >= 2)
        worker_number = atoi(argv[1]);
    else
        worker_number = 1;

    exit = run_server(worker_number);

    return exit;
}