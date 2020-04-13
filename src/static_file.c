#include "static_file.h"
#include "log/log.h"
#include "http.h"
#include "response.h"
#include "server.h"
#include "dir_list.h"
#include "html_generator.h"

#define NB_HANDLERS 4

int (*handlers[NB_HANDLERS]) (const char *ext, response_t *response, const char *path, int size) = { 
                                                        handle_text_file, 
                                                        handle_json_file, 
                                                        handle_png_file, 
                                                        handle_jpeg_file 
                                                        };

void serve_static_file(request_t *request, connection_instance_t *connection, configuration_t *config)
{
    log_trace("URL: %s", request->url);
    char *path;
    char dir[strlen(config->static_file_folder)];// = config->static_file_folder;//"/home/nolwenn/Documents/etna/master2/idv/aql5/group-763730/files\0";
    strcpy(dir, config->static_file_folder);

    response_t *response = malloc(sizeof(response_t));
    init_header(&(response->header));
    response->body = NULL;

    if (!response)
    {
        log_error("malloc() response_t failed in static file response for worker %d", connection->worker_id);
        return;
    }

    path = malloc(strlen(request->url) + strlen(dir));
    if (!path)
    {
        log_error("malloc() path failed in static file response for worker %d", connection->worker_id);
        return;
    }

    strcpy(path, dir);
    strcat(path, request->url);

    struct stat status;
    int error = stat(path, &status);

    if (-1 == error) 
    {
        if(ENOENT == errno) 
        {
            /* doesn't exists */ 
            not_found(response, connection);
            free(path);
            return;
        } 
        else 
        {
            log_error("Worker error on stat");
            internal_server_error(response, connection);
            free(path);
            return;
        }
    } 
    else if (S_ISDIR(status.st_mode)) 
    {
        /* it's a dir */    
        log_info("it's a dir");
        list_dir(path, request->url, &response->body);
        char *content_type = "text/html";
        response->header.content_type = malloc(strlen(content_type));
        strcpy(response->header.content_type, content_type);
    } 
    else if (S_ISREG(status.st_mode))
    {
        /* it's a regular file */
        log_info("it's a regular file");
        const char *extension = get_filename_ext(path);
      
        /* Checks for extension and handles it if needed  */
        for (int i = 0; i < NB_HANDLERS; i++)
        {
            if ((*handlers[i])(extension, response, path, status.st_size) == 0)
                break;

            /* can't handle extension */
            if (i == NB_HANDLERS - 1)
            {
                free(path);
                not_implemented(response, connection);
                return;
            }
        }
        response->header.content_length = status.st_size;
    }
    else
    {
        /* exists but is no dir nor file */
        not_found(response, connection);
        free(path);
        return;
    }

    ok(response, connection);

    free(path);
    return;
}

void list_dir(const char *path, char* url, char **body)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    if (d) 
    {      
        dir_list_t* list = create_list();

        while ((dir = readdir(d)) != NULL) 
        {
            if(add_dir(list, dir) == -1) perror(""); 
        }

        *body = generateDirListing(url, list);
        closedir(d);
    }
}

int handle_text_file(const char* extension, response_t *response, const char *path, int size)
{
    if (strcmp(extension, "txt") != 0 
        && strcmp(extension, "TXT") != 0
        && strcmp(extension, "html") != 0
        && strcmp(extension, "HTML") != 0)
        return -1;
    
    char *content_type = "text/html\0";
    response->header.content_type = malloc(strlen(content_type));
    strcpy(response->header.content_type, content_type);

    read_text_file(path, &response->body, size);

    return 0;
}

int handle_json_file(const char* extension, response_t *response, const char *path, int size)
{
    if (strcmp(extension, "JSON") != 0 
        && strcmp(extension, "json") != 0)
        return -1;
    
    char *content_type = "application/json\0";
    response->header.content_type = malloc(strlen(content_type));
    strcpy(response->header.content_type, content_type);

    read_text_file(path, &response->body, size);

    return 0;
}

int handle_png_file(const char *extension, response_t *response, const char *path, int size)
{
    if (strcmp(extension, "png") != 0 
        && strcmp(extension, "PNG") != 0)
        return -1;
    
    char *content_type = "image/png\0";
    response->header.content_type = malloc(strlen(content_type));
    strcpy(response->header.content_type, content_type);

    read_image_file(path, &response->body, size);

    return 0;
}

int handle_jpeg_file(const char *extension, response_t *response, const char *path, int size)
{
    if (strcmp(extension, "jpeg") != 0 
        && strcmp(extension, "JPEG") != 0
        && strcmp(extension, "jpg") != 0 )
        return -1;
    
    char *content_type = "image/jpeg\0";
    response->header.content_type = malloc(strlen(content_type));
    strcpy(response->header.content_type, content_type);

    read_image_file(path, &response->body, size);

    return 0;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) 
        return "";

    return dot + 1;
}

void read_text_file(const char *fileName, char **body, int size)
{
    FILE *file = fopen(fileName, "r");
    size_t n = 0;
    int c;

    if (file == NULL)
        return; /* could not open file */

    *body = malloc(size + 1);

    while ((c = fgetc(file)) != EOF)
    {
        (*body)[n++] = (char) c;
    }

    (*body)[n] = '\0';  
    fclose(file);    
}

void read_image_file(const char *fileName, char **body, int size)
{
    FILE *file = fopen(fileName, "rb");

    if (file == NULL)
    {
        log_error("Couldn't open image: %s", fileName);
        return; 
    }

    *body = malloc(size + 1);
    /* while */
    fread(*body, sizeof(char), size, file);
    if (ferror(file) != 0) 
    {
        log_error("Error reading image: %s", fileName);
        return;
    } 
    else 
    {
        (*body)[size++] = '\0';  
    }

    fclose(file);
}

