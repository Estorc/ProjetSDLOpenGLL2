#include "../include/lib.h"
#include "../include/main.h"
#include "../include/dictionary.h"


Dictionary_t * create_dictionnary () {
    Dictionary_t * dic = malloc(sizeof(Dictionary_t)) ;
    if (dic == NULL) {
        printf("Erreur malloc dictionary\n");
        return NULL ;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        dic->tab[i] = NULL ;
    }

    return dic ;
}


void destroy_dictionary(Dictionary_t ** dict) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry_t *entry = (*dict)->tab[i];
        while (entry) {
            Entry_t *temp = entry;
            entry = entry->next;
            free(temp->key);
            temp->destroy(&temp->value);
            free(temp);
        }
    }
    free(*dict);
    *dict = NULL ;
}


void dict_set(Dictionary_t *dict, const char * key, void * value, void (*destroy) (void *)) {
    unsigned int index = hash(key);

    // Recherche si la clé existe déjà
    Entry_t *entry = dict->tab[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) { // Mise à jour si clé existe
            entry->destroy(&entry->value);
            entry->value = value;
            entry->destroy = destroy ;
            return;
        }
        entry = entry->next;
    }

    // Sinon, on crée une nouvelle entrée
    entry = malloc(sizeof(Entry_t));
    entry->key = strdup(key);
    entry->value = value;
    entry->destroy = destroy ;
    entry->next = dict->tab[index]; // Chaînage pour collision
    dict->tab[index] = entry;
}


void * dict_get(Dictionary_t *dict, const char *key) {
    unsigned int index = hash(key);
    Entry_t *entry = dict->tab[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL; // Clé non trouvée
}


void dict_remove(Dictionary_t * dict, const char * key) {
    unsigned int index = hash(key);
    Entry_t *entry = dict->tab[index];
    Entry_t *prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                dict->tab[index] = entry->next;
            }
            free(entry->key);
            entry->destroy(&entry->value);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}


unsigned int hash (const char * key) {

    unsigned int index = 0 ;

    for (int i = 0; key[i] != '\0'; i++) {
        index = (index * 31) + key[i] ;
    }

    return index % TABLE_SIZE ;
}


void free_cb (void * data) {
    void ** pdata = (void **)data ;
    free(*pdata);
    *pdata = NULL ;
}