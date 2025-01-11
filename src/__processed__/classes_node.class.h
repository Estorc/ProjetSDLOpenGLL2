#ifndef __PROCESSED__CLASSES_NODE_H
#define __PROCESSED__CLASSES_NODE_H
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/render/framebuffer.h"
#include "../src/storage/node.h"
int call_method_1(void (*func)(void *, va_list), ...);
void __class_method_node_constructor(void * __retValueVP__, va_list args);
void __class_method_node_initialize_node(void * __retValueVP__, va_list args);
void __class_method_node_get_glow_shader(void * __retValueVP__, va_list args);
void __class_method_node_load(void * __retValueVP__, va_list args);
void __class_method_node_save(void * __retValueVP__, va_list args);
void __class_method_node_prepare_render(void * __retValueVP__, va_list args);
void __class_method_node_render(void * __retValueVP__, va_list args);
void __class_method_node_update_global_position(void * __retValueVP__, va_list args);
void __class_method_node_update(void * __retValueVP__, va_list args);
void __class_method_node_free(void * __retValueVP__, va_list args);
void __class_method_node_is_cshape(void * __retValueVP__, va_list args);
void __class_method_node_is_body(void * __retValueVP__, va_list args);
void __class_method_node_is_area(void * __retValueVP__, va_list args);
void __class_method_node_is_gui_element(void * __retValueVP__, va_list args);
void __class_method_node_add_child(void * __retValueVP__, va_list args);
void __class_method_node_add_child_and_realloc(void * __retValueVP__, va_list args);
void __class_method_node_remove_child(void * __retValueVP__, va_list args);
void __class_method_node_remove_child_and_realloc(void * __retValueVP__, va_list args);
void __class_method_node_remove_child_and_free(void * __retValueVP__, va_list args);
void __class_method_node_remove_child_and_free_and_realloc(void * __retValueVP__, va_list args);
void __class_method_node_index_of_child(void * __retValueVP__, va_list args);
void __class_method_node_print(void * __retValueVP__, va_list args);
void __class_method_node_emit_ready(void * __retValueVP__, va_list args);
void __class_method_node_emit_update(void * __retValueVP__, va_list args);
void __class_method_node_emit_signal(void * __retValueVP__, va_list args);
#endif
