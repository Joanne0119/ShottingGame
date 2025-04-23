#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "EnemyC.h"

EnemyC::EnemyC() : CEnemy()
{
    _vtxCount = 16;
    _indexCount = 42;
    _vtxAttrCount = 11;

    _points = new GLfloat[_vtxCount * _vtxAttrCount]{
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
        -0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.1f,  0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
    };
    _idx = new GLuint[_indexCount]{
        0, 1, 2,
        2, 3, 4,
        4, 5, 6,
        6, 7, 0,
        0, 2, 4,
        4, 6, 0,
        7, 7, 7,
        7, 7, 7,
        7, 7, 7,
        7, 7, 7,
        7, 7, 7,
        7, 7, 7,
        7, 7, 7,
        7, 7, 7};
    setupVertexAttributes();
};

EnemyC::~EnemyC()
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

void EnemyC::draw()
{
    CEnemy::draw();
}

void EnemyC::drawExplosion()
{
    CEnemy::drawExplosion();
}


void EnemyC::update(float dt)
{
    CEnemy::update(dt);
}



void EnemyC::reset()
{
    CEnemy::reset();
}
