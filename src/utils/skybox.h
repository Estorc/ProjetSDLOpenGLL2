#ifndef SKYBOX_H
#define SKYBOX_H
//
#endif

TextureMap load_cubemap(char faces[6][200]);
void create_skybox(TextureMap *texturedMesh, char skyboxTexture[6][200]);