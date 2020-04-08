#include "dir_list.h"

static dir_item_t* create_item(struct dirent* dir) {
  dir_item_t* item = malloc(sizeof(dir_item_t));

  if(item != NULL){
    item->dir = dir;
    item->next = NULL;
  }

  return item;
}

dir_list_t* create_list(){
  dir_list_t* list = malloc(sizeof(dir_list_t));

  if(list != NULL) {
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
  }

  return list;
}

int add_dir(dir_list_t* list, struct dirent* dir) {
  dir_item_t* dir_item = create_item(dir);

  if(dir_item != NULL){
    if(list->first == NULL && list->last == NULL){
      list->first = dir_item;
      list->last = dir_item;
    } else {
      list->last->next = dir_item;
      list->last = dir_item;
    }

    list->size++;

    return 0;
  }

  return -1;
}

void free_list(dir_list_t* list){
  if(list != NULL){
    if(list->first != NULL){
      dir_item_t *i = list->first;

      while(i != NULL){
        dir_item_t *n = i;
        i = i->next;
        free(n);
      }
    }
    free(list);
  }
}