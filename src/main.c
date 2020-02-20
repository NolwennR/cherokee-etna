#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <spawn.h>

#define SERVER_PORT 8000

int main (int argc, char *argv[])
{
    int                       i; 
    int                     num;
    int                     pid;
    int                  result; 
    int                      on;
    int               listen_fd; 
    struct sockaddr_in6    addr;

    on = 1;

   /* if argument specified, uses it as number of workers */
    if (argc >= 2)
        num = atoi(argv[1]);
    else
        num = 1;

    listen_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
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
        perror("setsockopt() failed");
        close(listen_fd);
        exit(-1);
    }

    /* Bind the socket */
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family      = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port        = htons(SERVER_PORT);
    result = bind(listen_fd,(struct sockaddr *)&addr, sizeof(addr));
    if (result < 0)
    {
        perror("bind() failed");
        close(listen_fd);
        exit(-1);
    }

    /* Set the listen back log */
    result = listen(listen_fd, 5);
    if (result < 0)
    {
        perror("listen() failed");
        close(listen_fd);
        exit(-1);
    }

    /* Create each of the worker jobs */
    printf("Creating worker jobs...\n");

    for (i=0; i < num; i++)
    {
        pid = fork();
        switch (pid) {
			case 0 : // enfant
				// listen_connection()
				exit(EXIT_SUCCESS);
			case -1 :
				perror("fork");
				return -1;
		}
        if (pid < 0)
        {
            perror("spawn() failed");
            close(listen_fd);
            exit(-1);
        }
        printf("  Worker = %d\n", pid);
    }

    /* Inform the user that the server is ready      */
    printf("The server is ready\n");

    /* Close down the listening socket               */
    close(listen_fd);

    return 0;
}