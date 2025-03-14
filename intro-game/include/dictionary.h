#pragma once 


#define TABLE_SIZE 50

typedef struct Entry_u {
    char * key ;
    void * value ;
    void (*destroy) (void *) ;
    struct Entry_u * next ;
} Entry_t ;

typedef struct Dictionary_u {
    Entry_t * tab[TABLE_SIZE] ;
} Dictionary_t ;


void destroy_dictionary(Dictionary_t ** dict) ;
Dictionary_t * create_dictionnary () ;
void dict_set(Dictionary_t *dict, const char * key, void * value, void (*destroy) (void *)) ;
void * dict_get(Dictionary_t *dict, const char *key) ;
void dict_remove(Dictionary_t * dict, const char * key) ;
unsigned int hash (const char * key) ;
void free_cb (void * data) ;