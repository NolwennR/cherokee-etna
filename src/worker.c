#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "worker.h"
#include "log.h"

void handle_connection(int id, int server_fd) {

    int             new_socket;
    long               valread;
    int                addrlen; 
    struct sockaddr_in address;
    char              *message;

    addrlen  = sizeof(address);
    message           = "Yo !";
    valread                = 0;

    while(1)
    {
        log_info("Worker %d wait for connection", id);

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            log_fatal("Worker %d in accept", id);
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        if (valread < 0){
            log_fatal("Worker %d read failed", id);
            perror("read() failed");
            close(new_socket);
            exit(-1);
        }
        log_info("Worker %d received %s", id, buffer);

        write(new_socket , message , strlen(message));
        log_info("Worker %d message sent %s", id, message);
        
        close(new_socket);
    }
}
