#pragma once
#include <glm/glm.hpp>
#include <list>
#include "CMissile.h"

namespace EnemyState {
    enum State { Alive, Hurt, Exploding, Dead };
}

class CEnemy {
public:
    CEnemy();
    virtual ~CEnemy();
    void setupVertexAttributes();
    GLuint setShader(const char* vshader, const char* fshader);
    void setShaderID(GLuint shaderID);
    virtual void draw();
    virtual void drawExplosion();
    virtual void update(float dt);
    virtual void move(float dt); 
    virtual void reset();
    void setColor(glm::vec3 vColor); // 設定模型的顏色
    void setScale(glm::vec3 vScale); // 設定模型的縮放比
    void setPos(glm::vec3 vPt); // 設定模型的位置
    void setRotX(float angle); // 設定模型的X軸旋轉角度
    void setRotY(float angle); // 設定模型的Y軸旋轉角度
    void setRotZ(float angle); // 設定模型的Z軸旋轉角度
    void setTransformMatrix(glm::mat4 mxMatrix);
    void updateMatrix();
    glm::mat4 getModelMatrix();
    GLuint getShaderProgram();
    glm::vec3 getPos() const;
    
    //Enemy Action
    virtual void onHit(int damage);
    bool isDead() const;
    void setState(EnemyState::State state);
    EnemyState::State getState() const;
    //shoot
    virtual void shoot();
    std::list<CMissile*>& getMissiles(); // 回傳子彈清單
    void printMissiles();
    //hp
    int getHp();
    void setHp(int hp);
    

protected:
    GLfloat* _points;
    GLuint* _idx;
    GLuint _vao, _vbo, _ebo;
    GLuint _shaderProg, _rotAxis;
    GLint _modelMxLoc;
    bool _bRotation, _bScale, _bPos;
    bool _bTransform, _bOnTransform;
    // _bTransform : true 代表有設定新的轉換矩陣
    // _bOnTransform : true 代表曾經設定過轉換矩陣，用於判斷是否需要更新 model matrix
    int _vtxCount, _indexCount, _vtxAttrCount;
    
    //Exploding
    int _explosionvtxCount;
    int _explosionidxCount;
    int _explosionvtxAttrCount;
    GLfloat* _explosionPoints;
    GLuint* _explosionIdx;
    
    //Enemy
    bool _isDead;
    float _speed;
    int _hp;
    float _dirX;
    float _explosionTimer;
    EnemyState::State _state;
    //shoot
    std::list<CMissile*> _missiles;
    GLfloat _fireRate;
    GLfloat _fireCooldown;
    
    glm::vec3 _color;
    glm::vec3 _scale;    // 模型的縮放值
    glm::vec3 _pos;        // 模型的位置
    GLfloat _rotX, _rotY, _rotZ; // 模型的旋轉角度
    glm::mat4 _mxRotX, _mxRotY, _mxRotZ, _mxRotation, _mxScale, _mxPos, _mxTRS;
    glm::mat4 _mxTransform, _mxFinal;
};
