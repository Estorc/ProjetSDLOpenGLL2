#ifndef __PROCESSED__CLASSES_NODES_PHYSICS_BODIES_BODY_H
#define __PROCESSED__CLASSES_NODES_PHYSICS_BODIES_BODY_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/storage/node.h"
#include "../src/io/scene_loader.h"
#include "../src/render/lighting.h"
#include "../src/buffer.h"
void __class_method_body_is_body(void * __retValueVP__, va_list args);
void __class_method_body_apply_impulse(void * __retValueVP__, va_list args);
void __class_method_body_add_shape(void * __retValueVP__, va_list args);
void __class_method_body_add_shape_and_realloc(void * __retValueVP__, va_list args);
void __class_method_body_remove_shape(void * __retValueVP__, va_list args);
void __class_method_body_remove_shape_and_realloc(void * __retValueVP__, va_list args);
void __class_method_body_remove_shape_and_free(void * __retValueVP__, va_list args);
void __class_method_body_remove_shape_and_free_and_realloc(void * __retValueVP__, va_list args);
#endif
