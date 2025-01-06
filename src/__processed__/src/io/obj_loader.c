#line 1 "src/io/obj_loader.c"
#include "../types.h"
#include "../math/math_util.h"
#include "model.h"
#include "shader.h"
#include "../memory.h"



int malloc_obj(ModelObjectData *obj, u32 vim, u32 fim, u32 vnim, u32 vtim) {
    obj->materials = NULL;
    obj->materialsLength = NULL;
    obj->materialsCount = 0;
    obj->vertex = malloc(sizeof(Vertex) * vim);
    obj->faces = malloc(sizeof(Face) * fim);
    obj->facesVertex = malloc(sizeof(Vertex)*3 * fim);
    obj->normals = malloc(sizeof(Normal) * vnim);
    obj->textureVertex = malloc(sizeof(TextureVertex) * vtim);
    POINTER_CHECK(obj->vertex);
    POINTER_CHECK(obj->faces);
    POINTER_CHECK(obj->facesVertex);
    POINTER_CHECK(obj->normals);
    POINTER_CHECK(obj->textureVertex);
    return 0;
}


void create_obj_vao(ModelObjectData *obj) {
    VBO VBO;
    glGenBuffers(1, &VBO);

    VAO VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * obj->length * 3, obj->facesVertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    obj->VAO = VAO;
}

void close_realloc_obj(ModelObjectData *obj, u32 vi, u32 fi, u32 vni, u32 vti) {
    obj->length = fi;

    create_obj_vao(obj);
    free(obj->textureVertex);
    free(obj->faces);
    obj->displayLists = malloc(sizeof(GLuint) * obj->materialsCount);
    POINTER_CHECK(obj->displayLists);
    memset(obj->displayLists, 0, sizeof(GLuint) * obj->materialsCount);
}



int load_obj_model(char *path, ModelData **modelPtr) {

    for (int i = 0; i < memoryCaches.modelsCount; i++) {
        if (!strcmp(memoryCaches.modelCache[i].modelName, path)) {
            #ifdef DEBUG
                printf("Model loaded from cache!\n");
            #endif
            (*modelPtr) = memoryCaches.modelCache[i].model;
            return 0;
        }
    }

    ModelData *model = *modelPtr = malloc(sizeof(ModelData));

    int selectedMaterialId;
    #ifdef DEBUG
        printf("Loading model %s\n", path);
    #endif
    FILE * file = fopen(path, "r");
    POINTER_CHECK(file);

    u32 oi = 0, offv = 1, offvt = 1, offvn = 1, fi = 0, vi = 0, vti = 0, vni = 0, fim = 30, vim = 50, vtim = 50, vnim = 50, oim = 1, mc = 0;

    model->objects = malloc(sizeof(ModelObjectData) * oim);
    POINTER_CHECK(model->objects);
    ModelObjectData *object = NULL;
    

    char symbol;
    do {
        if (object) {
            if (vi >= vim) {
                
                vim<<=1;
                object->vertex = realloc(object->vertex, sizeof(Vertex) * vim);
                POINTER_CHECK(object->vertex);
            }
            if (vni >= vnim) {
                
                vnim<<=1;
                object->normals = realloc(object->normals, sizeof(Normal) * vnim);
                POINTER_CHECK(object->normals);
            }
            if (vti >= vtim) {
                
                vtim<<=1;
                object->textureVertex = realloc(object->textureVertex, sizeof(TextureVertex) * vtim);
                POINTER_CHECK(object->textureVertex);
            }
            if (fi >= fim) {
                
                fim<<=1;
                object->faces = realloc(object->faces, sizeof(Face) * fim);
                object->facesVertex = realloc(object->facesVertex, sizeof(Vertex)*3 * fim);
                POINTER_CHECK(object->faces);
                POINTER_CHECK(object->facesVertex);
            }
        }
        symbol = getc(file);
        switch (symbol) {
        case 'v': ;
            symbol = getc(file);
            switch (symbol) {
                case ' ': ;
                    fscanf(file, "%f %f %f\n", &object->vertex[vi][0],&object->vertex[vi][1],&object->vertex[vi][2]);
                    vi++;
                    break;
                case 't': ;
                    fscanf(file, "%f %f\n", &object->textureVertex[vti][0],&object->textureVertex[vti][1]);
                    vti++;
                    break;
                case 'n': ;
                    fscanf(file, "%f %f %f\n", &object->normals[vni][0],&object->normals[vni][1],&object->normals[vni][2]);
                    vni++;
                    break;
                default: fscanf(file, "%*[^\n]\n");
            }
            break;
        case 'f': ;
            if (!object->materialsCount || model->materials[selectedMaterialId].name != object->materials[object->materialsCount-1]->name) {
                
                object->materialsCount++;
                object->materials = realloc(object->materials, sizeof(Material *) * (object->materialsCount));
                object->materialsLength = realloc(object->materialsLength, sizeof(u32) * (object->materialsCount));
                POINTER_CHECK(object->materials);
                POINTER_CHECK(object->materialsLength);
                object->materials[object->materialsCount-1] = &model->materials[selectedMaterialId];
                object->materialsLength[object->materialsCount-1] = 0;
                
            }
            object->materialsLength[object->materialsCount-1]++;
            int vertexId[MAX_VERTEX_PER_FACE], normalsId[MAX_VERTEX_PER_FACE], textureVertexId[MAX_VERTEX_PER_FACE];
            int i = 0;
            symbol = 0;
            do {
                fscanf(file, "%d/%d/%d", &vertexId[i], &textureVertexId[i], &normalsId[i]);
                i++;
            } while ((symbol = getc(file)) == ' ');
            object->faces[fi].length = i;
            for (int j = 0; j < i; j++) {
                object->faces[fi].vertex[j] = vertexId[j] - offv;

                memcpy(&object->facesVertex[fi][j][0], &object->vertex[vertexId[j] - offv], sizeof(float) * 3);
                memcpy(&object->facesVertex[fi][j][3], &object->normals[normalsId[j] - offvn], sizeof(float) * 3);
                memcpy(&object->facesVertex[fi][j][6], &object->textureVertex[textureVertexId[j] - offvt], sizeof(float) * 2);

                object->faces[fi].normals[j] = normalsId[j] - offvn;
                object->faces[fi].textureVertex[j] = textureVertexId[j] - offvt;
            }
            
            vec2 deltaUV1;
            vec2 deltaUV2;
            glm_vec2_sub(&object->facesVertex[fi][1][6], &object->facesVertex[fi][0][6], deltaUV1);
            glm_vec2_sub(&object->facesVertex[fi][2][6], &object->facesVertex[fi][0][6], deltaUV2);
            vec3 edge1;
            vec3 edge2;
            glm_vec3_sub(object->vertex[vertexId[1]-offv], object->vertex[vertexId[0]-offv],edge1);
            glm_vec3_sub(object->vertex[vertexId[2]-offv], object->vertex[vertexId[0]-offv],edge2);

            vec3 tangent;
            vec3 bitangent;
            double f = 1.0 / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);

            tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
            tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
            tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
            glm_vec3_normalize(tangent);

            bitangent[0] = f * (deltaUV1[0] * edge2[0] - deltaUV2[0] * edge1[0]);
            bitangent[1] = f * (deltaUV1[0] * edge2[1] - deltaUV2[0] * edge1[1]);
            bitangent[2] = f * (deltaUV1[0] * edge2[2] - deltaUV2[0] * edge1[2]);
            glm_vec3_normalize(bitangent);

            for (int j = 0; j < i; j++) {
                memcpy(&object->facesVertex[fi][j][8], tangent, sizeof(tangent));
                memcpy(&object->facesVertex[fi][j][11], bitangent, sizeof(bitangent));
            }
            fi++;
            break;
        case 'm': ;
            
            char materialFilename[50];
            fscanf(file, "%*[^ ] %s\n", (char *) &materialFilename);
            char * material_path = get_folder_path(path);
            if ((mc = load_mtl(material_path, materialFilename, &model->materials)) == -1) return -1;
            free(material_path);
            model->materialsCount = mc;
            break;
        case 's': ;
            
            fscanf(file, "%hhd\n", &object->smoothShading);
            break;
        case 'u': ;
            
            char materialName[50];
            fscanf(file, "%*[^ ] %[^\n]", (char *) materialName);
            selectedMaterialId = find_material(model->materials, mc, materialName);
            break;
        case 'o': ;
            
            fscanf(file, "%*[^\n]");
            if (object) {
                close_realloc_obj(object, vi, fi, vni, vti);

                oi++;
                if (oi >= oim) {
                    oim<<=1;
                    model->objects = realloc(model->objects, sizeof(ModelObjectData) * oim);
                    POINTER_CHECK(model->objects);
                }
            }
            object = &(model->objects[oi]);
            offv += vi, offvt += vti, offvn += vni, fi = 0, vi = 0, vti = 0, vni = 0, fim = 30, vim = 50, vtim = 50, vnim = 50;
            malloc_obj(object, vim, fim, vnim, vtim);
            break;
        case '\n': ;
            break;
        case '#': ;
        default:
            fscanf(file, "%*[^\n]\n");
            break;
        }
    } while (symbol != -1);
    
    close_realloc_obj(object, vi, fi, vni, vti);
    oi++;
    model->objects = realloc(model->objects, sizeof(ModelObjectData) * oi);
    POINTER_CHECK(model->objects);
    model->length = oi;

    memoryCaches.modelCache = realloc(memoryCaches.modelCache, sizeof (ModelCache) * (++memoryCaches.modelsCount));
    memoryCaches.modelCache[memoryCaches.modelsCount-1].model = model;
    strcpy(memoryCaches.modelCache[memoryCaches.modelsCount-1].modelName, path);
    
    return 0;

}