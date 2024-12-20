#include <SDL2/SDL_ttf.h>
#ifndef GLTEXTURE_LOADER_H
#define GLTEXTURE_LOADER_H

#endif
TextureMap load_texture_from_path(char * path, GLenum format, bool yReversed);
void draw_text(SDL_Surface *render_surface, int x, int y, char *text, TTF_Font *font, SDL_Color color, char *alignment, int width);