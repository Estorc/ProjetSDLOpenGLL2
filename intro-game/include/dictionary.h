#pragma once 


#define TABLE_SIZE 50

typedef struct Entry_u {
    char * key ;
    void * value ;
    void (*destroy) (void *) ;
    struct Entry_u * next ;
} Entry_t ;

/**
 * dictionnaire generique et heterogenes. attention ne donne aucune 
 * info sur le type de la donnnee, une fois stockes il faut se rappeler du type de la donnee.
 */
typedef struct Dictionary_u {
    Entry_t * tab[TABLE_SIZE] ;
    int nbEntry ;

    Entry_t * (*item) (struct Dictionary_u *, int) ;
    void * (*get) (struct Dictionary_u *, const char *) ;
    void (*set) (struct Dictionary_u *, const char *, void *, void (*) (void *)) ;
    void (*remove) (struct Dictionary_u *, const char *) ;
} Dictionary_t ;


Dictionary_t * create_dictionnary () ;
void destroy_dictionary(Dictionary_t ** dict) ;
void destroy_dictionary_cb (void * dict) ;
unsigned int hash (const char * key) ;
