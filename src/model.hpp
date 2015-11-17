#ifndef MODEL_HPP
#define MODEL_HPP

// This includes more headers than you will probably need.
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <utility> // pair
#include <list>
#include <vector>
#include <array>
#include <math.h>
#include <limits>
#include <fstream>
#include <algorithm> //swap
#include <cassert>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "util.hpp"

struct Vertex {
    GLfloat coord[3];
    GLfloat normal[3];
    GLfloat rgb[3];
};

typedef int ModelID;
static ModelID next_model_id = 0;
const ModelID registerNewModelID();

// static const unsigned int model_name_buffer_size = 256;
// class ModelName {
// private:
//     static ModelID max_id;
//     static unordered_map<ModelID, ModelName*> registered_models;
//     char name[model_name_buffer_size];

//     explicit ModelName();
//     explicit ModelName(char* name);
//     ~ModelName();

// public:
//     static ModelID registerModelName(char* name);
//     static ModelName* getModelName(ModelID id);

//     const bool operator==(const ModelName& rhs) const;
//     const bool operator==(const char* name) const;
//     const bool operator!=(const ModelName& rhs) const;
//     const bool operator!=(const char* name) const;
// };

// char terrain_block_model_name[model_name_buffer_size] = "terrain";
// char core_model_name[model_name_buffer_size] = "core";

// static const ModelID terrain_block_model_id =
//     ModelName::registerModelName(terrain_block_model_name);
// static const ModelID core_model_id =
//     ModelName::registerModelName(core_model_name);

// FIX HERE
static const ModelID terrain_block_model_id = 0;
static const ModelID core_model_id = 1;

class Model {
private:
    static unordered_map<ModelID, Model*> models;

    GLuint vbo_vertex_id;
    GLuint vbo_index_id;

    GLuint v_count;

    Vertex* vertices;
    GLuint* indices;

    vector<GLuint> tes_group_begin;

    explicit Model();
    ~Model();

    void genModelBRUTE(ModelID model_id);
    void genTerrainBlockModelBRUTE();
    void genCoreModelBRUTE();

public:
    static Model* createModel(ModelID model_id);
    static Model* getModel(ModelID model_id);
    static void deleteModel(ModelID model_id);
    static bool exists(ModelID model_id);

    GLuint getVBOVertexID();
    GLuint getVBOIndexID();
    GLuint getVCount();
    Vertex* getVertices();
    GLuint* getIndices();
    vector<GLuint>& getTesGroupBegin();
};


#endif