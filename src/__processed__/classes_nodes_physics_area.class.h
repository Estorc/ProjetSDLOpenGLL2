#ifndef __PROCESSED__CLASSES_NODES_PHYSICS_AREA_H
#define __PROCESSED__CLASSES_NODES_PHYSICS_AREA_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/physics/physics.h"
#include "../src/storage/node.h"
#include "../src/io/scene_loader.h"
#include "../src/render/lighting.h"
#include "../src/buffer.h"
void __class_method_area_constructor(void * __retValueVP__, va_list args);
void __class_method_area_collect_node(void * __retValueVP__, va_list args);
void __class_method_area_is_area(void * __retValueVP__, va_list args);
void __class_method_area_sort_nodes(void * __retValueVP__, va_list args);
void __class_method_area_update(void * __retValueVP__, va_list args);
void __class_method_area_get_collisions_shapes(void * __retValueVP__, va_list args);
void __class_method_area_get_node_collection(void * __retValueVP__, va_list args);
void __class_method_area_load(void * __retValueVP__, va_list args);
void __class_method_area_save(void * __retValueVP__, va_list args);
#endif
