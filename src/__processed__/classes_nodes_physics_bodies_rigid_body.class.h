#ifndef __PROCESSED__CLASSES_NODES_PHYSICS_BODIES_RIGID_BODY_H
#define __PROCESSED__CLASSES_NODES_PHYSICS_BODIES_RIGID_BODY_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/storage/node.h"
#include "../src/io/scene_loader.h"
#include "../src/render/lighting.h"
#include "../src/buffer.h"
void __class_method_rigidbody_constructor(void * __retValueVP__, va_list args);
void __class_method_rigidbody_get_collisions_shapes(void * __retValueVP__, va_list args);
void __class_method_rigidbody_update(void * __retValueVP__, va_list args);
void __class_method_rigidbody_load(void * __retValueVP__, va_list args);
void __class_method_rigidbody_save(void * __retValueVP__, va_list args);
void __class_method_rigidbody_apply_impulse(void * __retValueVP__, va_list args);
void __class_method_rigidbody_get_velocity_norm(void * __retValueVP__, va_list args);
void __class_method_rigidbody_get_velocity(void * __retValueVP__, va_list args);
void __class_method_rigidbody_get_mass(void * __retValueVP__, va_list args);
void __class_method_rigidbody_get_center_of_mass(void * __retValueVP__, va_list args);
#endif
