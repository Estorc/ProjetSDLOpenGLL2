#pragma once

/**
 * @file signals.h
 * @brief Signal and slot system for C projects.
 * 
 * This file contains the definitions of various structures used to create and manage
 * signals and slots in a C project.
 * 
 * @{
 */


/**
 * @enum Signal
 * @brief Represents a signal.
 * 
 * This enumeration defines the possible signals that can be emitted in the application.
 */
typedef enum Signal {
    SIGNAL_BUTTON_HOVERED,
    SIGNAL_BUTTON_CLICKED,
    SIGNAL_AREA_COLLISION,
    SIGNAL_COUNT,
} Signal;

/**
 * @}
 */