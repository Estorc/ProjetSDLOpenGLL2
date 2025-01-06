#ifndef __PROCESSED__CLASSES_NODES_LIGHTS_SPOT_LIGHT_H
#define __PROCESSED__CLASSES_NODES_LIGHTS_SPOT_LIGHT_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/storage/node.h"
#include "../src/render/lighting.h"
#include "../src/render/depth_map.h"
#include "../src/buffer.h"
void __class_method_spotlight_constructor(void * __retValueVP__, va_list args);
void __class_method_spotlight_update(void * __retValueVP__, va_list args);
void __class_method_spotlight_load(void * __retValueVP__, va_list args);
void __class_method_spotlight_save(void * __retValueVP__, va_list args);
void __class_method_spotlight_configure_lighting(void * __retValueVP__, va_list args);
#endif
