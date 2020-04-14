#include "cache.h"
#include "log/log.h"

lru_cache_t *create_cache(int capacity)
{
    lru_cache_t *cache;

    cache = malloc(sizeof(lru_cache_t));
    if (!cache){
        log_error("Malloc error, couldn't create cache");
        return cache;
    }

    cache->hash = new_hash(capacity);
    cache->list = new_list(capacity);

    return cache;
}

lru_hash_t *new_hash(int capacity)
{
    unsigned int i;

    lru_hash_t* hash = malloc(sizeof(lru_hash_t)); 
    hash->capacity = capacity; 
    hash->nodes_array = malloc(hash->capacity * sizeof(lru_node_t*)); 
  
    for (i = 0; i < hash->capacity; ++i){
        hash->nodes_array[i] = NULL; 
    }
  
    return hash; 
}

lru_list_t *new_list(int capacity)
{
    lru_list_t *list = malloc(sizeof(lru_list_t)); 
  
    list->count = 0; 
    list->front = list->rear = NULL; 
    list->capacity = capacity; 
  
    return list; 
}

lru_node_t *create_node(char *content, const char *file_name, int *content_length)
{
    if (!content){
        return NULL;
    }
    
    lru_node_t* node = malloc(sizeof(lru_node_t));
    if (!node){
        log_error("Malloc failed when creating cache node");
        return NULL;
    }
    node->file_name = strdup(file_name);
    node->content_length = *content_length; 

    node->file_content = malloc(sizeof(char) * (*content_length));
    if (!node->file_content){
        log_error("Malloc failed when creating cache node");
        return NULL;
    }

    memcpy(node->file_content, content, *content_length);
    node->previous = node->next = NULL; 
  
    return node; 
}

/* needs content length as we can deal with images (bytes) and can't use strlen */
int cache_get(lru_cache_t *cache, const char *file_name, char **content) 
{
    int hit;
    
    hit = -1;
    if (cache_is_empty(cache))
        return hit;

    /* search file name among hash nodes */
    for (size_t i = 0; i < cache->hash->capacity; i++)
    {
        lru_node_t *node = (lru_node_t *)cache->hash->nodes_array[i];
        if (!node){
            continue;
        }
        if (strcmp(node->file_name, file_name) == 0){
            hit = 0;
            if (!(*content)){
                *content = malloc(sizeof(node->content_length));
            }
            if (!(*content)){
                log_error("Malloc failed, couldn't set content from cache request");
            }
            memcpy(*content, node->file_content, node->content_length);
            set_entry_to_top_node(cache, node);
            break;
        } 
    }
    return hit;
}

void set_entry_to_top_node(lru_cache_t *cache, lru_node_t *node)
{
    /* already in list head*/
    if (node == cache->list->front){
        return;
    }

    /* detache node */
    node->previous->next = node->next; 
    if (node->next){
        node->next->previous = node->previous; 
    }

    /* if node is in rear, rebuild new rear */
    if (node == cache->list->rear) { 
        cache->list->rear = node->previous; 
        cache->list->rear->next = NULL; 
    } 

    /* Put the requested page before current front */
    node->next = cache->list->front; 
    node->previous = NULL; 

    /* Change prev of current front  */
    node->next->previous = node; 

    /* Change front to the requested page */
    cache->list->front = node; 
}

void cache_put(lru_cache_t *cache, const char *file_name, char *content, int *content_length)
{
    if (cache_is_full(cache)) { 
        /* remove last item (rear) from hash and list */
        for (size_t i = 0; i < cache->hash->capacity; i++)
        {
            lru_node_t *node = cache->hash->nodes_array[i];
            if (node && strcmp(node->file_name, cache->list->rear->file_name) == 0){
                cache->hash->nodes_array[i] = NULL;
                remove_last_entry(cache->list);
            }
        }
    } 
  
    // Create a new node with given page number, 
    // And add the new node to the front of queue 
    lru_node_t* temp = create_node(content, file_name, content_length); 
    if (!temp){
        log_debug("Couldn't add new data to cache");
        return;
    }
    temp->next = cache->list->front; 
  
    /* If queue is empty, change both front and rear pointers  */
    if (cache_is_empty(cache))
    {
        cache->list->rear = cache->list->front = temp; 
    }
    else // Else change the front 
    { 
        cache->list->front->previous = temp; 
        cache->list->front = temp; 
    } 
  
    // Add page entry to hash also 
    cache->hash->nodes_array[cache->list->count] = temp; 
  
    // increment number of full frames 
    cache->list->count++; 
}

void remove_last_entry(lru_list_t *list)
{
    if (list->count == 0)
        return;

    if (list->front == list->rear) 
        list->front = NULL; 
  
    lru_node_t* temp = list->rear; 
    list->rear = list->rear->previous; 
  
    if (list->rear) 
        list->rear->next = NULL; 
  
    free(temp); 
    list->count--; 
}

int cache_is_full(lru_cache_t *cache)
{
    return cache->list->capacity == cache->list->count;
}

int cache_is_empty(lru_cache_t *cache)
{
    return cache->list->count == 0;
}