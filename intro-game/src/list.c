#include "../include/lib.h"
#include "../include/main.h"
#include "../include/list.h"




static 
void list_push (List_t * list, void * data) {

    if (!existe(list)) {
        printf("Impossible de push dans list car list vide\n");
        return ;
    }

    if (!existe(data)) {
        printf("Impossible de push dans list car data vide\n");
        return ;
    }

    if (list->size == 0) {
        list->head = malloc(sizeof(ListNode_t)) ;
        
        list->head->data = data ;
        list->head->next = list->head ;
        list->head->prev = list->head ;
        
        list->size++;
    }
    else {
        ListNode_t * head = list->head ;
        ListNode_t * tail = list->head->prev ;

        ListNode_t * newNode = malloc(sizeof(ListNode_t)) ;
        
        newNode->data = data ;
        newNode->next = head ;
        newNode->prev = tail ;

        head->prev = newNode ;

        tail->next = newNode ;

        list->head = newNode ;

        list->size++;
    }
}


static 
void list_pop (List_t * list) {

    if (list->size == 0) {
        printf("Impossible de pop le dernier élément car la liste est vide\n");
        return ;
    }

    if (existe(list)) {

        if (list->size == 1) {

            list->destroy(&list->head->data);
            free(list->head);
            
            list->head = NULL ;

            list->size--;
        }
        else {

            ListNode_t * head = list->head ;
            ListNode_t * tail = head->prev ;
            ListNode_t * newTail = tail->prev ;

            head->prev = newTail ;
            newTail->next = head ;

            list->destroy(&tail->data);
            free(tail);

            list->size--;
        }   
    }
}


static 
void * list_item (List_t * list, int index) {

    if (!existe(list)) {
        printf("Impossible de recup l'élément a l'indice %d car list est NULL\n", index);
        return NULL ;
    }
    if (list->size == 0) {
        printf("Impossible de recup l'élément a l'indice %d car list est vide\n", index);
        return NULL ;
    }
    if ((index >= 0 ? index : -index) > (list->size - 1) || (index >= 0 ? index : -index) > list->size) {
        printf("Impossible de recup l'élément a l'indice %d, indice doit etre compris dans [%d, %d]\n", index, -list->size, list->size - 1);
        return NULL ;
    }

    if (index < 0) {
        index += list->size ;
    }

    ListNode_t * item ;
    if (index <= list->size / 2) {

        item = list->head ;
        while (index > 0) {
            item = item->next ;
            index--;
        }

        return item->data ;
    }
    else {

        // reprends l'indice de depart
        index -= list->size ;

        item = list->head ;
        while (index < 0) {
            item = item->prev ;
            index++;
        }

        return item->data ;
    }
}


static 
void list_remove (List_t * list, int index) {

    if (!existe(list)) {
        printf("Impossible de supprimer l'élément a l'indice %d car list est NULL\n", index);
        return ;
    }
    if (list->size == 0) {
        printf("Impossible de supprimer l'élément a l'indice %d car list est vide\n", index);
        return ;
    }
    if ((index >= 0 ? index : -index) > (list->size - 1) || (index >= 0 ? index : -index) > list->size) {
        printf("Impossible de supprimer l'élément a l'indice %d, indice doit etre compris dans [%d, %d]\n", index, -list->size, list->size - 1);
        return ;
    }

    if (list->size == 1) {

        list->destroy(&list->head->data);
        free(list->head);
        list->head = NULL ;

        list->size--;

        return ;
    }

    if (index == 0) {

        ListNode_t * head = list->head ;

        ListNode_t * prev = head->prev ;
        ListNode_t * next = head->next ;

        prev->next = next ;
        next->prev = prev ;

        list->destroy(&head->data);
        free(head);

        list->head = next ;

        list->size--;

        return ;
    } 

    if (index < 0) {
        index += list->size ;
    }

    ListNode_t * item ;
    if (index <= list->size / 2) {

        item = list->head ;
        while (index > 0) {
            item = item->next ;
            index--;
        }

        ListNode_t * prev = item->prev ;
        ListNode_t * next = item->next ;

        prev->next = next ;
        next->prev = prev ;

        list->destroy(&item->data);
        free(item);

        list->size--;

        return ;
    }
    else {

        // reprends l'indice de depart
        index -= list->size ;

        item = list->head ;
        while (index < 0) {
            item = item->prev ;
            index++;
        }

        ListNode_t * prev = item->prev ;
        ListNode_t * next = item->next ;

        prev->next = next ;
        next->prev = prev ;

        list->destroy(&item->data);
        free(item);

        list->size--;

        return ;
    }
}


/**
 * Ajoute en fin de liste sans supprimer d'élément si l'index >= list->size.
 * Sinon, supprime la data de l'élément actuellement présent a l'index et remplace sa data par la nouvelle. 
 */
static 
void list_set (List_t * list, void * data, int index) {

    if (!existe(list)) {
        printf("Impossible d'ajouter un element dans list car list NULL\n");
        return ;
    }

    if (!existe(data)) {
        printf("Impossible d'ajouter un element dans list car data NULL\n");
        return ;
    }

    if (index < 0) {
        int bits = sizeof(int) * 8 ; // nombre de bits dans un int
        int max_int = (1 << (bits - 1)) - 1 ; // 2 ^ (bits - 1) - 1
        printf("Impossible d'ajouter un element a l'index %d, index doit etre compris dans [%d, %d]\n", index, 0, max_int);
        return ;
    }

    if (list->size == 0) {
        list->head = malloc(sizeof(ListNode_t)) ;
        
        list->head->data = data ;
        list->head->next = list->head ;
        list->head->prev = list->head ;
        
        list->size++;

        return ;
    }

    if (index >= list->size) {

        ListNode_t * head = list->head ;
        ListNode_t * tail = head->prev ;

        ListNode_t * newNode = malloc(sizeof(ListNode_t)) ;
        newNode->data = data ;
        newNode->next = head ;
        newNode->prev = tail ;

        head->prev = newNode ;
        tail->next = newNode ;

        list->size++;

        return ;
    }
    else {

        ListNode_t * item ;
        if (index <= list->size / 2) {

            item = list->head ;
            while (index > 0) {
                item = item->next ;
                index--;
            }

            list->destroy(&item->data);
            item->data = data ;

            return ;
        }
        else {

            // recupere le complement de index pour aller a list->size
            index -= list->size ;

            item = list->head ;
            while (index < 0) {
                item = item->prev ;
                index++;
            }

            list->destroy(&item->data);
            item->data = data ;

            return ;
        }
    }
}


static 
void list_stack (List_t * list, void * data) {
    list_set(list, data, list->size);
}


static 
void list_swap (List_t * list, int indexA, int indexB) {

    if (!existe(list)) {
        printf("List n'existe pas, impossible de faire le swap\n");
        return ;
    }

    if (list->size < 2) {
        printf("impossible de faire le swap la liste ne contient pas assez d'élément\n");
        return ;
    }

    if (indexA > (list->size - 1) || indexA < 0 || indexB > (list->size - 1) || indexB < 0) {
        printf("impossible de faire le swap l'indice est incorrect\n");
        return ;
    }


    // Recup l'item indexA
    ListNode_t * itemA ;
    if (indexA <= list->size / 2) {

        itemA = list->head ;
        while (indexA > 0) {
            itemA = itemA->next ;
            indexA--;
        }
    }
    else {

        // reprends l'indice de depart
        indexA -= list->size ;

        itemA = list->head ;
        while (indexA < 0) {
            itemA = itemA->prev ;
            indexA++;
        }
    }

    // Recup l'item indexB
    ListNode_t * itemB ;
    if (indexB <= list->size / 2) {

        itemB = list->head ;
        while (indexB > 0) {
            itemB = itemB->next ;
            indexB--;
        }
    }
    else {

        // reprends l'indice de depart
        indexB -= list->size ;

        itemB = list->head ;
        while (indexB < 0) {
            itemB = itemB->prev ;
            indexB++;
        }
    }


    // Echange les données des deux item 
    void * temp = itemA->data ;
    itemA->data = itemB->data ;
    itemB->data = temp ;
}


List_t * create_list (void (*destroy) (void *)) {

    List_t * list = malloc(sizeof(List_t)) ;
    if (!existe(list)) {
        fprintf(stderr, "Erreur malloc de list\n");
        return NULL ;
    }

    list->size = 0 ;
    list->head = NULL ;
    list->item = list_item ;
    list->remove = list_remove ;
    list->set = list_set ;
    list->stack = list_stack ;
    list->swap = list_swap ;
    list->push = list_push ;
    list->pop = list_pop ;
    list->destroy = destroy ;

    return list ;
}


/**
 * Detruis la structure elle meme et tout ses éléments.
 */
void destroy_list (List_t ** list) {

    if (existe(list) && existe(*list)) {

        if ((*list)->size > 0 && existe((*list)->head) && existe((*list)->destroy)) {
            
            ListNode_t * currentNode = (*list)->head ;
            for (int i = 0; i < (*list)->size; i++) {
                ListNode_t * nextNode = currentNode->next;  // Sauvegarde du prochain élément
                (*list)->destroy(&currentNode->data);  // Libère les données stockées
                free(currentNode);  // Libère le nœud
                currentNode = nextNode;  // Passe au suivant
            }
        } 

        free(*list);
        *list = NULL ;
    }
}
void destroy_list_cb (void * list) {
    destroy_list(list);
}


void list_print (List_t * list, void (*print) (void *)) {

    if (!existe(list)) {
        printf("Ne peut pas afficher liste car liste NULL\n");
        return ;
    }

    if (list->size == 0) {
        printf("Ne peut pas afficher liste car liste vide\n");
        return ;
    }

    ListNode_t * current = list->head ;
    for (int i = 0; i < list->size; i++) {
        print(current->data) ;
        current = current->next ;
    }
    printf("\n");

    for (int i = 0; i < list->size; i++) {
        void * data = list->item(list, i) ;
        print(data) ;
    }
}


void list_print_value (List_t * list) {

    if (!existe(list)) {
        printf("Ne peut pas afficher liste car liste NULL\n");
        return ;
    }

    if (list == 0) {
        printf("Liste vide\n");
        return ;
    }

    if (existe(list)) {

        ListNode_t * current = list->head ;
        for (int i = 0; i < list->size; i++) {
            
            if (existe(current)) {

                printf("indice = %d\ncurrent = %p\nnext = %p, prev = %p\n\n", i, current, current->next, current->prev) ;
                if (existe(current->next)) {
                    current = current->next ;
                }
            }
        }
    }
    else {
        printf("la liste est NULL\n");
    }
}