#pragma once 


#include "lib.h"


typedef struct ListNode_u {
    void * data ;
    struct ListNode_u * prev ;
    struct ListNode_u * next ;
} ListNode_t ;


/** 
 * Liste d'objets generique homogènes. Ne pas mettre differents type de donnees dans une meme liste
 */
typedef struct List_u {
    ListNode_t * head ;
    int size ;

    void * (*item) (struct List_u *, int) ;
    void (*remove) (struct List_u *, int) ;
    void (*set) (struct List_u *, void *, int) ;
    void (*stack) (struct List_u *, void *) ;
    void (*swap) (struct List_u *, int, int) ;
    void (*push) (struct List_u *, void *) ;
    void (*pop) (struct List_u *) ;
    void (*destroy) (void *) ;      // fonction de destruction de data 
} List_t ;


List_t * create_list (void (*destroy) (void *)) ;
void destroy_list (List_t ** list) ;
void destroy_list_cb (void * list) ;

// pour debugage
void list_print (List_t * list, void (*print) (void *)) ;
void list_print_value (List_t * list) ;