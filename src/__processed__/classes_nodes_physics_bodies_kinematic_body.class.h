#ifndef __PROCESSED__CLASSES_NODES_PHYSICS_BODIES_KINEMATIC_BODY_H
#define __PROCESSED__CLASSES_NODES_PHYSICS_BODIES_KINEMATIC_BODY_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/storage/node.h"
#include "../src/io/scene_loader.h"
#include "../src/render/lighting.h"
#include "../src/buffer.h"
float call_method_3(void (*func)(void *, va_list), ...);
void __class_method_kinematicbody_constructor(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_get_length(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_get_collisions_shapes(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_update(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_load(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_save(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_apply_impulse(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_get_velocity_norm(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_get_velocity(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_get_mass(void * __retValueVP__, va_list args);
void __class_method_kinematicbody_get_center_of_mass(void * __retValueVP__, va_list args);
#endif
