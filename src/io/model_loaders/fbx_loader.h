#pragma once

/**
 * @file fbx_loader.h
 * @brief Header file for loading FBX models.
 *
 * This file contains the declaration of the function used to load FBX models
 * into the application.
 */

/**
 * @brief Loads an FBX model from the specified file path.
 *
 * This function reads an FBX file from the given path and loads its data into
 * the provided ModelData structure. The function returns an integer status
 * code indicating the success or failure of the operation.
 *
 * @param path The file path to the FBX model to be loaded.
 * @param modelPtr A pointer to a ModelData structure where the loaded model
 *                 data will be stored.
 * @return An integer status code. Returns 0 on success, or a non-zero error
 *         code on failure.
 */
int load_fbx_model(const char *path, ModelData *modelPtr);