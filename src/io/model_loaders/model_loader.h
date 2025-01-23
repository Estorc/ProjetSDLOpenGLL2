#pragma once

#include "obj_loader.h"
#include "mtl_loader.h"
#include "fbx_loader.h"


int load_model(const char *path, ModelData ** modelPtr);