#include <stdarg.h>
#include "../../classes/classes.h"
#include "../../types.h"
#include "../../math/math_util.h"
#include "../../io/model.h"
#include "../../render/framebuffer.h"
#include "../../physics/physics.h"
#include "../../storage/node.h"
#include "../../io/scene_loader.h"
#include "../../render/lighting.h"
#include "../../buffer.h"
#include "../../io/stringio.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_AREA;


void __class_method_area_constructor(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
struct Area * area = va_arg(args, struct Area *);
va_end(args);
(void)this;
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


void __class_method_area_collect_node(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
Node *  node = va_arg(args, Node * );
double  distance = va_arg(args, double );
va_end(args);
(void)this;
    Area *area = (Area *) this->object;
    area->collectedNodes = realloc(area->collectedNodes, sizeof(DistanceNode) * (area->collectedLength + 1));
    POINTER_CHECK(area->collectedNodes);
    area->collectedNodes[area->collectedLength].nodes = node;
    area->collectedNodes[area->collectedLength].distance = distance;
    area->collectedLength++;
}


void __class_method_area_is_area(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
bool * area = va_arg(args, bool *);
va_end(args);
(void)this;
    (*area) = true;
}


void __class_method_area_sort_nodes(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
va_end(args);
(void)this;
    Area *area = (Area *) this->object;

    int compare_distance_nodes(const void *a, const void *b) {
        DistanceNode *nodeA = (DistanceNode *)a;
        DistanceNode *nodeB = (DistanceNode *)b;
        if (nodeA->distance < nodeB->distance) return -1;
        if (nodeA->distance > nodeB->distance) return 1;
        return 0;
    }

    qsort(area->collectedNodes, area->collectedLength, sizeof(DistanceNode), compare_distance_nodes);
    memcpy(area->sortedNodes, area->collectedNodes, area->collectedLength * sizeof(DistanceNode));
    area->sortedLength = area->collectedLength;
}


void __class_method_area_update(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
float * pos = va_arg(args, float *);
float * rot = va_arg(args, float *);
float * scale = va_arg(args, float *);
va_end(args);
(void)this;
    Area *area = (Area *) this->object;

    METHOD(this, sort_nodes);
    #ifdef DEBUG
    #ifdef DEBUG_AREA
    if (area->sortedLength) {
        printf("Collected %d nodes:\n", area->sortedLength);
    }
    for (int i = 0; i < area->sortedLength; i++) {
        printf("Node %d: %f\n", i, area->sortedNodes[i].distance);
    }
    #endif
    #endif

    area->collectedLength = 0;
    area->collectedNodes = realloc(area->collectedNodes, buffers.collisionBuffer.length * sizeof(DistanceNode));

    update_global_position(this, pos, rot, scale);

    for (int i = 0; i < area->length; i++) {
        update_global_position(area->collisionsShapes[i], pos, rot, scale);
        glm_vec3_copy(this->globalPos, pos);
        glm_vec3_copy(this->globalRot, rot);
        glm_vec3_copy(this->globalScale, scale);
        check_collisions(area->collisionsShapes[i]);
    }
    memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], area->collisionsShapes, area->length * sizeof(area->collisionsShapes[0]));
    buffers.collisionBuffer.index += area->length;
}


void __class_method_area_get_collisions_shapes(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
Node *** shapes = va_arg(args, Node ***);
u8 * length = va_arg(args, u8 *);
va_end(args);
(void)this;
    Area *area = (Area *) this->object;
    *length = area->length;
    *shapes = area->collisionsShapes;
}


void __class_method_area_get_node_collection(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
DistanceNode ** nodes = va_arg(args, DistanceNode **);
u8 * length = va_arg(args, u8 *);
va_end(args);
(void)this;
    Area *area = (Area *) this->object;
    *length = area->collectedLength;
    *nodes = area->collectedNodes;
}


void __class_method_area_load(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
Camera ** c = va_arg(args, Camera **);
Script * scripts = va_arg(args, Script *);
Node * editor = va_arg(args, Node *);
va_end(args);
(void)this;
    Area *area;
    area = malloc(sizeof(Area));
    area->length = 0;
    int children_count = 0;
    POINTER_CHECK(area);
    if (file)
        fscanf(file,"(%d)\n", &children_count);
    METHOD_TYPE(this, __type__, constructor, area);

    area->collisionsShapes = malloc(sizeof(Node *) * children_count);
    buffers.collisionBuffer.length += children_count;
    POINTER_CHECK(area->collisionsShapes);
    
    for (int i = 0; i < children_count; i++) {
        Node *child = load_node(file, c, scripts, editor);
        area->collisionsShapes[area->length++] = child;
        child->parent = this;
    }
}


void __class_method_area_save(unsigned type, ...) {
va_list args;
va_start(args, type);
Node * this = va_arg(args, Node *);
FILE * file = va_arg(args, FILE *);
va_end(args);
(void)this;
    fprintf(file, "%s", classManager.class_names[this->type]);
    Area *area = (Area*) this->object;
    u8 collisionsLength = area->length;
    fprintf(file, "(%d)", collisionsLength);
}


