#include "../include/lib.h"
#include "../include/main.h"
#include "../include/texture_loader.h"
#include "../include/dictionary.h"
#include "../include/list.h"

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

    texture->numFrames = 3 ;
    texture->currentFrame = 0 ;
    texture->frameCount = 0 ;
    texture->animationSpeed = 3 ;
    texture->loop = FALSE ;
    texture->playing = TRUE ;

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


void texture_update (Texture_t * texture) {

    if (existe(texture)) {

        // Incrémentation du compteur de frames
        texture->frameCount++; 
        if (texture->frameCount >= texture->animationSpeed) {
            texture->frameCount = 0 ; // Réinitialisation du compteur

            // Si la texture est active, on passe au frame suivant
            if (texture->playing == TRUE) {
                texture->currentFrame++;
            }
        }
        
        // Vérifie si la texture a dépassé le nombre total de frames
        if (texture->currentFrame >= texture->numFrames) {
            
            // Si la texture est en boucle, on la remet à zéro
            if (texture->loop == TRUE) {
                texture->currentFrame = 0;
            }
            else {
                texture->currentFrame = texture->numFrames ;
                texture->playing = FALSE; // Sinon, on arrête la texture
            }
        }
        //printf("nouvelle current frame %d \n", texture->currentFrame);
    }
    else {
        printf("Impossible d'update la texture car texture NULL\n");
    }
}


void texture_list_update (List_t * list) {

    if (existe(list)) {

        for (int i = 0; i < list->size; i++) {

            Texture_t * texture = list->item(list, i) ;
            texture_update(texture);
        }
    }
    else {
        printf("Impossible d'update le texture_dict car dict NULL\n");
    }
}


void texture_list_update_from_file (List_t * list, const char * dataPath) {

    if (!existe(list)) {
        printf("Impossible d'update le listTexture car list NULL\n");
        return ;
    }
    
    if (fileModified(dataPath)) {

        FILE * file = fopen(dataPath, "r") ;
        if (!existe(file)) {
            fprintf(stderr, "Erreur ouverture du fichier %s : %s\n", dataPath, SDL_GetError());
            return ;
        }

        // Variables pour stocker les données lues
        int x, y, w, h;
        int srcX, srcY, srcW, srcH ;
        int hidden, numFrames, speed, loop, typeAnim ;
        char key[64], texturePath[128] ; 
        char buffer[512] ;

        // Lecture de la première ligne
        fgets(buffer, sizeof(buffer), file);

        for (int i = 0; i < list->size; i++) {

            Texture_t * texture = list->item(list, i) ;

            // Lecture et initialisation des éléments
            if (fscanf(file, "%[^;];%[^;];%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d%%", 
                key, texturePath, &x, &y, &w, &h, &srcX, &srcY, &srcW, &srcH, &hidden, &numFrames, &speed, &loop, &typeAnim) == 15) {

                texture->typeAnim = (TypeTextureAnim_t)typeAnim ;

                texture->numFrames = numFrames ;
                texture->animationSpeed = speed ;
                texture->loop = loop ; 

                texture->position.x = x ;
                texture->position.y = y ;
                texture->position.w = w ;
                texture->position.h = h ;

                texture->srcrect.x = srcX ;
                texture->srcrect.y = srcY ;
                texture->srcrect.w = srcW ;
                texture->srcrect.h = srcH ;

                texture->hidden = hidden ;
            }
            
            texture_update(texture);
        }

        fclose(file);
    }
    else {
        for (int i = 0; i < list->size; i++) {
            texture_update(list->item(list, i));
        }
    }
}

/**
 * Creer une liste de texture a partir d'un fichier et les stock dans une liste.
 */
int load_textures_from_file (List_t * list, const char * dataPath) {

    FILE * file = fopen(dataPath, "r") ;
    if (!existe(file)) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", dataPath) ;
        return ERROR;
    }

    // Variables pour stocker les données lues
    int x, y, w, h;
    int srcX, srcY, srcW, srcH ;
    int hidden, numFrames, speed, loop, typeAnim ;
    char key[64], texturePath[128] ; 
    char buffer[512] ;

    // Lecture de la première ligne
    fgets(buffer, sizeof(buffer), file);

    printf("Lecture fichier : %s\n", dataPath) ;

    // Lecture et initialisation des éléments
    while (fscanf(file, "%[^;];%[^;];%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d%%", 
        key, texturePath, &x, &y, &w, &h, &srcX, &srcY, &srcW, &srcH, &hidden, &numFrames, &speed, &loop, &typeAnim) == 15) {

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

        texture->hidden = hidden ;
        texture->typeAnim = (TypeTextureAnim_t)typeAnim ;
        texture->numFrames = numFrames ;
        texture->animationSpeed = speed ;
        texture->loop = loop ;

        list->set(list, texture, list->size);

        printf("key : %s, texturePath : %s\n", key, texturePath) ;    
    }

    fclose(file);

    return NO_ERR ;
}