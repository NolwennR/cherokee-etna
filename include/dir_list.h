#ifndef DIRLIST_H
#define DIRLIST_H

#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

typedef struct dir_item
{
  struct dir_item *next;
  struct dirent *dir;
} dir_item_t;

typedef struct dir_list
{
  struct dir_item *first;
  struct dir_item *last;
  unsigned int size;
} dir_list_t;

dir_list_t* create_list();
int add_dir(dir_list_t *list, struct dirent *dir);
void free_list(dir_list_t* list);

#endif