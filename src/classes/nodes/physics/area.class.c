/**
 * @file
 * @brief This file handles the physics area nodes in the project.
 *
 * This file contains the implementation of the physics area nodes, which are used to define and manage
 * physical areas within the application. These nodes are responsible for handling various physical
 * interactions and properties within their defined areas.
 * 
 * Detailed description:
 * The code in this file includes functions and data structures necessary for creating, updating, and
 * managing physics areas. It ensures that the physical properties and interactions within these areas
 * are accurately simulated and maintained throughout the application's lifecycle.
 * 
 * @date 2023-10-25
 * 
 * @author Loup Picault
 */

#include "raptiquax.h"
#include "classes/classes.h"
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
            area->collectedNodes = malloc(buffers.collisionBuffer.length * sizeof(CollectedNode));
            POINTER_CHECK(area->collectedNodes);
            area->sortedNodes = malloc(buffers.collisionBuffer.length * sizeof(CollectedNode));
            POINTER_CHECK(area->sortedNodes);
        }
    }

    void collect_node(Node * node, double distance, float *impactPoint) {
        Area *area = (Area *) this->object;
        area->collectedNodes = realloc(area->collectedNodes, sizeof(CollectedNode) * (area->collectedLength + 1));
        POINTER_CHECK(area->collectedNodes);
        area->collectedNodes[area->collectedLength].node = node;
        area->collectedNodes[area->collectedLength].distance = distance;
        glm_vec3_copy(impactPoint, area->collectedNodes[area->collectedLength].impactPoint);
        area->collectedLength++;
    }

    void is_area(bool *area) {
        (*area) = true;
    }

    static int compare_distance_nodes(const void *a, const void *b) {
        CollectedNode *nodeA = (CollectedNode *)a;
        CollectedNode *nodeB = (CollectedNode *)b;
        if (nodeA->distance < nodeB->distance) return -1;
        if (nodeA->distance > nodeB->distance) return 1;
        return 0;
    }

    void sort_nodes() {
        Area *area = (Area *) this->object;

        qsort(area->collectedNodes, area->collectedLength, sizeof(CollectedNode), compare_distance_nodes);
        memcpy(area->sortedNodes, area->collectedNodes, area->collectedLength * sizeof(CollectedNode));
        area->sortedLength = area->collectedLength;
    }

    //#define DEBUG_AREA

    void update(float *pos, float *rot, float *scale) {
        Area *area = (Area *) this->object;

        this::sort_nodes();
        #ifdef DEBUG
        #ifdef DEBUG_AREA
        if (area->sortedLength) {
            PRINT_INFO("Collected %d nodes:\n", area->sortedLength);
        }
        #endif
        #endif
        for (int i = 0; i < area->sortedLength; i++) {
            (area->sortedNodes[i].node)::emit_signal(SIGNAL_AREA_COLLISION, area->signal_id, this, area->sortedNodes[i].distance, area->sortedNodes[i].impactPoint);
            #ifdef DEBUG
            #ifdef DEBUG_AREA
            PRINT_INFO("Node %d: Distance[%.2f] | ImpactPoint[%.2f,%.2f,%.2f]\n", i, area->sortedNodes[i].distance, area->sortedNodes[i].impactPoint[0], area->sortedNodes[i].impactPoint[1], area->sortedNodes[i].impactPoint[2]);
            #endif
            #endif
        }


        area->collectedLength = 0;
        area->collectedNodes = realloc(area->collectedNodes, buffers.collisionBuffer.length * sizeof(CollectedNode));

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

    void get_node_collection(CollectedNode **nodes, u8 *length) {
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
            fscanf(file,"(%hd,%d)\n", &area->signal_id, &children_count);
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
        fprintf(file, "(%d,%d)", area->signal_id, collisionsLength);
    }

}