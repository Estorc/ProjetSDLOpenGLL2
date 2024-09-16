#ifndef SKYBOX_H
#define SKYBOX_H
//
#endif
TextureMap load_cubemap(char faces[6][100]);
void create_skybox(TexturedMesh *texturedMesh, char skyboxTexture[6][100]);