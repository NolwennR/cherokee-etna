#include "static_file.h"
#include "log/log.h"
#include "http.h"

void serve_static_file(request_t *request, connection_instance_t *connection)
{
    log_trace("URL: %s", request->url);

    response_t *response = (response_t *)malloc(sizeof(request_t));

    struct stat status;
    int error = stat(request->url, &status);

    if(-1 == error) 
    {
        if(ENOENT == errno) 
        {
            /* doesn't exists */ 
            return not_found(response, connection);
        } 
        else 
        {
            log_error("Worker error on stat");
            return internal_server_error(response, connection);
        }
    } 
    else if(S_ISDIR(status.st_mode)) 
    {
        /* it's a dir */    
        list_diretory();
    } 
    else if (S_ISREG(status.st_mode))
    {
        /* it's a regular file */
        read_file();
    }
    else
    {
        /* exists but is no dir or file */
        return not_found(response, connection);
    }
    
    return ok(response, connection);
}

