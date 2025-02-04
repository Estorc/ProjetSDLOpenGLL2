#line 1 "src/io/model_loaders/obj_loader.c"
#include "../../raptiquax.h"
#include "../../math/math_util.h"
#include "../model.h"
#include "../shader.h"
#include "obj_loader.h"
#include "../../memory.h"



int malloc_obj(ModelObjectData *obj, LoadingModelBuffer *buffer, u32 vim, u32 fim, u32 vnim, u32 vtim) {
    obj->materials = NULL;
    obj->materialsLength = NULL;
    obj->materialsCount = 0;
    obj->facesVertex = malloc(sizeof(Vertex)*3 * fim);
    buffer->vertex = malloc(sizeof(Vertex) * vim);
    buffer->faces = malloc(sizeof(Face) * fim);
    buffer->normals = malloc(sizeof(Normal) * vnim);
    buffer->textureVertex = malloc(sizeof(TextureVertex) * vtim);
    POINTER_CHECK(obj->facesVertex);
    POINTER_CHECK(buffer->vertex);
    POINTER_CHECK(buffer->faces);
    POINTER_CHECK(buffer->normals);
    POINTER_CHECK(buffer->textureVertex);
    return 0;
}

void close_realloc_obj(ModelObjectData *obj, LoadingModelBuffer *buffer, u32 vi, u32 fi, u32 vni, u32 vti) {
    obj->length = fi;

    free(buffer->faces);
    free(buffer->normals);
    free(buffer->textureVertex);
    free(buffer->vertex);
}



int load_obj_model(const char *path, ModelData *model) {

    LoadingModelBuffer *buffer = malloc(sizeof(LoadingModelBuffer));

    int selectedMaterialId;
    PRINT_INFO("Loading model %s\n", path);
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
                buffer->vertex = realloc(buffer->vertex, sizeof(Vertex) * vim);
                POINTER_CHECK(buffer->vertex);
            }
            if (vni >= vnim) {
                
                vnim<<=1;
                buffer->normals = realloc(buffer->normals, sizeof(Normal) * vnim);
                POINTER_CHECK(buffer->normals);
            }
            if (vti >= vtim) {
                
                vtim<<=1;
                buffer->textureVertex = realloc(buffer->textureVertex, sizeof(TextureVertex) * vtim);
                POINTER_CHECK(buffer->textureVertex);
            }
            if (fi >= fim) {
                
                fim<<=1;
                buffer->faces = realloc(buffer->faces, sizeof(Face) * fim);
                object->facesVertex = realloc(object->facesVertex, sizeof(Vertex)*3 * fim);
                POINTER_CHECK(buffer->faces);
                POINTER_CHECK(object->facesVertex);
            }
        }
        symbol = getc(file);
        switch (symbol) {
        case 'v': ;
            symbol = getc(file);
            switch (symbol) {
                case ' ': ;
                    fscanf(file, "%f %f %f\n", &buffer->vertex[vi][0],&buffer->vertex[vi][1],&buffer->vertex[vi][2]);
                    vi++;
                    break;
                case 't': ;
                    fscanf(file, "%f %f\n", &buffer->textureVertex[vti][0],&buffer->textureVertex[vti][1]);
                    vti++;
                    break;
                case 'n': ;
                    fscanf(file, "%f %f %f\n", &buffer->normals[vni][0],&buffer->normals[vni][1],&buffer->normals[vni][2]);
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
            buffer->faces[fi].length = i;
            for (int j = 0; j < i; j++) {
                buffer->faces[fi].vertex[j] = vertexId[j] - offv;

                memcpy(&object->facesVertex[j+fi*3][VERTEX_ATTRIBUTE_POSITION_X], &buffer->vertex[vertexId[j] - offv], sizeof(float) * 3);
                memcpy(&object->facesVertex[j+fi*3][VERTEX_ATTRIBUTE_NORMAL_X], &buffer->normals[normalsId[j] - offvn], sizeof(float) * 3);
                memcpy(&object->facesVertex[j+fi*3][VERTEX_ATTRIBUTE_TEXTURE_U], &buffer->textureVertex[textureVertexId[j] - offvt], sizeof(float) * 2);

                buffer->faces[fi].normals[j] = normalsId[j] - offvn;
                buffer->faces[fi].textureVertex[j] = textureVertexId[j] - offvt;
            }
            
            vec2 deltaUV1;
            vec2 deltaUV2;
            glm_vec2_sub(&object->facesVertex[1+fi*3][VERTEX_ATTRIBUTE_TEXTURE_U], &object->facesVertex[fi*3+0][VERTEX_ATTRIBUTE_TEXTURE_U], deltaUV1);
            glm_vec2_sub(&object->facesVertex[2+fi*3][VERTEX_ATTRIBUTE_TEXTURE_U], &object->facesVertex[fi*3+0][VERTEX_ATTRIBUTE_TEXTURE_U], deltaUV2);
            vec3 edge1;
            vec3 edge2;
            glm_vec3_sub(buffer->vertex[vertexId[1]-offv], buffer->vertex[vertexId[0]-offv],edge1);
            glm_vec3_sub(buffer->vertex[vertexId[2]-offv], buffer->vertex[vertexId[0]-offv],edge2);

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
                memcpy(&object->facesVertex[fi*3+j][VERTEX_ATTRIBUTE_TANGENT_X], tangent, sizeof(tangent));
                memcpy(&object->facesVertex[fi*3+j][VERTEX_ATTRIBUTE_BITANGENT_X], bitangent, sizeof(bitangent));
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
        case 'u': ;
            
            char materialName[50];
            fscanf(file, "%*[^ ] %[^\n]", (char *) materialName);
            selectedMaterialId = find_material(model->materials, mc, materialName);
            break;
        case 'o': ;
            
            fscanf(file, "%*[^\n]");
            if (object) {
                close_realloc_obj(object, buffer, vi, fi, vni, vti);

                oi++;
                if (oi >= oim) {
                    oim<<=1;
                    model->objects = realloc(model->objects, sizeof(ModelObjectData) * oim);
                    POINTER_CHECK(model->objects);
                }
            }
            object = &(model->objects[oi]);
            offv += vi, offvt += vti, offvn += vni, fi = 0, vi = 0, vti = 0, vni = 0, fim = 30, vim = 50, vtim = 50, vnim = 50;
            malloc_obj(object, buffer, vim, fim, vnim, vtim);
            break;
        case '\n': ;
            break;
        case '#': ;
        default:
            fscanf(file, "%*[^\n]\n");
            break;
        }
    } while (symbol != -1);
    
    close_realloc_obj(object, buffer, vi, fi, vni, vti);
    free(buffer);
    oi++;
    model->objects = realloc(model->objects, sizeof(ModelObjectData) * oi);
    POINTER_CHECK(model->objects);
    model->length = oi;
    
    return 0;

}