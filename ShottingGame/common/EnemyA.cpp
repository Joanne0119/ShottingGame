#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "EnemyA.h"

EnemyA::EnemyA() : CEnemy()
{
    _angle = 0.0f;
    _vtxCount = 16;
    _indexCount = 42;
    _vtxAttrCount = 11;

    _points = new GLfloat[_vtxCount * _vtxAttrCount]{
        0.0f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.1f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.35f, 0.15f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.85f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.6f,  -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.3f,  -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.0f,  -0.7f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.3f,  -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.6f,  -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.85f,  0.45f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.35f,  0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.1f,  0.2f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.05f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.05f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
        
    };
    _idx = new GLuint[_indexCount]{
        0, 1, 11,
        1, 12, 11,
        12, 13, 14,
        2, 3, 4,
        2, 5, 4,
        2, 5, 1,
        1, 5, 14,
        14, 6, 5,
        6, 13, 14,
        6, 7, 13,
        7, 11, 13,
        7, 10, 11,
        7, 8, 10,
        8, 9, 10
    };
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
    CEnemy::draw();
}

void EnemyA::drawExplosion()
{
    CEnemy::drawExplosion();

}


void EnemyA::update(float dt)
{
    CEnemy::update(dt);
}

void EnemyA::move(float dt)
{
    _pos.x += _dirX * dt;
    if (_pos.x < -4.0f || _pos.x > 4.0f) {
        _dirX = -_dirX;  // 到邊界反向
        _pos.x = std::clamp(_pos.x, -4.0f, 4.0f);
    }

    _angle += dt * 5.0f; // 控制 Y 軸波動頻率
    _pos.y = sin(_angle) * 0.5f + 2.5f; // 在 Y = 2.5 的上下區間波動
    
}

void EnemyA::shoot(){
    CEnemy::shoot();
}


void EnemyA::reset()
{
    CEnemy::reset();
}
