#pragma once
#include <glm/glm.hpp>
#include "Cplayer.h"
#include "CQuad.h"

class CPlayerShield : public CPlayer {
public:
    CPlayerShield(GLint shaderProg);
    CPlayerShield(GLint shaderProg, CPlayer* target, GLfloat dx, GLfloat radius);
    ~CPlayerShield();
    virtual void update();
    virtual void draw();
private:
    CPlayer* _targetPlayer;
    CQuad* _shield;
    int _number;
    float _angle;
    GLfloat _dx;
    GLfloat _radius;
    glm::vec3 _col;
    glm::vec3 _scal;
};
