#include <ufbx/ufbx.h>
#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <io/shader.h>
#include <memory.h>
#include "test.h"

int test() {
    return 0;
}

int main(int argc, char **argv) {
    #ifndef WIN32
    const char path[] = "assets/models/rigtest/rigtest.fbx";
    ModelData *model = NULL;
    load_model(path, &model);
    #endif
    return 0;
}