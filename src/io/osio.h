#pragma once

/**
 * @file osio.h
 * @brief File operations and error handling for the project.
 * 
 * This header file contains declarations for functions that handle
 * file saving, opening, error printing, and path manipulations.
 */

/**
 * @defgroup OSIO OSIO File Operations
 * @brief Functions for file operations and error handling.
 * @{
 */

/**
 * @brief Saves a file to the specified path with a given filter.
 * 
 * This function attempts to save a file to the location specified by
 * the `path` parameter, applying the filter specified by the `filter`
 * parameter. The filter can be used to specify file types or other
 * criteria.
 * 
 * @param path The path where the file should be saved.
 * @param filter The filter to apply when saving the file.
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int osio_save_file(char *path, char *filter);

/**
 * @brief Opens a file from the specified path with a given filter.
 * 
 * This function attempts to open a file from the location specified by
 * the `path` parameter, applying the filter specified by the `filter`
 * parameter. The filter can be used to specify file types or other
 * criteria.
 * 
 * @param path The path from where the file should be opened.
 * @param filter The filter to apply when opening the file.
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int osio_open_file(char *path, char *filter);

/**
 * @brief Prints an error message to the standard error output.
 * 
 * This function prints the error message specified by the `msg`
 * parameter to the standard error output (stderr). It can be used
 * for logging errors or debugging purposes.
 * 
 * @param msg The error message to be printed.
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int osio_print_error(char *msg);

/**
 * @brief Updates the current working directory.
 * 
 * This function updates the current working directory to the latest
 * state. It can be used to ensure that the program is operating in
 * the correct directory context.
 * 
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int update_cwd();

/**
 * @brief Converts an absolute file path to a relative path.
 * 
 * This function converts the absolute file path specified by the `path`
 * parameter to a relative path. It can be useful for creating paths
 * that are relative to the current working directory or another base
 * directory.
 * 
 * @param path The absolute path to be converted to a relative path.
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int absolute_path_to_relative(char *path);

/** @} */ // end of OSIO group