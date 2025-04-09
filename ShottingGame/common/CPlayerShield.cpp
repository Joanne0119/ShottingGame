#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "Cplayer.h"
#include "CQuad.h"
#include "CPlayerShield.h"
#include "initshader.h"

CPlayerShield::CPlayerShield(GLint shaderProg, CPlayer* target, GLfloat dx, GLfloat radius):CPlayer(shaderProg)
{
    _targetPlayer = target;
    _dx = dx;
    _radius = radius;
    
    _shield = new CQuad;
    _shield->setShaderID(shaderProg);
    _shield->setPos(getPos());
    _shield->setColor(_col);
    _shield->setScale(_scal);
};

CPlayerShield::~CPlayerShield(){
    if(_shield != nullptr) delete _shield;
}

void CPlayerShield::update() {
    if (_targetPlayer) {
        glm::vec3 center = _targetPlayer->getPos();
        float xAngle = 3.1415916f*_dx/2.0f;
        
        glm::mat4 mxMove = glm::translate(glm::mat4(1.0f), glm::vec3(_dx, 0.0f, 0.0f));
        
        glm::mat4 mxGRot = glm::rotate(glm::mat4(1.0f), xAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        
        glm::mat4 translateZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 1.0f));

        
        glm::mat4 mxTransform = mxMove * translateZ * mxGRot ;
        
//        for (int i = 0; i < 12; i++){ g_quad_shild[i].setTransformMatrix(mxTransform);
//        }
        
    }
}

void CPlayerShield::draw() {
    // 呼叫 base class 的 draw 或自己畫
    CPlayer::draw();
}
