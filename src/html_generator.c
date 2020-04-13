#include <html_generator.h>

static char* generateLink(char* path, struct dirent* dir) {
  size_t length;
  char *link = NULL;

  __uint8_t hasEndSlash = path[strlen(path) - 1] == '/';

  char* pattern = NULL;

  if(hasEndSlash == 0) {
    pattern = "<a href='%s/%s'>%s</a></br>";
  } else {
    pattern = "<a href='%s%s'>%s</a></br>";
  }

  length = snprintf(NULL, 0, pattern, path, dir->d_name, dir->d_name);

  link = malloc(length);
  
  sprintf(link, pattern, path, dir->d_name, dir->d_name);

  return link;
}

static char** generateLinks(dir_list_t* list, char* path, unsigned int* size) {
  dir_item_t* item = list->first;
  char** links = malloc((list->size + 1) * sizeof(char*));
  unsigned int i = 0;

  do {
    char* link = generateLink(path, item->dir);

    links[i] = link;

    (*size) += strlen(links[i++]);

    item = item->next;
  } while(item != NULL);

  links[list->size] = NULL;

  return links;
}


char* generateDirListing(char* path, dir_list_t* list){
  dir_item_t* item = list->first;
  size_t length = 0;

  if(item != NULL){
    char* html_header = "<html><head><title>My dir</title></head><body>";
    char* html_footer = "</body></html>";
    size_t header_length = strlen(html_header);
    size_t footer_length = strlen(html_footer);
    unsigned int linksSize = 0;
    unsigned int linkIndex = 0;
    char* link = NULL;
    char* body = NULL; 

    char** links = generateLinks(list, path, &linksSize);

    length = header_length;

    size_t body_length = length + footer_length + linksSize;
    body = malloc(body_length);
  
    strncpy(body, html_header, length);

    while((link = links[linkIndex++]) != NULL) {
      size_t name_length = strlen(link);

      strncpy(body + length, link, name_length);

      length += name_length;
    }

    strncpy(body + length, html_footer, footer_length);
    length += footer_length;

    body[length] = 0;
    
    return body;
  }

  return NULL;
}