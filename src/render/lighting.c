#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <framebuffer.h>
#include <storage/node.h>
#include <window.h>
#include <camera.h>
#include <render.h>
#include <memory.h>
#include <lighting.h>
#include <depth_map.h>
#include <classes/classes.h>




void set_lightings(u8 lightsCount[LIGHTS_COUNT]) {
    for (int i = 0; i < Game.memoryCaches->shadersCount; i++) {
        use_shader(Game.memoryCaches->shaderCache[i].shader);
        set_shader_int(Game.memoryCaches->shaderCache[i].shader, "pointLightsNum", lightsCount[POINT_LIGHT]);
        set_shader_int(Game.memoryCaches->shaderCache[i].shader, "dirLightsNum", lightsCount[DIRECTIONAL_LIGHT]);
        set_shader_int(Game.memoryCaches->shaderCache[i].shader, "spotLightsNum", lightsCount[SPOT_LIGHT]);
    }
}
