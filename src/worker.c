#include "worker.h"
#include "log.h"

int server_id;

void handle_connection(int id, int server_fd) {

    int                      i;
    int                 client;
    long               valread;
    int                addrlen; 
    struct sockaddr_in address;

    int                    ret;
    int       number_of_events;
    int               epoll_fd;
    struct epoll_event   event;
    struct epoll_event *events;

    char              *message;

    addrlen  = sizeof(address);
    message           = "Yo !";
    valread                = 0;

    server_id = id;

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) 
    {
        log_fatal("Error in epoll_create, close worker");
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    event.data.fd = server_fd;
    event.events = EPOLLIN | EPOLLET;

    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event); // add server socket to accept connection
    if (ret == -1) 
    {
        log_fatal("Error in epoll_ctl, close worker");
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    events = calloc(MAX_EVENTS, sizeof event);
    if (events == NULL)
    {
        log_fatal("calloc error, close worker %d", server_id);
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        log_trace("Worker %d wait for events", id);
        number_of_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (i = 0; i < number_of_events; ++i) 
        {

            if (events[i].data.fd == server_fd)
            {
                if ((client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    log_error("Worker %d in accept", id);
                    perror("In accept");
                    continue;
                }

                set_non_block(client);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) // Add fd to the interest list
                {
                    log_error("Worker %d in epoll_ctl", id);
                    perror("In epoll_ctl");
                }
            }
            else if (events[i].events & EPOLLIN) // socket is available for read operations
            {
                char buffer[30000] = {0};
                int client_send = events[i].data.fd;

                valread = read(client_send, buffer, 30000);
                if (valread < 0){
                    log_error("Worker %d read failed", id);
                    perror("read() failed");
                }

                log_trace("Worker %d received %s", id, buffer);

                write(client_send , message , strlen(message));
                log_trace("Worker %d message sent %s", id, message);

                if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_send, &event))
                {
                    log_error("Worker %d in epoll_ctl", id);
                    perror("In epoll_ctl");
                }

                close(client_send);
            }
        }
    }

    free(events);
}


int set_non_block(int fd) {
    int flags, ret;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        log_fatal("Worker %d fcntl failed", server_id);
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1) {
        log_fatal("Worker %d fcntl failed", server_id);
        perror("fcntl");
        return -1;
    }

    return 0;
}
