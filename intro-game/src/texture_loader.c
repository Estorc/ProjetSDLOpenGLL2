#include "../include/lib.h"
#include "../include/main.h"
#include "../include/texture_loader.h"
#include "../include/dictionary.h"

// load png file and return the texture 
SDL_Texture * load_png (char * path) {
    SDL_Surface * surface = IMG_Load (path); 
    if (surface == NULL) {
        printf("Erreur de chargement de l'image: %s\n", IMG_GetError());
        return NULL;
    }
    
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("Erreur creation texture : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(surface);

    return texture;
}

void SDL_DestroyTexture_cb (void * texture) {
    SDL_Texture ** ptexture = (SDL_Texture **)texture ;
    SDL_DestroyTexture(*ptexture);
    *ptexture = NULL ;
}


Texture_t * create_texture (SDL_Rect position, SDL_Rect srcrect, int hidden) {

    Texture_t * texture = malloc(sizeof(Texture_t)) ;
    if (!existe(texture)) {
        fprintf(stderr, "erreur malloc de texture : %s\n", SDL_GetError()) ;
        return NULL ;
    }

    texture->hidden = hidden ;
    texture->position = position ;
    texture->srcrect = srcrect ; 
    texture->texture = NULL ;

    return texture ;
}


void destroy_texture (Texture_t ** texture) {

    if (existe(texture) && existe(*texture)) {

        if (existe((*texture)->texture)) {

            SDL_DestroyTexture((*texture)->texture);
            (*texture)->texture = NULL ;
        }

        free(*texture);
        *texture = NULL ;
    }
}
void destroy_texture_cb (void * texture) {
    destroy_texture(texture);
}


int load_textures_from_file (Dictionary_t * dict, const char * dataPath) {

    FILE * file = fopen(dataPath, "r") ;
    if (!existe(file)) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", dataPath) ;
        return ERROR;
    }

    // Variables pour stocker les données lues
    int x, y, w, h;
    int srcX, srcY, srcW, srcH ;
    int hidden ;
    char key[64], texturePath[128] ; 
    char buffer[512] ;

    // Lecture de la première ligne
    fgets(buffer, sizeof(buffer), file);

    printf("Lecture fichier : %s\n", dataPath) ;

    // Lecture et initialisation des éléments
    while (fscanf(file, "%[^;];%[^;];%d;%d;%d;%d;%d;%d;%d;%d;%d%%", 
        key, texturePath, &x, &y, &w, &h, &srcX, &srcY, &srcW, &srcH, &hidden) == 11) {

        Texture_t * texture = create_texture ((SDL_Rect){x, y, w, h}, (SDL_Rect){srcX, srcY, srcW, srcH}, hidden) ;
        
        if (!existe(texture)) {
            fprintf(stderr, "Erreur creation de la texture dans \"%s\"\n", dataPath) ;
            fclose(file);
            return ERROR ;
        } 
        
        texture->texture = load_png(texturePath) ;
        if (!existe(texture->texture)) {
            fprintf(stderr, "Erreur chargement des textures dans %s\n", dataPath) ;
            return ERROR ;
        }

        dict->set(dict, key, texture, destroy_texture_cb);

        printf("key : %s, texturePath : %s\n", key, texturePath) ;    
    }

    fclose(file);

    return NO_ERR ;
}