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

#include <raptiquax.h>
#include <classes/classes.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <physics/physics.h>
#include <storage/node.h>
#include <io/scene_loader.h>
#include <render/lighting.h>
#include <buffer.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class Area : public PhysicalNode {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the physics area class.
     *
     * This function initializes a new instance of the physics area class with the given signal ID.
     *
     * @param signal_id The signal ID to initialize the physics area instance with.
     */
    void constructor(int signal_id) {
        this->type = __type__;

        Area *area;
        area = malloc(sizeof(Area));
        POINTER_CHECK(area);
        area->length = 0;
        area->collectedLength = 0;
        area->collectedNodes = malloc(Game.buffers->collisionBuffer.length * sizeof(CollectedNode));
        POINTER_CHECK(area->collectedNodes);
        area->signal_id = signal_id;

        this->object = area;
        SUPER(initialize_node);
    }

    /**
     * @brief Loads data from a file and initializes the given camera, scripts, and editor node.
     *
     * This function reads data from the specified file and uses it to initialize the provided camera,
     * scripts, and editor node. The exact format and content of the file are assumed to be known by the
     * caller.
     *
     * @param file A pointer to the FILE object from which data will be read.
     * @param c A double pointer to a Camera object that will be initialized with data from the file.
     * @param scripts A pointer to a Script object that will be initialized with data from the file.
     * @param editor A pointer to a Node object representing the editor that will be initialized with data from the file.
     */
    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        int signal_id = 0;
        int children_count = 0;
        if (file)
            fscanf(file,"(%d,%d)\n", &signal_id, &children_count);
        this::constructor(signal_id);

        Area *area = this->object;

        area->collisionsShapes = malloc(sizeof(Node *) * children_count);
        Game.buffers->collisionBuffer.length += children_count;
        POINTER_CHECK(area->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, scripts, editor);
            area->collisionsShapes[area->length++] = child;
            child->parent = this;
        }
    }

    /**
     * @brief Collects a node based on the given distance and calculates the impact point.
     *
     * This function processes a given node and calculates the impact point based on the specified distance.
     *
     * @param node A pointer to the Node structure that needs to be processed.
     * @param distance A double value representing the distance used in the calculation.
     * @param impactPoint Vec3 where the calculated impact point will be stored.
     */
    void collect_node(Node * node, float distance, vec3 impactPoint) {
        Area *area = (Area *) this->object;
        area->collectedNodes = realloc(area->collectedNodes, sizeof(CollectedNode) * (area->collectedLength + 1));
        POINTER_CHECK(area->collectedNodes);
        area->collectedNodes[area->collectedLength].node = node;
        area->collectedNodes[area->collectedLength].distance = distance;
        glm_vec3_copy(impactPoint, area->collectedNodes[area->collectedLength].impactPoint);
        area->collectedLength++;
    }

    /**
     * @brief Checks if the given area is valid.
     *
     * This function takes a pointer to a boolean variable and checks if the area is valid.
     *
     * @param area A pointer to a boolean variable that indicates the validity of the area.
     */
    void is_area(bool *area) {
        (*area) = true;
    }

    /**
     * @brief Compares the distances of two nodes.
     *
     * This function is used to compare the distances of two nodes. It is typically used
     * in sorting algorithms where nodes need to be ordered based on their distances.
     *
     * @param a Pointer to the first node to compare.
     * @param b Pointer to the second node to compare.
     * @return An integer less than, equal to, or greater than zero if the distance of the first node
     *         is considered to be respectively less than, equal to, or greater than the distance of the second node.
     */
    static int compare_distance_nodes(const void *a, const void *b) {
        CollectedNode *nodeA = (CollectedNode *)a;
        CollectedNode *nodeB = (CollectedNode *)b;
        if (nodeA->distance < nodeB->distance) return -1;
        if (nodeA->distance > nodeB->distance) return 1;
        return 0;
    }

    /**
     * @brief Sorts the nodes in the physics area.
     *
     * This function sorts the nodes in the physics area based on a specific criterion.
     * The sorting algorithm and criterion are not specified in the provided code snippet.
     *
     * @note Ensure that the nodes are properly initialized before calling this function.
     */
    void sort_nodes() {
        Area *area = (Area *) this->object;

        qsort(area->collectedNodes, area->collectedLength, sizeof(CollectedNode), compare_distance_nodes);
    }

    //#define DEBUG_AREA

    /**
     * @brief Updates the position, rotation, and scale of an object.
     *
     * This function takes pointers to the position, rotation, and scale of an object and updates them accordingly.
     *
     * @param pos Vec3 position of the object. It is expected to be a float array of size 3 representing x, y, and z coordinates.
     * @param rot Vec3 rotation of the object. It is expected to be a float array of size 3 representing rotation angles around x, y, and z axes.
     * @param scale Vec3 scale of the object. It is expected to be a float array of size 3 representing scaling factors along x, y, and z axes.
     */
    void update(vec3 pos, vec3 rot, vec3 scale) {
        Area *area = (Area *) this->object;

        this::sort_nodes();
        #ifdef DEBUG
        #ifdef DEBUG_AREA
        if (area->collectedLength) {
            PRINT_INFO("Collected %d nodes:\n", area->collectedLength);
        }
        #endif
        #endif
        for (int i = 0; i < area->collectedLength; i++) {
            (area->collectedNodes[i].node)::emit_signal(SIGNAL_AREA_COLLISION, area->signal_id, this, area->collectedNodes[i].distance, area->collectedNodes[i].impactPoint, i);
            #ifdef DEBUG
            #ifdef DEBUG_AREA
            PRINT_INFO("Node %d: Distance[%.2f] | ImpactPoint[%.2f,%.2f,%.2f]\n", i, area->collectedNodes[i].distance, area->collectedNodes[i].impactPoint[0], area->collectedNodes[i].impactPoint[1], area->collectedNodes[i].impactPoint[2]);
            #endif
            #endif
        }


        area->collectedLength = 0;
        free(area->collectedNodes);
        area->collectedNodes = NULL;

        this::update_global_position(pos, rot, scale);

        for (int i = 0; i < area->length; i++) {
            (area->collisionsShapes[i])::update_global_position(pos, rot, scale);
            glm_vec3_copy(this->globalPos, pos);
            glm_vec3_copy(this->globalRot, rot);
            glm_vec3_copy(this->globalScale, scale);
            check_collisions(area->collisionsShapes[i]);
        }
        memcpy(&Game.buffers->collisionBuffer.collisionsShapes[Game.buffers->collisionBuffer.index], area->collisionsShapes, area->length * sizeof(area->collisionsShapes[0]));
        Game.buffers->collisionBuffer.index += area->length;
    }

    /**
     * @brief Retrieves the collision shapes and their lengths.
     *
     * This function populates the provided pointers with the collision shapes and their corresponding lengths.
     *
     * @param shapes A pointer to a 4-dimensional array of Node pointers. This will be populated with the collision shapes.
     * @param length A pointer to an array of unsigned 8-bit integers. This will be populated with the lengths of the collision shapes.
     */
    void get_collisions_shapes(Node ****shapes, u8 **length) {
        Area *area = (Area *) this->object;
        *length = &area->length;
        *shapes = &area->collisionsShapes;
    }

    /**
     * @brief Retrieves a collection of nodes.
     *
     * This function populates the provided array with a collection of nodes and sets the length of the collection.
     *
     * @param[out] nodes A pointer to an array of CollectedNode pointers that will be populated with the nodes.
     * @param[out] length A pointer to an unsigned 8-bit integer that will be set to the length of the node collection.
     */
    void get_node_collection(CollectedNode **nodes, u8 *length) {
        Area *area = (Area *) this->object;
        *length = area->collectedLength;
        *nodes = area->collectedNodes;
    }

    /**
     * @brief Saves the current state of the physics area to a file.
     *
     * This function writes the current state of the physics area to the specified file.
     * The file should be opened in a mode that allows writing before calling this function.
     *
     * @param file A pointer to a FILE object that identifies the file to which the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        Area *area = (Area*) this->object;
        u8 collisionsLength = area->length;
        fprintf(file, "(%hd,%d)", area->signal_id, collisionsLength);
    }

}

