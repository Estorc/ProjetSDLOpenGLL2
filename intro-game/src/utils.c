#include "../include/lib.h"
#include "../include/main.h"
#include "../include/dictionary.h"

int existe (void * ptr) {
    if (ptr != NULL) 
        return TRUE ;
    else 
        return FALSE ;
}

int compare_SDL_Color (SDL_Color a, SDL_Color b) {

    int sommeA = a.r + a.g + a.b + a.a ;
    int sommeB = b.r + b.g + b.b + b.a ;

    return sommeA - sommeB ;
}

int compare_SDL_Rect (SDL_Rect a, SDL_Rect b) {

    int sommeA = a.x + a.y + a.w + a.h ;
    int sommeB = b.x + b.y + b.w + b.h ;

    return sommeA - sommeB ;
}


/**
 * Vérifie si le fichier a été modifié depuis la dernière date de modification.
 * Utilise un dictionnaire déclaré en static pour stocker les dernières dates de modification
 * des fichiers; passer un parametre NULL pour libérer la mémoire du dictionnaire.
 * 
 * @return Renvoi `1` si fichier modifié, `0` sinon.
 */
int fileModified (const char * path) {

    static Dictionary_t * dict = NULL;
    if (!existe(dict)) {
        dict = create_dictionnary() ;
    }
    if (path == NULL) {
        destroy_dictionary(&dict);
    }

    struct stat file_stat ;
    if (stat(path, &file_stat) != 0) {
        fprintf(stderr, "Erreur recup stat fichier %s : %s\n", path, SDL_GetError());
        return 0 ;
    }

    time_t * lastModifiedTime = dict->get(dict, path) ;
    if (lastModifiedTime == NULL) {
        lastModifiedTime = malloc(sizeof(time_t)) ;
        *lastModifiedTime = file_stat.st_mtime ;
    
        dict->set(dict, path, lastModifiedTime, free_cb);
    }

    
    if (file_stat.st_mtime != *lastModifiedTime) {
        *lastModifiedTime = file_stat.st_mtime;
        
        return 1;  // fichier modifié
    }

    return 0;  // pas de changement dans le fichier 
}


void free_cb (void * data) {
    void ** pdata = (void **)data ;
    free(*pdata);
    *pdata = NULL ;
}


void swap(void* a, void* b, size_t size) {
    unsigned char temp[size]; 
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}
