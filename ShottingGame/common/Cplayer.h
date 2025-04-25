#pragma once
#include <glm/glm.hpp>
#include <list>
#include "CTrapezid.h"
#include "CTriangle.h"
#include "CPenta.h"
#include "CMissile.h"

namespace PlayerState {
    enum State { Alive, Hurt1, Hurt2, Dead };
    enum Style { Style1, Style2, Style3 };
}


class CPlayer
{
public:
    CPlayer(GLint shaderProg);
    ~CPlayer();
    void draw();
    void drawHurt1();
    void drawHurt2();
    void reset();
    void setColor(glm::vec3 vTopColor, glm::vec3 vBodyColor, glm::vec3 vWingsColor , glm::vec3 vWindowColor ,glm::vec3 vBottomColor, glm::vec3 vFireColor); // 設定模型的顏色
    void setScale(glm::vec3 vTopScale, glm::vec3 vBodyScale, glm::vec3 vWingsScale, glm::vec3 vWindowScale, glm::vec3 vBottomScale, glm::vec3 vFireScale); // 設定模型的縮放比
    void setScale(glm::vec3 vScale);
    void setPos(glm::vec3 vTopPos, glm::vec3 vBodyPos, glm::vec3 vWingsPos, glm::vec3 vWindowPos,glm::vec3 vBottomPos, glm::vec3 vFirePos1, glm::vec3 vFirePos2); // 設定模型的位置
    void setPos(glm::vec3 vPos);
    void setRotX(float angle); // 設定模型的X軸旋轉角度
    void setRotY(float angle); // 設定模型的Y軸旋轉角度
    void setRotZ(float angle); // 設定模型的Z軸旋轉角度
    void setTransformMatrix(glm::mat4 mxMatrix);
    glm::vec3 getPos();
    glm::mat4 getModelMatrix();
    GLuint getShaderProgram();
    void update(float dt);
    void shoot(); 
    void move();
    std::list<CMissile*>& getMissiles(); // 回傳子彈清單
    void printMissiles();
    
    void onHit(int damage);
    bool isDead() const;
    void setState(PlayerState::State state);
    PlayerState::State getState() const;
    int getHp();

private:
    //action
    bool _isDead;
    int _hp;
    int _maxHp;
    PlayerState::State _state;
    float _hurtTimer;
    
    CTriangle* top;
    CTrapezid* body;
    CTrapezid* wings;
    CTrapezid* window;
    CTriangle* bottom;
    CTriangle* fire;
    std::list<CMissile*> _missiles;
    GLint _modelMxLoc;
    GLuint _shaderProg;
    GLfloat _fireRate;
    GLfloat _fireCooldown;
    bool _bRotation, _bScale, _bPos;
    bool _bTransform, _bOnTransform;
    // _bTransform : true 代表有設定新的轉換矩陣
    // _bOnTransform : true 代表曾經設定過轉換矩陣，用於判斷是否需要更新 model matrix
    //color
    glm::vec3 _topColor;
    glm::vec3 _bodyColor;
    glm::vec3 _wingsColor;
    glm::vec3 _windowColor;
    glm::vec3 _bottomColor;
    glm::vec3 _fireColor;
    //scale
    glm::vec3 _scale;
    glm::vec3 _topScale;
    glm::vec3 _bodyScale;
    glm::vec3 _wingsScale;
    glm::vec3 _windowScale;
    glm::vec3 _bottomScale;
    glm::vec3 _fireScale;
    //pos
    glm::vec3 _pos;
    glm::vec3 _topPos;
    glm::vec3 _bodyPos;
    glm::vec3 _wingsPos;
    glm::vec3 _windowPos;
    glm::vec3 _bottomPos;
    glm::vec3 _firePos[2];
    //rot
    GLfloat _topRotX, _topRotY, _topRotZ;
    GLfloat _bodyRotX, _bodyRotY, _bodyRotZ;
    GLfloat _wingsRotX, _wingsRotY, _wingsRotZ;
    GLfloat _windowRotX, _windowRotY, _windowRotZ;
    GLfloat _bottomRotX, _bottomRotY, _bottomRotZ;
    GLfloat _fireRotX, _fireRotY, _fireRotZ;
    glm::mat4 _mxRotX, _mxRotY, _mxRotZ, _mxRotation, _mxScale, _mxPos, _mxTRS;
    glm::mat4 _mxTransform, _mxFinal;
    void updateMatrix();
    
};
