#include <ufbx/ufbx.h>
#include "../src/raptiquax.h"
#include "../src/math/math_util.h"
#include "../src/io/model.h"
#include "../src/io/shader.h"
#include "../src/memory.h"



int main() {
    const char path[] = "assets/models/rigtest/rigtest.fbx";
    ModelData *model = NULL;
    load_model(path, &model);
}