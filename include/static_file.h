#ifndef STATIC_FILE_H
#define STATIC_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h> 
#include <stdlib.h>
#include <dirent.h> 
#include "server.h"

#include "http.h"

void serve_static_file(request_t *request, connection_instance_t *connection, configuration_t *config);
const char *get_filename_ext(const char *filename);
void read_text_file(const char *fileName, char **body, int size);
void read_image_file(const char *fileName, char **body, int size);
void list_dir(const char *path, char **body);

int handle_text_file(const char* extension, response_t *response, const char *path, int size);
int handle_json_file(const char* extension, response_t *response, const char *path, int size);
int handle_png_file(const char *extension, response_t *response, const char *path, int size);
int handle_jpeg_file(const char *extension, response_t *response, const char *path, int size);

#endif