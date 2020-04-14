#ifndef CACHE_H
#define CACHE_H 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE_LENGTH

typedef struct lru_node
{
    struct lru_node *next;
    struct lru_node *previous;
    char *file_content;
    char *file_name;
    int content_length;
} lru_node_t;

typedef struct lru_list
{
    lru_node_t *front;
    lru_node_t *rear;
    unsigned int capacity;
    unsigned int count;
} lru_list_t;

typedef struct lru_hash
{
    unsigned int capacity;
    lru_node_t **nodes_array; /* contains references of all nodes (which are then double linked by the fifo) */
} lru_hash_t;

typedef struct lru_cache
{
    lru_hash_t *hash;
    lru_list_t *list;
} lru_cache_t;


lru_cache_t *create_cache(int capacity);
lru_hash_t *new_hash(int capacity);
lru_list_t *new_list(int capacity);
lru_node_t *create_node(char *content, const char *file_name, int *content_length);
void set_entry_to_top_node(lru_cache_t *cache, lru_node_t *node);
int cache_get(lru_cache_t *cache, const char *file_name, char **content);
void cache_put(lru_cache_t *cache, const char *file_name, char *content, int *content_length);
int cache_is_full(lru_cache_t *cache);
int cache_is_empty(lru_cache_t *cache);
void remove_last_entry(lru_list_t *list);
void free_cache();

#endif // !LRU_H