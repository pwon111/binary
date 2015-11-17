#include "model.hpp"

/****************************** ModelID Controller ****************************/

const ModelID registerNewModelID() {
    return next_model_id++;
}

/******************************* ModelName Struct *****************************/

// ModelID ModelName::max_id = 0;
// unordered_map<ModelID, ModelName*> ModelName::registered_models;

// ModelID ModelName::registerModelName(char* name) {
//     assert(max_id >= 0);
//     assert(registered_models.find(max_id) == registered_models.end());

//     ModelName* new_model_name = new ModelName(name);
//     registered_models[max_id] = new_model_name;
//     max_id++;
//     return max_id - 1;
// }

// ModelName* ModelName::getModelName(ModelID id) {
//     assert(registered_models.find(id) != registered_models.end());

//     return registered_models[id];
// }

// ModelName::ModelName(char* name) : name() {
//     for (int i = 0; i < model_name_buffer_size; i++) {
//         this->name[i] = name[i];
//     }
// }

// ModelName::~ModelName() {

// }

// const bool ModelName::operator==(const ModelName& rhs) const {
//     for (unsigned int i = 0; i < model_name_buffer_size; i++) {
//         if (this->name[i] == rhs.name[i]) {
//             return false;
//         }
//     }
//     return true;
// }
// const bool ModelName::operator==(const char* name) const {
//     for (unsigned int i = 0; i < model_name_buffer_size; i++) {
//         if (this->name[i] == name[i]) {
//             return false;
//         }
//     }
//     return true;
// }
// const bool ModelName::operator!=(const ModelName& rhs) const {
//     for (unsigned int i = 0; i < model_name_buffer_size; i++) {
//         if (this->name[i] != rhs.name[i]) {
//             return true;
//         }
//     }
//     return false;
// }
// const bool ModelName::operator!=(const char* name) const {
//     for (unsigned int i = 0; i < model_name_buffer_size; i++) {
//         if (this->name[i] != name[i]) {
//             return true;
//         }
//     }
//     return false;
// }

/********************************* Model Class ********************************/

unordered_map<ModelID, Model*> Model::models;

Model::Model() :
    vbo_vertex_id(),
    vbo_index_id(),
    v_count(),
    vertices(NULL),
    indices(NULL),
    tes_group_begin()
{
    //
}

Model::~Model() {
    delete[] this->vertices;
    delete[] this->indices;
}


GLuint Model::getVBOVertexID() {
    return this->vbo_vertex_id;
}
GLuint Model::getVBOIndexID() {
    return this->vbo_index_id;
}
GLuint Model::getVCount() {
    return this->v_count;
}
Vertex* Model::getVertices() {
    return this->vertices;
}
GLuint* Model::getIndices() {
    return this->indices;
}
vector<GLuint>& Model::getTesGroupBegin() {
    return this->tes_group_begin;
}

Model* Model::createModel(ModelID model_id) {
    if (models.find(model_id) == models.end()) {
        printf("%d BLAH\n", model_id);
        Model* new_model = new Model();
        new_model->genModelBRUTE(model_id);
        models[model_id] = new_model;
    } else {
        fprintf(stderr,
            "ERROR Model::create model for ModelID %d already exists\n",
            model_id);
    }
}

Model* Model::getModel(ModelID model_id) {
    if (models.find(model_id) != models.end()) {
        return models[model_id];
    } else {
        fprintf(stderr,
            "ERROR Model::get model for ModelID %d does not exist\n",
            model_id);
    }
}

void Model::deleteModel(ModelID model_id) {
    if (models.find(model_id) != models.end()) {
        delete models[model_id];
        // FIX HERE: remove setup for vbo if setup?
    } else {
        fprintf(stderr,
            "ERROR Model::delete model for ModelID %d does not exist\n",
            model_id);
    }
}

bool Model::exists(ModelID model_id) {
    return models.find(model_id) != models.end();
}

void Model::genModelBRUTE(ModelID model_id) {
    printf("%d %d\n", terrain_block_model_id, core_model_id);
    if (model_id == terrain_block_model_id) {
        this->genTerrainBlockModelBRUTE();
    } else if (model_id == core_model_id) {
        this->genCoreModelBRUTE();
    } else {
        printf("NOT READY FOR THIS MODEL ID YET\n");
        return;
    }
    // set up vertex VBO
    glGenBuffers(1, &(this->vbo_vertex_id));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertex_id);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex) * this->v_count,
        this->vertices,
        GL_STATIC_DRAW);
    // set up index VBO
    glGenBuffers(1, &(this->vbo_index_id));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_index_id);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * this->v_count,
        this->indices,
        GL_STATIC_DRAW);
}







void Model::genTerrainBlockModelBRUTE() {
    this->v_count = 4;
    this->vertices = new Vertex[this->v_count];
    this->indices = new GLuint[this->v_count];

    // SW corner
    this->vertices[0].coord[0] = -0.45;
    this->vertices[0].coord[1] = -0.45;
    this->vertices[0].coord[2] = 0;
    this->vertices[0].normal[0] = 0.0;
    this->vertices[0].normal[1] = 0.0;
    this->vertices[0].normal[2] = 1.0;
    this->vertices[0].rgb[0] = 1.0;
    this->vertices[0].rgb[1] = 0.0;
    this->vertices[0].rgb[2] = 0.0;
    this->indices[0] = 0;
    // NW corner
    this->vertices[1].coord[0] = -0.45;
    this->vertices[1].coord[1] = 0.45;
    this->vertices[1].coord[2] = 0.0;
    this->vertices[1].normal[0] = 0.0;
    this->vertices[1].normal[1] = 0.0;
    this->vertices[1].normal[2] = 1.0;
    this->vertices[1].rgb[0] = 1.0;
    this->vertices[1].rgb[1] = 0.0;
    this->vertices[1].rgb[2] = 0.0;
    this->indices[1] = 1;
    // SE corner
    this->vertices[2].coord[0] = 0.45;
    this->vertices[2].coord[1] = -0.45;
    this->vertices[2].coord[2] = 0.0;
    this->vertices[2].normal[0] = 0.0;
    this->vertices[2].normal[1] = 0.0;
    this->vertices[2].normal[2] = 1.0;
    this->vertices[2].rgb[0] = 1.0;
    this->vertices[2].rgb[1] = 0.0;
    this->vertices[2].rgb[2] = 0.0;
    this->indices[2] = 2;
    // NE corner
    this->vertices[3].coord[0] = 0.45;
    this->vertices[3].coord[1] = 0.45;
    this->vertices[3].coord[2] = 0.0;
    this->vertices[3].normal[0] = 0.0;
    this->vertices[3].normal[1] = 0.0;
    this->vertices[3].normal[2] = 1.0;
    this->vertices[3].rgb[0] = 1.0;
    this->vertices[3].rgb[1] = 0.0;
    this->vertices[3].rgb[2] = 0.0;
    this->indices[3] = 3;
    // update model.roup_begin by marking the first index of this block
    this->tes_group_begin.push_back(0);
}

void Model::genCoreModelBRUTE() {
    this->v_count = 4;
    this->vertices = new Vertex[this->v_count];
    this->indices = new GLuint[this->v_count];

    // SW corner
    this->vertices[0].coord[0] = -0.4;
    this->vertices[0].coord[1] = -0.4;
    this->vertices[0].coord[2] = 0.5;
    this->vertices[0].normal[0] = 0.0;
    this->vertices[0].normal[1] = 0.0;
    this->vertices[0].normal[2] = 1.0;
    this->vertices[0].rgb[0] = 0.0;
    this->vertices[0].rgb[1] = 1.0;
    this->vertices[0].rgb[2] = 0.0;
    this->indices[0] = 0;
    // NW corner
    this->vertices[1].coord[0] = -0.3;
    this->vertices[1].coord[1] = 0.4;
    this->vertices[1].coord[2] = 0.5;
    this->vertices[1].normal[0] = 0.0;
    this->vertices[1].normal[1] = 0.0;
    this->vertices[1].normal[2] = 1.0;
    this->vertices[1].rgb[0] = 0.0;
    this->vertices[1].rgb[1] = 1.0;
    this->vertices[1].rgb[2] = 0.0;
    this->indices[1] = 1;
    // SE corner
    this->vertices[2].coord[0] = 0.4;
    this->vertices[2].coord[1] = -0.4;
    this->vertices[2].coord[2] = 0.5;
    this->vertices[2].normal[0] = 0.0;
    this->vertices[2].normal[1] = 0.0;
    this->vertices[2].normal[2] = 1.0;
    this->vertices[2].rgb[0] = 0.0;
    this->vertices[2].rgb[1] = 1.0;
    this->vertices[2].rgb[2] = 0.0;
    this->indices[2] = 2;
    // NE corner
    this->vertices[3].coord[0] = 0.3;
    this->vertices[3].coord[1] = 0.4;
    this->vertices[3].coord[2] = 0.5;
    this->vertices[3].normal[0] = 0.0;
    this->vertices[3].normal[1] = 0.0;
    this->vertices[3].normal[2] = 1.0;
    this->vertices[3].rgb[0] = 0.0;
    this->vertices[3].rgb[1] = 1.0;
    this->vertices[3].rgb[2] = 0.0;
    this->indices[3] = 3;

    this->tes_group_begin.push_back(0);
}