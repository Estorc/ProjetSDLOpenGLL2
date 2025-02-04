#line 1 "src/__processed__/classes_nodes_physics_area.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../physics/physics.h"
#include "../storage/node.h"
#include "../io/scene_loader.h"
#include "../render/lighting.h"
#include "../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_AREA;
#line 14 "src/classes/nodes/physics/area.class.c"
void __class_method_area_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct Area * area = va_arg(args, struct Area *);(void)this;
        this->object = area;
        this->type = __type__;
        SUPER(initialize_node);
        if (area) {
            area->collectedLength = 0;
            area->sortedLength = 0;
            area->collectedNodes = malloc(buffers.collisionBuffer.length * sizeof(DistanceNode));
            POINTER_CHECK(area->collectedNodes);
            area->sortedNodes = malloc(buffers.collisionBuffer.length * sizeof(DistanceNode));
            POINTER_CHECK(area->sortedNodes);
        }
}

#line 28 "src/classes/nodes/physics/area.class.c"
void __class_method_area_collect_node(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node *  node = va_arg(args, Node * );double  distance = va_arg(args, double );(void)this;
        Area *area = (Area *) this->object;
        area->collectedNodes = realloc(area->collectedNodes, sizeof(DistanceNode) * (area->collectedLength + 1));
        POINTER_CHECK(area->collectedNodes);
        area->collectedNodes[area->collectedLength].node = node;
        area->collectedNodes[area->collectedLength].distance = distance;
        area->collectedLength++;
}

#line 37 "src/classes/nodes/physics/area.class.c"
void __class_method_area_is_area(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * area = va_arg(args, bool *);(void)this;
        (*area) = true;
}

    static int compare_distance_nodes(const void *a, const void *b) {
        DistanceNode *nodeA = (DistanceNode *)a;
        DistanceNode *nodeB = (DistanceNode *)b;
        if (nodeA->distance < nodeB->distance) return -1;
        if (nodeA->distance > nodeB->distance) return 1;
        return 0;
}

#line 49 "src/classes/nodes/physics/area.class.c"
void __class_method_area_sort_nodes(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Area *area = (Area *) this->object;

        qsort(area->collectedNodes, area->collectedLength, sizeof(DistanceNode), compare_distance_nodes);
        memcpy(area->sortedNodes, area->collectedNodes, area->collectedLength * sizeof(DistanceNode));
        area->sortedLength = area->collectedLength;
}

#line 57 "src/classes/nodes/physics/area.class.c"
void __class_method_area_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float * pos = va_arg(args, float *);float * rot = va_arg(args, float *);float * scale = va_arg(args, float *);(void)this;
        Area *area = (Area *) this->object;

        call_method_0(METHOD(sort_nodes,this));
        #ifdef DEBUG
        #ifdef DEBUG_AREA
        if (area->sortedLength) {
            PRINT_INFO("Collected %d nodes:\n", area->sortedLength);
        }
        for (int i = 0; i < area->sortedLength; i++) {
            PRINT_INFO("Node %d: %f\n", i, area->sortedNodes[i].distance);
        }
        #endif
        #endif

        area->collectedLength = 0;
        area->collectedNodes = realloc(area->collectedNodes, buffers.collisionBuffer.length * sizeof(DistanceNode));

        call_method_0(METHOD(update_global_position,this,pos, rot, scale));

        for (int i = 0; i < area->length; i++) {
            call_method_0(METHOD(update_global_position,(area->collisionsShapes[i]),pos, rot, scale));
            glm_vec3_copy(this->globalPos, pos);
            glm_vec3_copy(this->globalRot, rot);
            glm_vec3_copy(this->globalScale, scale);
            check_collisions(area->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], area->collisionsShapes, area->length * sizeof(area->collisionsShapes[0]));
        buffers.collisionBuffer.index += area->length;
}

#line 88 "src/classes/nodes/physics/area.class.c"
void __class_method_area_get_collisions_shapes(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node **** shapes = va_arg(args, Node ****);u8 ** length = va_arg(args, u8 **);(void)this;
        Area *area = (Area *) this->object;
        *length = &area->length;
        *shapes = &area->collisionsShapes;
}

#line 94 "src/classes/nodes/physics/area.class.c"
void __class_method_area_get_node_collection(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);DistanceNode ** nodes = va_arg(args, DistanceNode **);u8 * length = va_arg(args, u8 *);(void)this;
        Area *area = (Area *) this->object;
        *length = area->collectedLength;
        *nodes = area->collectedNodes;
}

#line 100 "src/classes/nodes/physics/area.class.c"
void __class_method_area_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Camera ** c = va_arg(args, Camera **);Script * scripts = va_arg(args, Script *);Node * editor = va_arg(args, Node *);(void)this;
        Area *area;
        area = malloc(sizeof(Area));
        area->length = 0;
        int children_count = 0;
        POINTER_CHECK(area);
        if (file)
            fscanf(file,"(%d)\n", &children_count);
        this->type = __type__;
        call_method_0(METHOD(constructor,this,area));

        area->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(area->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            area->collisionsShapes[area->length++] = child;
            child->parent = this;
        }
}

#line 122 "src/classes/nodes/physics/area.class.c"
void __class_method_area_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
        Area *area = (Area*) this->object;
        u8 collisionsLength = area->length;
        fprintf(file, "(%d)", collisionsLength);
}

