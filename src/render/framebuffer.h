#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/**
 * @brief Functions for managing MSAA framebuffers.
 * @defgroup Framebuffer Framebuffer Management
 * @{
 */

/**
 * @typedef FBO
 * @brief Alias for an unsigned int representing a Frame Buffer Object.
 */
typedef unsigned int FBO;

/**
 * @typedef FrameBuffer
 * @brief Alias for FBO, representing a Frame Buffer Object.
 */
typedef FBO FrameBuffer;

/**
 * @typedef RBO
 * @brief Alias for an unsigned int representing a Render Buffer Object.
 */
typedef unsigned int RBO;

/**
 * @struct MSAA
 * @brief A structure to handle Multi-Sample Anti-Aliasing (MSAA) framebuffers.
 *
 * This structure contains the necessary components to manage MSAA framebuffers,
 * including the main framebuffer, an intermediate framebuffer, a renderbuffer
 * object, and texture maps for multi-sampled color buffer and screen texture.
 */
typedef struct MSAA {
    FBO framebuffer; /** < The main MSAA framebuffer. */
    FBO intermediateFBO; /** < The intermediate framebuffer for post-processing. */
    RBO rbo; /** < The renderbuffer object for depth and stencil attachments. */
    TextureMap textureColorBufferMultiSampled; /** < The multi-sampled color attachment texture. */
    TextureMap screenTexture; /** < The screen texture for post-processing. */
} MSAA;

/**
 * @brief Creates an MSAA framebuffer.
 *
 * This function initializes and sets up a multisample anti-aliasing (MSAA) framebuffer.
 * MSAA is used to improve the visual quality of rendered images by reducing aliasing.
 *
 * @param msaa A pointer to an MSAA structure that will hold the framebuffer information.
 */
void create_msaa_framebuffer(MSAA *msaa);

/**
 * @brief Frees the resources associated with an MSAA framebuffer.
 *
 * This function deallocates and cleans up the resources used by an MSAA framebuffer.
 * It should be called when the framebuffer is no longer needed to avoid memory leaks.
 *
 * @param msaa A pointer to an MSAA structure that holds the framebuffer information to be freed.
 */
void free_msaa_framebuffer(MSAA *msaa);

/**
 * @brief Resizes an MSAA framebuffer.
 *
 * This function adjusts the size of an existing MSAA framebuffer to match new dimensions.
 * It is useful when the window or viewport size changes and the framebuffer needs to be updated accordingly.
 *
 * @param msaa A pointer to an MSAA structure that holds the framebuffer information to be resized.
 */
void resize_msaa_framebuffer(MSAA *msaa);

/** @} */ // end of Framebuffer group
#endif