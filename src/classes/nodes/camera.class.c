/**
 * @file
 * @brief This file contains the implementation of the Camera class.
 *
 * This source file provides the functionality for managing and manipulating
 * camera objects within the application. It includes functions for setting
 * camera parameters, updating the camera's position and orientation, and
 * handling camera-related transformations.
 *
 * The Camera class is essential for rendering scenes from different viewpoints
 * and is used extensively in the graphics rendering pipeline.
 *
 * @author Loup Picault
 * @date 2023-10-31
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

/**
 * @ingroup Classes Classes
 * @{
 */

class Camera : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the Camera class.
     *
     * This function initializes a new instance of the Camera class.
     */
    void constructor() {
        this->type = __type__;

        Camera *cam;
        cam = malloc(sizeof(Camera));
        POINTER_CHECK(cam);
        init_camera(cam);

        this->object = cam;
        SUPER(initialize_node);
    }

    /**
     * @brief Loads a Camera object from a file.
     *
     * This function reads data from the specified file and initializes a Camera object.
     *
     * @param file A pointer to the file from which the Camera data will be read.
     * @param c A double pointer to the Camera object that will be initialized.
     * @param scripts A pointer to the Script object associated with the Camera.
     * @param editor A pointer to the Node object representing the editor.
     */
    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        UNUSED(scripts);
        this::constructor();
        Camera *cam = this->object;
        if (file) {
            int active_camera;
            fscanf(file,"(%d)", &active_camera);
            if (active_camera) {
                if (c) *c = cam;
                else if (editor) editor->attribute[5].node = this;
            }
        }
    }

    /**
     * @brief Updates the camera's position, rotation, and scale.
     *
     * This function updates the camera's transformation properties including
     * position, rotation, and scale based on the provided vectors.
     *
     * @param pos Vec3 structure representing the new position of the camera.
     * @param rot Vec3 structure representing the new rotation of the camera.
     * @param scale Vec3 structure representing the new scale of the camera.
     */
    void update(vec3 pos, vec3 rot, vec3 scale) {
        Camera *camera = (Camera *) this->object;

        this::update_global_position(pos, rot, scale);
        glm_vec3_negate_to(pos, camera->pos);
        glm_vec3_negate_to(rot, camera->rot);
    }

    /**
     * @brief Retrieves the settings data.
     *
     * This function retrieves the settings data and stores it in the provided pointer.
     * It also sets the length of the data.
     *
     * @param[out] ptr A pointer to a pointer to a pointer where the settings data will be stored.
     *                 The function will allocate memory for the settings data and set this pointer
     *                 to point to the allocated memory.
     * @param[out] length A pointer to an integer where the length of the settings data will be stored.
     *                    The function will set this integer to the length of the settings data.
     */
    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        void *data[] = {
            "camera", "Active : "
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    /**
     * @brief Saves the state of the editor node to a file.
     *
     * This function writes the current state of the editor node to the specified file.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     * @param editor A pointer to the Node object representing the editor whose state is to be saved.
     */
    void save(FILE *file, Node *editor) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        if (editor) fprintf(file, "(%d)", !!(editor->attribute[5].node == this));
        else fprintf(file, "(%d)", 0);
    }
}

