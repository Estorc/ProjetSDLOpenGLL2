#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/desktop.h"
#include "../include/render.h"
#include "../include/text.h"


/**
 * @brief Crée un nouvel objet de type `Text_t` pour gérer l'affichage et l'animation d'un texte.
 * 
 * Cette fonction alloue dynamiquement une structure `Text_t`, copie le texte fourni,
 * initialise les paramètres d'affichage et configure l'animation associée.
 * 
 * @param string   Le texte à afficher.
 * @param font     La police de caractères utilisée.
 * @param color    La couleur du texte.
 * @param position La position du texte à l'écran.
 * 
 * @return Un pointeur vers la structure `Text_t` créée, ou NULL en cas d'échec d'allocation.
 */
Text_t * create_text (const char * string, TTF_Font * font, SDL_Color * color, SDL_Rect * position) {

    // Allocation dynamique de la structure `Text_t`
    Text_t * text = malloc(sizeof(Text_t));
    if (text == NULL) {
        fprintf(stderr, "Erreur malloc `Text_t` (id : %s) : %s\n", string, SDL_GetError());
        return NULL;
    }



    // Allocation dynamique pour stocker la chaîne de caractères
    int len = strlen(string);
    text->string = malloc(sizeof(char) * (len + 1));  // +1 pour le caractère nul de fin de chaîne
    if (text->string == NULL) {
        fprintf(stderr, "Erreur malloc `char * Text_t::string` (id : %s) : %s\n", string, SDL_GetError());
        free(text);  // Libération de `text` en cas d'échec
        return NULL;
    }

    // Copie du texte fourni dans la structure
    strcpy(text->string, string);

    // Calcul de la taille de la chaine 
    text->len = len;

    // Assignation de la police de caractères
    text->font = font;

    // Initialisation des paramètres de l'animation du texte
    text->animation.texture = NULL ;
    text->animation.textColor = *color;   // Assignation de la couleur du texte
    text->animation.position = *position; // Position définie par le paramètre
    text->animation.needChange = TRUE ;   // Texture pas à jour au départ

    // Initialisation des paramètres d'ombre (désactivée par défaut)
    text->animation.hollowTexture = NULL ;
    text->animation.hollow = FALSE;           // Pas d'ombre
    text->animation.hollowColor.r = 0;        // Couleur noire
    text->animation.hollowColor.g = 0;
    text->animation.hollowColor.b = 0;
    text->animation.hollowColor.a = 255;      // Opacité maximale

    // Configuration de l'animation du texte
    text->animation.numFrames = text->len; // Nombre total de frames basé sur le nombre de lettres
    text->animation.currentFrame = 0;        // Début de l'animation à la première frame
    text->animation.animationSpeed = TEXT_ANIM_SPEED; // Vitesse de l'animation définie par une constante
    text->animation.frameCount = 0;          // Compteur interne initialisé à 0
    text->animation.loop = FALSE;            // L'animation ne boucle pas
    text->animation.playing = TRUE;          // L'animation démarre automatiquement

    return text; // Retourne la structure `Text_t` initialisée 
}



/**
 * @brief Libère la mémoire allouée pour un objet `Text_t` et remet le pointeur à NULL.
 * 
 * Cette fonction prend un pointeur vers un pointeur de `Text_t` et libère 
 * toutes les ressources associées, y compris :
 * - La chaîne de caractères stockée dans `string`.
 * - Les textures `texture` et `hollowTexture` associées à l'animation du texte.
 * 
 * Elle empêche également les fuites de mémoire en s'assurant que les pointeurs sont mis à `NULL` après la libération.
 * 
 * @param text Pointeur vers un pointeur de `Text_t` à détruire.
 */
void destroy_text (Text_t ** text) {

    // Vérifie que le pointeur `text` et son contenu ne sont pas NULL avant de libérer la mémoire
    if (existe(text) && existe(*text)) {

        // Libère la mémoire allouée pour la chaîne de caractères si elle existe
        if (existe((*text)->string)) {
            free((*text)->string);
            (*text)->string = NULL; // Évite les accès mémoire invalides après libération
        }

        // Détruit la texture principale associée à l'animation du texte, si elle existe
        if (existe((*text)->animation.texture)) {
            SDL_DestroyTexture((*text)->animation.texture);
            (*text)->animation.texture = NULL; // Évite un pointeur suspendu
        }

        // Détruit la texture de l'ombre du texte (si présente), si elle existe
        if (existe((*text)->animation.hollowTexture)) {
            SDL_DestroyTexture((*text)->animation.hollowTexture);
            (*text)->animation.hollowTexture = NULL; // Évite un pointeur suspendu
        }

        // Libère la structure `Text_t`
        free(*text);
        *text = NULL; // Évite un pointeur suspendu en mettant `text` à NULL
    }
}



/**
 * 
 */
SDL_Texture * create_TTF_Texture (TTF_Font * font, const char * string, SDL_Color color) {

    if (strlen(string) == 0) 
        return NULL ;

    // Création d'une surface SDL avec le texte et sa couleur
    SDL_Surface * surface = TTF_RenderUTF8_Blended(font, string, color);
    if (surface == NULL) {
        fprintf(stderr, "Erreur malloc surface du `Text_t` %s : %s\n", string, SDL_GetError());
        return NULL ;
    }

    // Création d'une texture SDL à partir de la surface
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "Erreur creation texture from surface du `Text_t` %s : %s\n", string, SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL ;
    }
    SDL_FreeSurface(surface); // Libération de la surface car elle n'est plus nécessaire

    return texture ;
}



/**
 * @brief Met à jour l'animation et la texture d'un texte.
 *
 * Cette fonction met à jour les paramètres d'animation d'un texte (`Text_t`), 
 * en incrémentant le compteur de frames et en changeant l'image affichée si nécessaire.
 * Si l'animation est terminée, elle peut être arrêtée ou remise à zéro selon le mode de boucle.
 * En cas de mise à jour, la texture SDL est recréée avec la nouvelle apparence du texte.
 * 
 * @param text Pointeur vers la structure `Text_t` à mettre à jour.
 */
void text_update (Text_t * text) {

    // Vérifie si le text existe 
    if (text == NULL) {
        return ;
    }

    // Récupération de la structure d'animation associée au texte
    TextAnim_t * anim = &text->animation;

    // Incrémentation du compteur de frames
    anim->frameCount++; 
    if (anim->frameCount == anim->animationSpeed) {
        anim->frameCount = 0 ; // Réinitialisation du compteur

        // Si l'animation est active, on passe au frame suivant et on marque un besoin de mise à jour
        if (anim->playing == TRUE) {
            do {
                anim->currentFrame++;
            } while (!isalpha(text->string[anim->currentFrame])) ;
            anim->needChange = TRUE; // Indique qu'on doit redessiner la texture
        }
    }

    // Vérifie si l'animation a dépassé le nombre total de frames
    if (anim->currentFrame > anim->numFrames) {

        // Si l'animation est en boucle, on la remet à zéro
        if (anim->loop == TRUE) {
            anim->currentFrame = 0;
            anim->needChange = TRUE;
        }
        else {
            anim->playing = FALSE; // Sinon, on arrête l'animation
        }
    }

    // Si un changement est nécessaire, on met à jour le `srcrect`
    if (anim->needChange == TRUE) {

        char * buffer = malloc(sizeof(char) * (anim->currentFrame + 1)) ;
        strncpy(buffer, text->string, anim->currentFrame);
        buffer[anim->currentFrame] = '\0';

        if (existe(anim->texture)) {
            SDL_DestroyTexture(anim->texture);
            anim->texture = NULL ;
        }
        if (existe(anim->hollowTexture)) {
            SDL_DestroyTexture(anim->hollowTexture);
            anim->hollowTexture = NULL ;
        }

        int width, height ;
        TTF_SizeUTF8(text->font, buffer, &width, &height);

        anim->texture = create_TTF_Texture(text->font, buffer, anim->textColor) ;
        SDL_QueryTexture(anim->texture, NULL, NULL, &width, &height);
        
        anim->hollowTexture = create_TTF_Texture(text->font, buffer, anim->hollowColor) ;
        SDL_QueryTexture(anim->hollowTexture, NULL, NULL, &width, &height);

        anim->position.w = width ;
        anim->position.h = height ;

        free(buffer);
    }
}



/**
 * 
 */
void text_change_hollow (Text_t * text, int boolean, SDL_Color color, TypeHollow_t dir) {

    TextAnim_t * anim = &text->animation ;

    anim->hollow = boolean ;
    anim->hollowColor = color ;
    anim->hollowDir = dir ;
}
