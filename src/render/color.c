#include "../types.h"
#include "color.h"
#include <stdio.h>


/**
 * Converts RGBA color components from 0-255 range to a normalized floating-point format.
 * 
 * @param r {u8} Red component of the color (0-255).
 * @param g {u8} Green component of the color (0-255).
 * @param b {u8} Blue component of the color (0-255).
 * @param a {u8} Alpha component of the color (0-255).
 * 
 * @return {ColRGBA} A ColRGBA structure containing the normalized color components,
 *                   where each component is in the range [0.0, 1.0].
 * 
 * This function takes the red, green, blue, and alpha components of a color,
 * which are typically represented as unsigned 8-bit integers (0-255), and converts
 * them into a normalized format (0.0-1.0) suitable for rendering in graphics applications.
 * The function creates a ColRGBA structure that contains the converted color values
 * and returns it.
 */

ColRGBA get_color(u8 r, u8 g, u8 b, u8 a) {
    ColRGBA col = {
        .r = (f32) r/255,
        .g = (f32) g/255,
        .b = (f32) b/255,
        .a = (f32) a/255
    };
    return col;
}


/**
 * Darkens a given color by multiplying its RGB components with a specified rate.
 * 
 * @param col {ColRGBA*} Pointer to the ColRGBA structure representing the color to be darkened.
 * @param rate {f32} The rate by which to darken the color. A value between 0.0 and 1.0 
 *                   will darken the color, where 0.0 results in black and 1.0 leaves the 
 *                   color unchanged.
 * 
 * This function takes a pointer to a ColRGBA structure and modifies its RGB components 
 * by multiplying each component (red, green, and blue) by the specified rate. The alpha 
 * component remains unchanged. This is useful for achieving darker shades of the original 
 * color while preserving its transparency.
 */

void darken_color(ColRGBA *col, f32 rate) {
    col->r *= rate;
    col->g *= rate;
    col->b *= rate;
}