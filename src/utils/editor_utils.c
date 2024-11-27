void draw_model_input(int *id, int x, int y, Window *window, Node *node, Model **model, Input *input, TTF_Font *font, char * name) {
	char str[150];

    if (draw_button(window->ui_surface, x+800-48, y, 32, 32, "assets/textures/editor/folder.png", 0xffffffff, input)) {
        char path[200];
        if (osio_open_file(path, "\"Object 3d Model File(*.obj)\"") == 0) {
            absolute_path_to_relative(path);
            printf("%s\n", path);
            FILE * file = fopen(path, "r");
            if (file) {
                Model *new_model;
                load_obj_model(path, &new_model);
                *model = new_model;
                fclose(file);
            }
        }
    }
    sprintf(str, "%s: %s", name, "None");
    for (int i = 0; i < memoryCaches.modelsCount; i++) {
        if (memoryCaches.modelCache[i].model == *model) {
            sprintf(str, "%s: %s", name, memoryCaches.modelCache[i].modelName);
            break;
        }
    }
	draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
    (*id)++;
}

void draw_texture_input(int *id, int x, int y, Window *window, Node *node, TextureMap *texture, Input *input, TTF_Font *font, char * name) {
	char str[150];

    if (draw_button(window->ui_surface, x+800-48, y, 32, 32, "assets/textures/editor/folder.png", 0xffffffff, input)) {
        char path[200];
        if (osio_open_file(path, "\"image/png image/bmp image/jpeg\"") == 0) {
            absolute_path_to_relative(path);
            FILE * file = fopen(path, "r");
            if (file) {
                printf("%s\n", path);
                (*texture) = load_texture_from_path(path);
                fclose(file);
            }
        }
    }
    sprintf(str, "%s: %s", name, "None");
    for (int i = 0; i < memoryCaches.texturesCount; i++) {
        if (memoryCaches.textureCache[i].textureMap == *texture) {
            sprintf(str, "%s: %s", name, memoryCaches.textureCache[i].textureName);
            break;
        }
    }
	draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
    (*id)++;
}

void draw_cubemap_input(int *id, int x, int y, Window *window, Node *node, TextureMap *texture, Input *input, TTF_Font *font, char * name) {
	char str[150];

    if (draw_button(window->ui_surface, x+800-48, y, 32, 32, "assets/textures/editor/folder.png", 0xffffffff, input)) {
        char path[6][200];
        int res = 0;
        for (int i = 0; i < 6; i++) res |= osio_open_file(path[i], "\"image/png image/bmp image/jpeg\"");
        if (res == 0) {
            for (int i = 0; i < 6; i++) absolute_path_to_relative(path[i]);
            (*texture) = load_cubemap(path);
        }
    }
    (*id)++;
    for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
        if (memoryCaches.cubeMapCache[i].cubeMap == *texture) {
            sprintf(str, "%s: %s", name, memoryCaches.cubeMapCache[i].textureName[0]);
            draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
            sprintf(str, "%s: %s", name, memoryCaches.cubeMapCache[i].textureName[1]);
            draw_text(window->ui_surface, x+10, y+32, str, font, (SDL_Color) {255, 255, 255, 255});
            sprintf(str, "%s: %s", name, memoryCaches.cubeMapCache[i].textureName[2]);
            draw_text(window->ui_surface, x+10, y+64, str, font, (SDL_Color) {255, 255, 255, 255});
            sprintf(str, "%s: %s", name, memoryCaches.cubeMapCache[i].textureName[3]);
            draw_text(window->ui_surface, x+10, y+96, str, font, (SDL_Color) {255, 255, 255, 255});
            sprintf(str, "%s: %s", name, memoryCaches.cubeMapCache[i].textureName[4]);
            draw_text(window->ui_surface, x+10, y+128, str, font, (SDL_Color) {255, 255, 255, 255});
            sprintf(str, "%s: %s", name, memoryCaches.cubeMapCache[i].textureName[5]);
            draw_text(window->ui_surface, x+10, y+160, str, font, (SDL_Color) {255, 255, 255, 255});
            return;
        }
    }
    sprintf(str, "%s: None", name);
    draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
    sprintf(str, "%s: None", name);
    draw_text(window->ui_surface, x+10, y+32, str, font, (SDL_Color) {255, 255, 255, 255});
    sprintf(str, "%s: None", name);
    draw_text(window->ui_surface, x+10, y+64, str, font, (SDL_Color) {255, 255, 255, 255});
    sprintf(str, "%s: None", name);
    draw_text(window->ui_surface, x+10, y+96, str, font, (SDL_Color) {255, 255, 255, 255});
    sprintf(str, "%s: None", name);
    draw_text(window->ui_surface, x+10, y+128, str, font, (SDL_Color) {255, 255, 255, 255});
    sprintf(str, "%s: None", name);
    draw_text(window->ui_surface, x+10, y+160, str, font, (SDL_Color) {255, 255, 255, 255});
}


void draw_vec3_input(int *id, int x, int y, Window *window, Node *node, vec3 vector, Input *input, TTF_Font *font, char * name) {
	char str[100];
	sprintf(str, "%s:", name);
	draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
	for (int i = 0; i < 3; i++) {
		sprintf(str, "%c: ", 'X' + i);
		draw_text(window->ui_surface, x+190, y, str, font, (SDL_Color) {255, 255, 255, 255});
		if (draw_input_box(window->ui_surface, x+190 + 50, y-1, 140, 28, 0xffd0d0d0, input))
			node->params[3].i = (*id);
		if (node->params[3].i == (*id)) {
			sprintf(str, "%s", input->inputBuffer);
			sscanf(str, "%f", &vector[i]);
		} else sprintf(str, "%.2f", vector[i]);
		draw_text(window->ui_surface, x+190 + 50, y, str, font, (SDL_Color) {255, 255, 255, 255});
		x += 200;
        (*id)++;
	}
}

void draw_rgb_input(int *id, int x, int y, Window *window, Node *node, vec3 vector, Input *input, TTF_Font *font, char * name) {
	char str[100];
	sprintf(str, "%s:", name);
	draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
	for (int i = 0; i < 3; i++) {
		sprintf(str, "%c: ", "RGB"[i]);
		draw_text(window->ui_surface, x+190, y, str, font, (SDL_Color) {255, 255, 255, 255});
		if (draw_input_box(window->ui_surface, x+190 + 50, y-1, 140, 28, 0xffd0d0d0, input))
			node->params[3].i = (*id);
		if (node->params[3].i == (*id)) {
			sprintf(str, "%s", input->inputBuffer);
			sscanf(str, "%f", &vector[i]);
		} else sprintf(str, "%.2f", vector[i]);
		draw_text(window->ui_surface, x+190 + 50, y, str, font, (SDL_Color) {255, 255, 255, 255});
		x += 200;
        (*id)++;
	}
}

void draw_float_input(int *id, int x, int y, Window *window, Node *node, float *flt, Input *input, TTF_Font *font, char * name) {
	char str[100];
	sprintf(str, "%s:", name);
	draw_text(window->ui_surface, x+10, y, str, font, (SDL_Color) {255, 255, 255, 255});
	if (draw_input_box(window->ui_surface, x+190, y-1, 590, 28, 0xffd0d0d0, input))
		node->params[3].i = (*id);
	if (node->params[3].i == (*id)) {
		sprintf(str, "%s", input->inputBuffer);
		sscanf(str, "%f", flt);
	} else sprintf(str, "%.2f", *flt);
	draw_text(window->ui_surface, x+190, y, str, font, (SDL_Color) {255, 255, 255, 255});
    (*id)++;
}


int update_scene_for_node_editing(Window *window, Node *node, Input *input, TTF_Font *font) {
    Node *selectedNode = node->params[2].node;
    int *scrollY = &node->params[0].i;
    if (*scrollY < 100) *scrollY = 100;
	vec3 gap = {cos(window->time) * 10.0 * *scrollY * 0.01, 0.5 * *scrollY * 0.01, sin(window->time) * 10.0 * *scrollY * 0.01};
	node->children[0]->rot[1] = atan2(gap[0], gap[2])*180.0/PI + 180.0;
	node->children[0]->rot[0] = 0.0;
	node->children[0]->rot[2] = 0.0;
	glm_vec3_copy(selectedNode->globalPos, node->children[0]->pos);
	glm_vec3_add(node->children[0]->pos, gap, node->children[0]->pos);

	s8 collisionShapeID = -1;
	switch (selectedNode->type) {
		case NODE_BOX_CSHAPE:
			collisionShapeID = 0;
			break;
		case NODE_PLANE_CSHAPE:
			collisionShapeID = 1;
			break;
		case NODE_SPHERE_CSHAPE:
			collisionShapeID = 2;
			if (node->params[3].i == 7) {
				selectedNode->scale[1] = selectedNode->scale[0];
				selectedNode->scale[2] = selectedNode->scale[0];
			}
			if (node->params[3].i == 8) {
				selectedNode->scale[0] = selectedNode->scale[1];
				selectedNode->scale[2] = selectedNode->scale[1];
			}
			if (node->params[3].i == 9) {
				selectedNode->scale[0] = selectedNode->scale[2];
				selectedNode->scale[1] = selectedNode->scale[2];
			}
			break;
		default: break;
	}

	if (collisionShapeID > -1) {
		node->children[1]->children[collisionShapeID]->flags |= NODE_VISIBLE;
		glm_vec3_copy(selectedNode->parent->globalPos, node->children[1]->children[collisionShapeID]->pos);
		glm_vec3_copy(selectedNode->parent->globalRot, node->children[1]->children[collisionShapeID]->rot);
		if (collisionShapeID != 2) glm_vec3_copy(selectedNode->parent->globalScale, node->children[1]->children[collisionShapeID]->scale);
		else glm_vec3_copy(selectedNode->scale, node->children[1]->children[collisionShapeID]->scale);
		update_global_position(selectedNode, node->children[1]->children[collisionShapeID]->pos, node->children[1]->children[collisionShapeID]->rot, node->children[1]->children[collisionShapeID]->scale);
	}
    return collisionShapeID;
}


void load_node_tree(FILE * file, Node *node) {

	Node *loadedScene = load_node(file, 0, scripts, node->parent);
	//free_node(node->children[2]);
	loadedScene->flags &= ~NODE_ACTIVE;
	loadedScene->parent = node->parent;
	node->parent->children[2] = loadedScene;
    buffers.collisionBuffer.collisionsShapes = realloc(buffers.collisionBuffer.collisionsShapes, sizeof(Node *) * buffers.collisionBuffer.length);
    buffers.lightingBuffer.lightings = realloc(buffers.lightingBuffer.lightings, sizeof(Node *) * buffers.lightingBuffer.length);

}



void save_node_tree(FILE * file, Window *window, Node *node, Node *editor, Input *input, TTF_Font *font) {

	node_tree_to_file(file, node, editor);
	u8 *collisionsLength;
	Node ***collisionsShapes;
	GET_FROM_BODY_NODE(node, length, collisionsLength);
	GET_FROM_BODY_NODE(node, collisionsShapes, collisionsShapes);
	if (IS_NODE_BODY(node->type)) {
		fprintf(file, "\n");
		for (int i = 0; i < *collisionsLength; i++) {
			save_node_tree(file, window, (*collisionsShapes)[i], editor, input, font);
		}
	}
	fprintf(file, "[");
	fprintf(file, "m%f,%f,%f", node->pos[0], node->pos[1], node->pos[2]);
	fprintf(file, "r%f,%f,%f", node->rot[0], node->rot[1], node->rot[2]);
	fprintf(file, "s%f,%f,%f", node->scale[0], node->scale[1], node->scale[2]);
	fprintf(file, "a%d", !!(node->flags & NODE_ACTIVE));
	fprintf(file, "v%d", !!(node->flags & NODE_VISIBLE));
	fprintf(file, "]");

	if (node->flags & NODE_SCRIPT) {
		char scriptname[100] = "None";
		for (int i = 0; i < SCRIPTS_COUNT; i++) {
			if (scripts[i].script == node->script) {
				strcpy(scriptname, scripts[i].name);
				break;
			}
				
		}
		fprintf(file, "{%s}", scriptname);
	}
	if (node->length) fprintf(file, ":%d", node->length);
	fprintf(file, "\n");
    for (int i = 0; i < node->length; i++) {
        save_node_tree(file, window, node->children[i], editor, input, font);
    }

}



int draw_node_params(int x, int y, Window *window, Node *editor, Input *input, TTF_Font *font) {

    int c = 1;
    Node *node = editor->params[2].node;
    switch (node->type) {
        case NODE_EMPTY:
            {
            //
            }
            break;

        case NODE_CAMERA:
            {
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            if (draw_button(window->ui_surface, x+400-16, y, 32, 32, editor->params[5].node == node ? "assets/textures/editor/camera.png" : "assets/textures/editor/camera-off.png", 0xffffffff, input)) {
                if (editor->params[5].node == node) editor->params[5].node = 0;
                else editor->params[5].node = node;
            }
            y += 32;
            }
            break;

        case NODE_KINEMATIC_BODY:
            {
            KinematicBody *kinematicBody = (KinematicBody*) node->object;

            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_vec3_input(&c, x, y, window, editor, kinematicBody->velocity, input, font, "Velocity");
            y += 32;
            }
            break;

        case NODE_RIGID_BODY:
            {
            RigidBody *rigidBody = (RigidBody*) node->object;

            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_vec3_input(&c, x, y, window, editor, rigidBody->velocity, input, font, "Velocity");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_vec3_input(&c, x, y, window, editor, rigidBody->angularVelocity, input, font, "Ang. Vel.");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_vec3_input(&c, x, y, window, editor, rigidBody->gravity, input, font, "Gravity");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &rigidBody->mass, input, font, "Mass");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &rigidBody->friction, input, font, "Friction");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_vec3_input(&c, x, y, window, editor, rigidBody->centerOfMass, input, font, "CoM");
            y += 32;
            break;
            }
        case NODE_STATIC_BODY:
            {
            //
            }
            break;

        case NODE_MESH:
            {
            //
            }
            break;

        case NODE_MODEL:
            {
            Model **model = (Model**) &node->object;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_model_input(&c, x, y, window, editor, model, input, font, "Model");
            y += 32;
            }
            break;

        case NODE_TEXTURED_MESH:
            {
            TextureMap *texture = &((TexturedMesh*) node->object)->texture;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_texture_input(&c, x, y, window, editor, texture, input, font, "Texture");
            y += 32;
            }
            break;

        case NODE_SKYBOX:
            {
            TextureMap *texture = &((TexturedMesh*) node->object)->texture;
            draw_rectangle(window->ui_surface, x, y, 800, 192, 0xff555555);
            draw_cubemap_input(&c, x, y, window, editor, texture, input, font, "Texture");
            y += 192;
            }
            break;

        case NODE_VIEWPORT:
            {
            //
            }
            break;

        case NODE_FRAMEBUFFER:
            {
            //
            }
            break;

        case NODE_POINT_LIGHT:
            {
            PointLight *pointLight = (PointLight*) node->object;

            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, pointLight->ambient, input, font, "Ambient");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, pointLight->diffuse, input, font, "Diffuse");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, pointLight->specular, input, font, "Specular");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &pointLight->constant, input, font, "Constant");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &pointLight->linear, input, font, "Linear");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &pointLight->quadratic, input, font, "Quadratic");
            y += 32;
            break;
            }

        case NODE_DIRECTIONAL_LIGHT:
            {
            DirectionalLight *directionalLight = (DirectionalLight*) node->object;

            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, directionalLight->ambient, input, font, "Ambient");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, directionalLight->diffuse, input, font, "Diffuse");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, directionalLight->specular, input, font, "Specular");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &directionalLight->constant, input, font, "Constant");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &directionalLight->linear, input, font, "Linear");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &directionalLight->quadratic, input, font, "Quadratic");
            y += 32;
            break;
            }
        case NODE_SPOT_LIGHT:
            {
            SpotLight *spotLight = (SpotLight*) node->object;

            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, spotLight->ambient, input, font, "Ambient");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, spotLight->diffuse, input, font, "Diffuse");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_rgb_input(&c, x, y, window, editor, spotLight->specular, input, font, "Specular");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &spotLight->constant, input, font, "Constant");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &spotLight->linear, input, font, "Linear");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &spotLight->quadratic, input, font, "Quadratic");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &spotLight->cutOff, input, font, "Cut Off");
            y += 32;
            draw_rectangle(window->ui_surface, x, y, 800, 32, 0xff555555);
            draw_float_input(&c, x, y, window, editor, &spotLight->outerCutOff, input, font, "Outer Cut Off");
            y += 32;
            break;
            }
    }

    return y;
}