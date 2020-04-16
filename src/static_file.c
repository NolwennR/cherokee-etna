#include "static_file.h"
#include "log/log.h"
#include "http.h"
#include "response.h"
#include "server.h"
#include "dir_list.h"
#include "html_generator.h"
#include "cache.h"

#define NB_HANDLERS 7

int (*handlers[NB_HANDLERS]) (const char *ext, response_t *response, const char *path, int size, lru_cache_t *cache) = { 
                                                        handle_text_file,
                                                        handle_css_file,
                                                        handle_json_file, 
                                                        handle_javascript_file, 
                                                        handle_png_file, 
                                                        handle_jpeg_file,
                                                        handle_mp4_file
                                                        };

void serve_static_file(request_t *request, connection_instance_t *connection, response_t* response, configuration_t* config)
{
    lru_cache_t *cache;
    log_trace("URL: %s", request->url);
    const char *dir = config->static_file_folder;
    char *path;
    
    cache = config->cache;

    path = malloc(sizeof(char) * (strlen(request->url) + strlen(dir)));
    if (!path)
    {
        log_error("malloc() path failed in static file response for worker %d", connection->worker_id);
        return;
    }

    strcpy(path, dir);
    strcat(path, request->url);

    // check_index_exists(&path);

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
        log_trace("list directory");
        list_dir(path, request->url, &(response->body));
        response->header.content_type = strdup("text/html");
    } 
    else if (S_ISREG(status.st_mode))
    {
        /* it's a regular file */
        log_info("it's a regular file");
        const char *extension = get_filename_ext(path);
      
        /* Checks for extension and handles it if needed  */
        for (int i = 0; i < NB_HANDLERS; i++)
        {
            if ((*handlers[i])(extension, response, path, status.st_size, cache) == 0)
                break;

            /* can't handle extension */
            if (i == NB_HANDLERS - 1)
            {
                free(path);
                not_implemented(response, connection);
                return;
            }
        }
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

int handle_text_file(const char* extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "txt") != 0 
        && strcmp(extension, "TXT") != 0
        && strcmp(extension, "html") != 0
        && strcmp(extension, "HTML") != 0)
        return -1;
    
    char *content_type = "text/html\0";
    read_text_file(path, &response->body, &size, cache);

    response->header.content_type = malloc(sizeof(char) * strlen(content_type));
    strcpy(response->header.content_type, content_type);
    response->header.content_length = size;

    return 0;
}

int handle_css_file(const char* extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "css") != 0 
        && strcmp(extension, "CSS") != 0)
        return -1;
    
    char *content_type = "text/css\0";
    read_text_file(path, &response->body, &size, cache);

    response->header.content_type = malloc(sizeof(char) * strlen(content_type));
    strcpy(response->header.content_type, content_type);
    response->header.content_length = size;

    return 0;
}

int handle_json_file(const char* extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "JSON") != 0 
        && strcmp(extension, "json") != 0)
        return -1;
    
    char *content_type = "application/json\0";
    response->header.content_type = malloc(sizeof(char) * strlen(content_type));
    strcpy(response->header.content_type, content_type);
    response->header.content_length = size;

    read_text_file(path, &response->body, &size, cache);

    return 0;
}

int handle_javascript_file(const char *extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "js") != 0 
        && strcmp(extension, "min.js") != 0)
        return -1;
    
    char *content_type = "application/javascript\0";
    response->header.content_type = strdup(content_type);
    response->header.content_length = size;

    read_image_file(path, &response->body, &size, cache);

    return 0;
}

int handle_png_file(const char *extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "png") != 0 
        && strcmp(extension, "PNG") != 0)
        return -1;
    
    char *content_type = "image/png\0";
    response->header.content_type = malloc(sizeof(char) * strlen(content_type));
    strcpy(response->header.content_type, content_type);
    response->header.content_length = size;

    read_image_file(path, &response->body, &size, cache);

    return 0;
}

int handle_jpeg_file(const char *extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "jpeg") != 0 
        && strcmp(extension, "JPEG") != 0
        && strcmp(extension, "jpg") != 0 )
        return -1;
    
    char *content_type = "image/jpeg\0";
    response->header.content_type = malloc(sizeof(char) * strlen(content_type));
    strcpy(response->header.content_type, content_type);
    response->header.content_length = size;

    read_image_file(path, &response->body, &size, cache);

    return 0;
}

int handle_mp4_file(const char *extension, response_t *response, const char *path, int size, lru_cache_t *cache)
{
    if (strcmp(extension, "mp4") != 0 )
        return -1;
    
    char *content_type = "video/mp4\0";
    response->header.content_type = strdup(content_type);
    response->header.content_length = size;

    read_image_file(path, &response->body, &size, cache);

    return 0;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) 
        return "";

    return dot + 1;
}

void read_text_file(const char *fileName, char **body, int *size, lru_cache_t *cache)
{
    int cache_result;
    if ((cache_result = cache_get(cache, fileName, body)) != -1){
        log_trace("hit cache");
        return;
    }

    FILE *file = fopen(fileName, "r");
    size_t n = 0;
    int c;

    if (file == NULL)
        return; /* could not open file */

    *body = malloc(sizeof(char) * (*size));

    while ((c = fgetc(file)) != EOF)
    {
        (*body)[n++] = (char) c;
    }

    fclose(file);    

    cache_put(cache, fileName, *body, size);
    //(*body)[n] = '\0';  
}

void read_image_file(const char *fileName, char **body, int *size, lru_cache_t *cache)
{
    int cache_result;
    if ((cache_result = cache_get(cache, fileName, body)) != -1){
        log_trace("hit cache");
        return;
    }

    FILE *file = fopen(fileName, "rb");

    if (file == NULL)
    {
        log_error("Couldn't open image: %s", fileName);
        return; 
    }

    *body = malloc(sizeof(char) * (*size));
    /* while */
    fread(*body, sizeof(char), *size, file);
    if (ferror(file) != 0) 
    {
        log_error("Error reading image: %s", fileName);
        return;
    } 

    fclose(file);

    cache_put(cache, fileName, *body, size);
}

int check_index_exists(char **path)
{
    int success;
    int path_length;
    char *index;
    char *index_path;

    success = -1;
    path_length = strlen(*path);
    index_path = malloc(sizeof(char) * (path_length + 12));

    if (index_path[path_length] == '/') {
        index = "index.html\0";
    }
    else {
        index = "/index.html\0";
    }

    strcpy(index_path, *path);
    strcat(index_path, index);

    struct stat status;
    stat(index_path, &status);

    if (S_ISREG(status.st_mode)) {
        success = 0;
    }
    if (success == 0) {
        free(*path);
        *path = index_path;
    } else
    {
        free(index_path);
    }

    return success;
}