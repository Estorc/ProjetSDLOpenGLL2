#pragma once

#include <io/model_loaders/obj_loader.h>
#include <io/model_loaders/mtl_loader.h>
#include <io/model_loaders/fbx_loader.h>


/**
 * @file model_loader.h
 * @brief Header file for model loading functions.
 *
 * This file contains the declaration of functions used for loading 3D models.
 */

/**
 * @brief Loads a 3D model from the specified file path.
 *
 * This function reads a 3D model from the given file path and stores the
 * model data in the provided ModelData pointer.
 *
 * @param path The file path to the 3D model to be loaded.
 * @param modelPtr A pointer to a pointer of ModelData where the loaded model
 *                 data will be stored.
 * @return An integer indicating the success or failure of the model loading
 *         operation. Typically, 0 indicates success, while a non-zero value
 *         indicates an error.
 */
int load_model(const char *path, ModelData ** modelPtr);