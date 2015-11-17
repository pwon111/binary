#include "renderer.hpp"

void renderBRUTE(Model* model) {
    glPushMatrix();
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, model->getVBOVertexID());
    glVertexPointer(
        3,              // size
        GL_FLOAT,       // type
        sizeof(Vertex), // stride
        0);             // pointer offset
                        // ^ pointer offset instead of pointer for VBOs
    glNormalPointer(
        GL_FLOAT,                           // type
        sizeof(Vertex),                     // stride
        (GLvoid*) (sizeof(GLfloat) * 3));   // pointer offset
                                            // assumes padding bytes = 0
    glColorPointer(
        3,                                  // size
        GL_FLOAT,                           // type
        sizeof(Vertex),                     // stride
        (GLvoid*) (sizeof(GLfloat) * 6));   // pointer offset
                                            // assumes padding bytes = 0
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getVBOIndexID());
    glIndexPointer(
        GL_UNSIGNED_INT,        // type
        0,                      // stride
        0);                     // pointer offset

    // actually draw
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    vector<GLuint>& tes_group_begin = model->getTesGroupBegin();
    for (vector<GLuint>::iterator it = tes_group_begin.begin();
        it != tes_group_begin.end();
        it++)
    {
        glDrawElements(
            GL_TRIANGLE_STRIP,      // type
            4,                      // size
            GL_UNSIGNED_INT,        // type of index
            (GLuint*) 0 + *it);     // pointer
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopMatrix();
}

void renderWireBRUTE(Model* model) {
    glPushMatrix();
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, model->getVBOVertexID());
    glVertexPointer(
        3,              // size
        GL_FLOAT,       // type
        sizeof(Vertex), // stride
        0);             // pointer offset
                        // ^ pointer offset instead of pointer for VBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getVBOIndexID());

    vector<GLuint>& tes_group_begin = model->getTesGroupBegin();
    // actually draw
    glEnableClientState(GL_VERTEX_ARRAY);

    for (vector<GLuint>::iterator it = tes_group_begin.begin();
        it != tes_group_begin.end();
        it++)
    {
        glDrawElements(
            GL_TRIANGLE_STRIP,      // type
            4,                      // size
            GL_UNSIGNED_INT,        // type of index
            (GLuint*) 0 + *it);     // pointer
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopMatrix();
}