#ifndef __PROCESSED__CLASSES_NODES_SKYBOX_H
#define __PROCESSED__CLASSES_NODES_SKYBOX_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/storage/node.h"
#include "../src/memory.h"
#include "../src/render/render.h"
TextureMap call_method_4(void (*func)(void *, va_list), ...);
void __class_method_skybox_constructor(void * __retValueVP__, va_list args);
void __class_method_skybox_load_cubemap(void * __retValueVP__, va_list args);
void __class_method_skybox_create_skybox(void * __retValueVP__, va_list args);
void __class_method_skybox_load(void * __retValueVP__, va_list args);
void __class_method_skybox_save(void * __retValueVP__, va_list args);
void __class_method_skybox_render(void * __retValueVP__, va_list args);
#endif
