#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib> 

#include "EnemyC.h"

EnemyC::EnemyC() : CEnemy()
{
    _angle = 0.0f;
    _vtxCount = 16;
    _indexCount = 42;
    _vtxAttrCount = 11;

    _points = new GLfloat[_vtxCount * _vtxAttrCount]{
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
        -0.14f, 0.35f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.14f, 0.35f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.0f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         -0.15f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.15f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         -0.4f,  -0.17f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.03f,  -0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.03f,  -0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.4f,  -0.17f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  -0.2f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.25f,  -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.18f,  -0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.18f,  -0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.25f,  -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
    };
    _idx = new GLuint[_indexCount]{
        0, 1, 2,
        1, 2, 3,
        4, 5, 3,
        4, 6, 7,
        4, 5, 7,
        5, 7, 8,
        5, 8, 9,
        7, 8, 10,
        6, 12, 10,
        10, 12, 14,
        9, 10, 14,
        6, 11, 12,
        12, 13, 14,
        9, 14, 15};
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

void EnemyC::move(float dt)
{
    _angle += 3.0f * dt;  // 每幀旋轉
    _pos.y -= _speed * dt;
    _pos.x += cos(_angle) * 0.05f;
    
    if (_pos.y < -3.5f) {
        _pos.y = 3.5f;
        _pos.x = (rand() % 900 - 450) / 100.0f;  // -4.5 到 4.5 隨機 X
    }
}

void EnemyC::reset()
{
    CEnemy::reset();
}
