#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <io/input.h>

/**
 * @brief Functions for managing framebuffers and render targets.
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
 * @}
 * @class RenderTarget
 * @brief A structure to handle render targets.
 * 
 * This structure contains the necessary components to manage render targets,
 * including the framebuffer and texture map.
 * 
 * @note The render target is used to render to a texture instead of the screen.
 * @ingroup Classes Classes
 */
typedef struct RenderTarget {
    FBO fbo;
    TextureMap texture;
    int w;
    int h;
    Mouse mouse;
} RenderTarget;

/**
 * @addtogroup Framebuffer
 * @{ 
 */

/**
 * @struct MSAA
 * @brief A structure to handle Multi-Sample Anti-Aliasing (MSAA) framebuffers.
 *
 * This structure contains the necessary components to manage MSAA framebuffers,
 * including the main framebuffer, an intermediate framebuffer, a renderbuffer
 * object, and texture maps for multi-sampled color buffer and screen texture.
 * 
 * @note DEPRECATED: This structure is deprecated and should not be used in new code.
 */
typedef struct MSAA {
    FBO framebuffer; /** < The main MSAA framebuffer. */
    FBO intermediateFBO; /** < The intermediate framebuffer for post-processing. */
    RBO rbo; /** < The renderbuffer object for depth and stencil attachments. */
    TextureMap textureColorBufferMultiSampled; /** < The multi-sampled color attachment texture. */
    TextureMap screenTexture; /** < The screen texture for post-processing. */
} MSAA;

/**
 * @struct DFBO
 * @brief A structure to handle Deferred Frame Buffer Objects (DFBO).
 * 
 * This structure contains the necessary components to manage deferred framebuffers,
 * including the main framebuffer, an intermediate framebuffer, and texture maps
 * for color buffer, normal buffer, position buffer, and screen texture.
 */
typedef struct DFBO {
    FBO framebuffer; /** < The main DFBO framebuffer. */
    FBO intermediateFBO; /** < The intermediate framebuffer for post-processing. */
    RBO rbo; /** < The renderbuffer object for depth and stencil attachments. */
    TextureMap screenTexture; /** < The screen texture for post-processing. */
    TextureMap textureColorBuffer; /** < The color attachment texture. */
    TextureMap textureNormalBuffer; /** < The normal attachment texture. */
    TextureMap texturePositionBuffer; /** < The position attachment texture. */
    TextureMap textureExtraComponent; /** < The extra component texture for post-processing. */
} DFBO;

/**
 * @struct CFBO
 * @brief A structure to handle Complete Frame Buffer Objects (CFBO).
 * 
 * This structure contains the necessary components to manage complete framebuffers,
 * including the main framebuffer and a texture map for the color attachment.
 */
typedef struct CFBO {
    FBO framebuffer; /** < The main UI framebuffer. */
    TextureMap texture; /** < The color attachment texture. */
} CFBO;


/**
 * @brief Swaps the intermediate framebuffer.
 * 
 * This function swaps the current intermediate framebuffer with the next one.
 * It is used for post-processing effects.
 * 
 * @note This function is typically called after rendering to the main framebuffer
 * and before applying post-processing effects.
 */
void swap_intermediate_fbo();

/**
 * @brief Gets the last intermediate framebuffer texture.
 * 
 * This function retrieves the last intermediate framebuffer texture.
 * It is used for post-processing effects.
 * 
 * @return The last intermediate framebuffer texture.
 * 
 * @note This function is typically called after rendering to the main framebuffer
 * and before applying post-processing effects.
 */
TextureMap get_intermediate_texture();

/**
 * @brief Sets the current framebuffer to an intermediate framebuffer.
 * 
 * This function sets the current framebuffer to the intermediate framebuffer.
 * It is used for post-processing effects.
 * 
 * @note This function is typically called after rendering to the main framebuffer
 * and before applying post-processing effects.
 */
void use_intermediate_fbo();

/**
 * @brief Creates two intermediate framebuffers.
 * 
 * This function initializes and sets up two intermediate framebuffers for post-processing effects.
 * It is typically called during the initialization phase of the rendering pipeline.
 * 
 * @note This function should be called before rendering to the main framebuffer.
 */
void create_intermediate_fbo();

/**
 * @brief Frees the resources associated with the intermediate framebuffers.
 * 
 * This function deallocates and cleans up the resources used by the intermediate framebuffers.
 * It should be called when the framebuffers are no longer needed to avoid memory leaks.
 * 
 * @note This function should be called during the cleanup phase of the rendering pipeline.
 */
void free_intermediate_fbo();

/**
 * @brief Resizes the intermediate framebuffers.
 * 
 * This function adjusts the size of the intermediate framebuffers to match new dimensions.
 * It is useful when the window or viewport size changes and the framebuffers need to be updated accordingly.
 * 
 * @note This function should be called when the window or viewport size changes.
 */
void resize_intermediate_fbo();

/**
 * @brief Sets the current framebuffer.
 * 
 * This function sets the current framebuffer object to the specified framebuffer.
 * 
 * @param fbo The framebuffer object to be set as the current framebuffer.
 * @param mode The mode to set the framebuffer to (GL_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER).
 */
void use_fbo(GLenum mode, FBO fbo);

/**
 * @brief Gets the current framebuffer.
 * 
 * This function returns the current framebuffer object.
 */
FBO get_fbo();

/**
 * @brief Creates a Deferred Frame Buffer Object (DFBO).
 * 
 * This function initializes and sets up a deferred framebuffer object.
 * DFBOs are used for deferred rendering techniques, allowing for efficient
 * rendering of complex scenes with multiple lights.
 * 
 * @param dfbo A pointer to a DFBO structure that will hold the framebuffer information.
 */
void create_dfbo(DFBO *dfbo);

/**
 * @brief Frees the resources associated with a DFBO.
 * 
 * This function deallocates and cleans up the resources used by a DFBO.
 * It should be called when the framebuffer is no longer needed to avoid memory leaks.
 * 
 * @param dfbo A pointer to a DFBO structure that holds the framebuffer information to be freed.
 */
void free_dfbo(DFBO *dfbo);

/**
 * @brief Resizes a DFBO.
 * 
 * This function adjusts the size of an existing DFBO to match new dimensions.
 * It is useful when the window or viewport size changes and the framebuffer needs to be updated accordingly.
 * 
 * @param dfbo A pointer to a DFBO structure that holds the framebuffer information to be resized.
 */
void resize_dfbo(DFBO *dfbo);

/**
 * @brief Creates a CFBO (Complete Frame Buffer Object).
 *
 * This function initializes and sets up a complete framebuffer object.
 * CFBOs are used to render to textures and can be used for various effects.
 * 
 * @param cFbo A pointer to a CFBO structure that will hold the framebuffer information.
 */
void create_cfbo(CFBO *cFbo);

/**
 * @brief Frees the resources associated with a CFBO.
 * 
 * This function deallocates and cleans up the resources used by a CFBO.
 * It should be called when the framebuffer is no longer needed to avoid memory leaks.
 * 
 * @param cFbo A pointer to a CFBO structure that holds the framebuffer information to be freed.
 */
void free_cfbo(CFBO *cFbo);

/**
 * @brief Resizes a CFBO.
 * 
 * This function adjusts the size of an existing CFBO to match new dimensions.
 * It is useful when the window or viewport size changes and the framebuffer needs to be updated accordingly.
 * 
 * @param cFbo A pointer to a CFBO structure that holds the framebuffer information to be resized.
 */
void resize_cfbo(CFBO *cFbo);

/**
 * @brief Creates an MSAA framebuffer.
 *
 * This function initializes and sets up a multisample anti-aliasing (MSAA) framebuffer.
 * MSAA is used to improve the visual quality of rendered images by reducing aliasing.
 *
 * @param msaa A pointer to an MSAA structure that will hold the framebuffer information.
 * @note DEPRECATED: This function is deprecated and should not be used in new code.
 */
void create_msaa(MSAA *msaa);

/**
 * @brief Frees the resources associated with an MSAA framebuffer.
 *
 * This function deallocates and cleans up the resources used by an MSAA framebuffer.
 * It should be called when the framebuffer is no longer needed to avoid memory leaks.
 *
 * @param msaa A pointer to an MSAA structure that holds the framebuffer information to be freed.
 * @note DEPRECATED: This function is deprecated and should not be used in new code.
 */
void free_msaa(MSAA *msaa);

/**
 * @brief Resizes an MSAA framebuffer.
 *
 * This function adjusts the size of an existing MSAA framebuffer to match new dimensions.
 * It is useful when the window or viewport size changes and the framebuffer needs to be updated accordingly.
 *
 * @param msaa A pointer to an MSAA structure that holds the framebuffer information to be resized.
 * @note DEPRECATED: This function is deprecated and should not be used in new code.
 */
void resize_msaa(MSAA *msaa);

/** @} */ // end of Framebuffer group
#endif