#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "physics/physics.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class Area : public PhysicalNode {
    __containerType__ Node *
    public:

    void constructor(struct Area *area) {
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

    void collect_node(Node * node, double distance) {
        Area *area = (Area *) this->object;
        area->collectedNodes = realloc(area->collectedNodes, sizeof(DistanceNode) * (area->collectedLength + 1));
        POINTER_CHECK(area->collectedNodes);
        area->collectedNodes[area->collectedLength].node = node;
        area->collectedNodes[area->collectedLength].distance = distance;
        area->collectedLength++;
    }

    void is_area(bool *area) {
        (*area) = true;
    }

    static int compare_distance_nodes(const void *a, const void *b) {
        DistanceNode *nodeA = (DistanceNode *)a;
        DistanceNode *nodeB = (DistanceNode *)b;
        if (nodeA->distance < nodeB->distance) return -1;
        if (nodeA->distance > nodeB->distance) return 1;
        return 0;
    }

    void sort_nodes() {
        Area *area = (Area *) this->object;

        qsort(area->collectedNodes, area->collectedLength, sizeof(DistanceNode), compare_distance_nodes);
        memcpy(area->sortedNodes, area->collectedNodes, area->collectedLength * sizeof(DistanceNode));
        area->sortedLength = area->collectedLength;
    }

    void update(float *pos, float *rot, float *scale) {
        Area *area = (Area *) this->object;

        this::sort_nodes();
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

        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < area->length; i++) {
            (area->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, pos);
            glm_vec3_copy(this->globalRot, rot);
            glm_vec3_copy(this->globalScale, scale);
            check_collisions(area->collisionsShapes[i]);
        }
        memcpy(&buffers.collisionBuffer.collisionsShapes[buffers.collisionBuffer.index], area->collisionsShapes, area->length * sizeof(area->collisionsShapes[0]));
        buffers.collisionBuffer.index += area->length;
    }

    void get_collisions_shapes(Node ****shapes, u8 **length) {
        Area *area = (Area *) this->object;
        *length = &area->length;
        *shapes = &area->collisionsShapes;
    }

    void get_node_collection(DistanceNode **nodes, u8 *length) {
        Area *area = (Area *) this->object;
        *length = area->collectedLength;
        *nodes = area->collectedNodes;
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        Area *area;
        area = malloc(sizeof(Area));
        area->length = 0;
        int children_count = 0;
        POINTER_CHECK(area);
        if (file)
            fscanf(file,"(%d)\n", &children_count);
        this->type = __type__;
        this::constructor(area);

        area->collisionsShapes = malloc(sizeof(Node *) * children_count);
        buffers.collisionBuffer.length += children_count;
        POINTER_CHECK(area->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            area->collisionsShapes[area->length++] = child;
            child->parent = this;
        }
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        Area *area = (Area*) this->object;
        u8 collisionsLength = area->length;
        fprintf(file, "(%d)", collisionsLength);
    }

}