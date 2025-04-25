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
        // ¶Ï∏m            // √C¶‚         // ™k¶V∂q       // ∂KπœÆyº–
        -0.5f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
        -0.3f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.3f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.5f, 0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.35f,  0.18f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.6f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.4f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.5f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.1f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  -0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.1f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.5f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.4f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.6f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.35f,  0.18f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
        
    };
    _idx = new GLuint[_indexCount]{
        0, 1, 14,
        1, 14, 15,
        1, 15, 2,
        2, 15, 4,
        2, 3, 4,
        4, 5, 6,
        4, 6, 8,
        6, 8, 7,
        8, 10, 15,
        8, 9, 10,
        10, 11, 12,
        10, 12, 14,
        12, 13, 14,
        12, 13, 14
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
