#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "EnemyA.h"

EnemyA::EnemyA() : CEnemy()
{
    _vtxCount = 4;
    _indexCount = 6;
    _vtxAttrCount = 11;

    _points = new GLfloat[_vtxCount * _vtxAttrCount]{
        // ¶Ï∏m            // √C¶‚         // ™k¶V∂q       // ∂KπœÆyº–
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
    };
    _idx = new GLuint[_indexCount]{ 0, 1, 2, 2, 3, 0 };
    setupVertexAttributes();
};

EnemyA::~EnemyA()
{
    glDeleteBuffers(1, &_vbo);  //•˝ƒ¿©Ò VBO ªP EBO
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao); //¶Aƒ¿©Ò VAO
    if (_points != NULL) {
        delete[] _points;
        _points = nullptr;
    }
    if (_idx != NULL){
        delete[] _idx;
        _idx = nullptr;
    }
}

void EnemyA::draw()
{
    glUseProgram(_shaderProg);
    updateMatrix();
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}

void EnemyA::drawExplosion()
{
    CEnemy::drawExplosion();
}


void EnemyA::update(float dt)
{
    CEnemy::update(dt);
}



void EnemyA::reset()
{
    CEnemy::reset();
}
