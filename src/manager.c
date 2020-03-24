#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include "manager.h"
#include "log/log.h"
#include "worker.h"

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int run_server(int workers, int port){

    int                    i; 
    int                  pid;
    int               result; 
    int                   on;
    int            listen_fd; 
    SOCKADDR_IN         addr;

    /* To wait child processes  */
    pid_t               wpid;
    int               status;

    status               = 0;
    on                   = 1;

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_fd < 0)
    {
        perror("socket() failed");
        exit(-1);
    }

    /* Allow socket descriptor to be reuseable */
    result = setsockopt(listen_fd,
                    SOL_SOCKET,  SO_REUSEADDR,
                    (char *)&on, sizeof(on));
    if (result < 0)
    {
        log_fatal("setsockopt failed");
        perror("setsockopt() failed");
        close(listen_fd);
        exit(-1);
    }

    /* Bind the socket */

    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    result = bind(listen_fd,(struct sockaddr *)&addr, sizeof(addr));
    if (result < 0)
    {
        log_fatal("bind failed");
        perror("bind() failed");
        close(listen_fd);
        exit(-1);
    }

    /* Set the listen back log (5 connections queued) */
    log_info("Creating listener on port %d...", port);
    result = listen(listen_fd, 5);
    if (result < 0)
    {
        log_fatal("listen failed");
        perror("listen() failed");
        close(listen_fd);
        exit(-1);
    }

    /* Create each of the worker jobs */
    log_info("Creating %d worker jobs...", workers);

    for (i = 1; i < workers + 1; ++i)
    {
        pid = fork();
        switch (pid) {
			case 0 : // child
				handle_connection(i, listen_fd);
				exit(EXIT_SUCCESS);
			case -1 :
				perror("fork");
				return -1;
		}
        if (pid < 0)
        {
            log_fatal("spawn worker failed");
            perror("spawn worker failed");
            close(listen_fd);
            exit(-1);
        }
        log_info("  Worker %d = %d",i, pid);
    }

    /* Inform user that server is ready      */
    log_info("The server is ready");

    while ((wpid = wait(&status)) > 0);

    /* Close down the listening socket               */
    close(listen_fd);
    log_info("Close listener on port %d", port);

    return 0;
}