#pragma once

/**
 * @defgroup NodeLoader Node Loader
 * @brief Functions for loading and saving nodes.
 * @{
 */

/**
 * @brief Allocates memory for a node and initializes it.
 *
 * This function allocates memory for a node and initializes it based on the provided node type.
 * It also reads additional data from the given file and sets up the camera and scripts.
 *
 * @param node Pointer to the node to be allocated and initialized.
 * @param nodeType The type of the node to be created.
 * @param file Pointer to the file from which additional data is read.
 * @param c Pointer to the camera array to be set up.
 * @param scripts Array of scripts to be initialized.
 * @param editor Pointer to the editor node.
 */
void malloc_node(Node *node, int nodeType, FILE *file, Camera **c, Script scripts[SCRIPTS_COUNT], Node *editor);

/**
 * @brief Writes the node tree to a file.
 *
 * This function traverses the node tree and writes its structure and data to the specified file.
 * It includes the editor node in the output.
 *
 * @param file Pointer to the file where the node tree will be written.
 * @param node Pointer to the root node of the tree to be written.
 * @param editor Pointer to the editor node to be included in the output.
 */
void node_tree_to_file(FILE * file, Node *node, Node *editor);

/** @} */ // end of NodeLoader