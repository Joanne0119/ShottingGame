#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "CEnemy.h"

#include "initshader.h"

CEnemy::CEnemy()
{
    _vao = 0; _vbo = 0; _ebo = 0;
    _shaderProg = 0;
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _color = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
    _rotAxis = 0;
    _bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
    _mxScale = glm::mat4(1.0f);
    _mxPos = glm::mat4(1.0f);
    _mxRotX = glm::mat4(1.0f);
    _mxRotY = glm::mat4(1.0f);
    _mxRotZ = glm::mat4(1.0f);
    _mxRotation = glm::mat4(1.0f);
    _mxTRS = glm::mat4(1.0f);
    _mxTransform = glm::mat4(1.0f);
    _mxFinal = glm::mat4(1.0f);
    _vtxCount = 0;
    _indexCount = 0;
    _vtxAttrCount = 0;
    _isDead = false;
    _speed = 1.5f;
    _hp = 1;
    _state = Alive;
    _explosionTimer = 0;
    _dirX = (rand() % 2 == 0 ? 1.0f : -1.0f) * (0.5f + rand() % 100 / 100.0f);
    _points = nullptr;
    _idx = nullptr;
    _fireRate = 0.8f;
    _fireCooldown = 0.0f;
    
//    _explosionvtxCount = 16;
//    _explosionidxCount = 48;
    _explosionvtxAttrCount = 11;
    _explosionvtxCount = 8;
    _explosionidxCount = 18;
   
    _explosionPoints = new GLfloat[_explosionvtxCount * _explosionvtxAttrCount]{
//        // ¶Ï∏m            // √C¶‚         // ™k¶V∂q       // ∂KπœÆyº–
//        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
//        -0.35f, -0.35f,  0.00f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
//        -0.10f, -0.50f,  0.00f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
//        0.10f, -0.50f,  0.00f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
//        0.35f, -0.35f,  0.00f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
//        0.50f,  0.00f,  0.00f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
//        0.35f,  0.35f,  0.00f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
//        0.10f,  0.50f,  0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
//        -0.10f,  0.50f,  0.00f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
//        -0.35f,  0.35f,  0.00f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
//        -0.50f,  0.00f,  0.00f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
//        -0.35f, -0.35f,  0.00f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
//        0.10f, -0.50f,  0.00f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
//        0.35f, -0.35f,  0.00f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
//        0.50f,  0.00f,  0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
//        0.35f,  0.35f,  0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
        
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
        -0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.1f,  0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
    };

    _explosionIdx = new GLuint[_explosionidxCount]{ 
//        0, 1, 8,
//        1, 2, 9,
//        2, 3, 10,
//        3, 4, 11,
//        4, 5, 12,
//        5, 6, 13,
//        6, 7, 14,
//        7, 8, 15,
//        8, 9, 0,
//        9, 10, 1,
//        10, 11, 2,
//        11, 12, 3,
//        12, 13, 4,
//        13, 14, 5,
//        14, 15, 6,
//        15, 0, 7
        0, 1, 7, 1 ,2 ,3, 3, 4, 5, 5, 6, 7, 7, 3, 5, 7, 1, 3
    };
    
    setupVertexAttributes();

}

CEnemy::~CEnemy()
{
    glDeleteBuffers(1, &_vbo);  //•˝ƒ¿©Ò VBO ªP EBO
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao); //¶Aƒ¿©Ò VAO
    glDeleteProgram(_shaderProg);  //ƒ¿©Ò shader program
    if (_points != NULL) {
        delete[] _points;
        _points = nullptr;
    }
    if (_idx != NULL){
        delete[] _idx;
        _idx = nullptr;
    }
    if (_explosionPoints != nullptr) {
        delete[] _explosionPoints;
        _explosionPoints = nullptr;
    }
    if (_explosionIdx != nullptr) {
        delete[] _explosionIdx;
        _explosionIdx = nullptr;
    }
}

void CEnemy::setupVertexAttributes()
{
    // ≥]©w VAO°BVBO ªP EBO
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(_vao);

    // ≥]©w VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vtxCount * _vtxAttrCount * sizeof(_points), _points, GL_STATIC_DRAW);

    // ≥]©w EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(GLuint), _idx, GL_STATIC_DRAW);

    // ¶Ï∏mƒ›©
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // √C¶‚ƒ›©
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //™k¶V∂qƒ›©
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //∂KπœÆyº–ƒ›©
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0); // ∏—∞£πÔ VAO ™∫∏j©w
}

GLuint CEnemy::setShader(const char* vshader, const  char* fshader)
{
    _shaderProg = createShader(vshader, fshader);
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
    return _shaderProg;
}

void CEnemy::setShaderID(GLuint shaderID)
{
    _shaderProg = shaderID;
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void CEnemy::setColor(glm::vec3 vColor)
{
    _color = vColor;
    for (int i = 0; i < _vtxCount; i++) {
        _points[i * _vtxAttrCount + COLOR_OFFSET ]  = _color.x;
        _points[i * _vtxAttrCount + COLOR_OFFSET+1] = _color.y;
        _points[i * _vtxAttrCount + COLOR_OFFSET+2] = _color.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vtxCount * _vtxAttrCount * sizeof(_points), _points, GL_STATIC_DRAW);
}

void CEnemy::draw()
{
    if (_state == Dead) return;

    if (_state == Exploding) {
        // 你可以畫個簡單的爆炸或先用不同顏色表示
        drawExplosion();
        return;
    }
    else if(_state == Alive){
        // Alive 狀態畫正常模型
        updateMatrix();
        glUseProgram(_shaderProg);
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
        
        for (auto it = _missiles.begin(); it != _missiles.end(); ++it) {
            (*it)->draw();
        }
    }
    
}

void CEnemy::drawExplosion() {
    glUseProgram(_shaderProg);
    updateMatrix();
    glBindVertexArray(_vao);

    // 替換 VBO 資料
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * _explosionvtxCount * _explosionvtxAttrCount, _explosionPoints);

    // 替換 EBO 資料（其實不變也可以）
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * _explosionidxCount, _explosionIdx);

    glDrawElements(GL_TRIANGLES, _explosionidxCount, GL_UNSIGNED_INT, 0);
}

void CEnemy::update(float dt){
    _fireCooldown -= dt;
    
    for (auto it = _missiles.begin(); it != _missiles.end(); ) {
        (*it)->updateEnemy(dt);
        if ((*it)->isOutOfBounds()) {
            delete *it;               // 釋放記憶體
            it = _missiles.erase(it); // 移除元素
        } else {
            ++it;
        }
    }

    if (_state == Dead) return;

    if (_state == Exploding) {
        _explosionTimer -= dt;
        if (_explosionTimer <= 0) {
            _state = Dead;
        }
        return; // 爆炸中不更新移動
    }

    // Alive 狀態的移動邏輯
    if (_pos.y > 3.0f) {
        _pos.y -= _speed * dt;
    } else {
        _pos.x += _dirX * dt;
        if (_pos.x < -5.0f || _pos.x > 5.0f) {
            _dirX = -_dirX;
        }
    }
    if (_fireCooldown <= 0.0f) {
        shoot();
        printMissiles();
    }

    setPos(_pos);
}

void CEnemy::shoot() {
    if (_fireCooldown <= 0.0f) {
        auto missile = new CMissile(_pos - glm::vec3(0.0f, 0.5f, 0.0f), 5.0f);
        missile->setShaderID(_shaderProg);
        missile->setColor(glm::vec3(0.5f, 0.3f, 0.3f));
        missile->setScale(glm::vec3(0.1f,0.1f,0.1f));
        missile->setRotY(180);
        _missiles.push_back(missile);
        _fireCooldown = _fireRate;
    }
}

std::list<CMissile*>& CEnemy::getMissiles() {
    return _missiles;
}

void CEnemy::printMissiles() {
    for (auto* missile : _missiles) {
        glm::vec3 position = missile->getPos();
        std::cout << "Missile position: ("
                  << position.x << ", " << position.y << ", " << position.z << ")\n";
    }
}


void CEnemy::setState(State state) { _state = state; }
State CEnemy::getState() const { return _state; }

bool CEnemy::isDead() const{
    return _isDead;
}

void CEnemy::onHit(int damage){
    if (_isDead) return;
    _hp -= damage;
    if (_hp <= 0) {
        _isDead = true;  
        _state = Exploding;
        _explosionTimer = 1.5;
    }
}

void CEnemy::setScale(glm::vec3 vScale)
{
    _scale = vScale;
    _bScale = true;
    _mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CEnemy::setPos(glm::vec3 vPt)
{
    _pos = vPt;
    _bPos = true;
    _mxPos = glm::translate(glm::mat4(1.0f), _pos);
}

void CEnemy::setRotX(float angle)
{
    _rotX = glm::radians(angle);
    _rotAxis = _rotAxis | 1;
    _mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    _mxRotation = _mxRotX;
    _bRotation = true;
}

void CEnemy::setRotY(float angle)
{
    _rotY = glm::radians(angle);
    _rotAxis = _rotAxis | 2;
    _mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
    if( _rotAxis & 1 )_mxRotation = _mxRotY * _mxRotX;  // ¶≥ X ∂b™∫±€¬‡∂q
    else _mxRotation = _mxRotY ;
    _bRotation = true;
}

void CEnemy::setRotZ(float angle)
{
    _rotZ = glm::radians(angle);
    _mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    if ( _rotAxis == 1 ) _mxRotation = _mxRotZ * _mxRotX; // •u¶≥ X ∂b™∫±€¬‡∂q
    else if ( _rotAxis == 2 ) _mxRotation = _mxRotZ * _mxRotY; // •u¶≥ Y ∂b™∫±€¬‡∂q
    else if ( _rotAxis == 3 ) _mxRotation = _mxRotZ * _mxRotY * _mxRotX; // ¶≥ X°BY ∂b™∫±€¬‡∂q
    else _mxRotation = _mxRotZ;
    _bRotation = true;
}

void CEnemy::updateMatrix()
{
    if (_bScale || _bPos || _bRotation)
    {
        if (_bOnTransform == false) {
            _mxFinal = _mxTRS = _mxPos * _mxRotation * _mxScale;
        }
        else {
            _mxTRS = _mxPos * _mxRotation * _mxScale;
            _mxFinal = _mxTransform * _mxTRS;
        }
        _bScale = _bPos = _bRotation = false;
    }
    if (_bTransform) {
        _mxFinal = _mxTransform * _mxTRS;
        _bTransform = false;
    }
    // ¶p¶h≠”º“´¨®œ•Œ¨€¶P™∫ shader program,¶]®C§@≠”º“´¨™∫ mxTRS ≥£§£¶P°A©“•H®C≠”frame≥£≠nßÛ∑s
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));
}

void CEnemy::setTransformMatrix(glm::mat4 mxMatrix)
{
    _bOnTransform = _bTransform = true;
    _mxTransform = mxMatrix;
}

glm::mat4 CEnemy::getModelMatrix() { return _mxFinal; }

GLuint CEnemy::getShaderProgram() { return _shaderProg; }

glm::vec3 CEnemy::getPos() const { return _pos; }

void CEnemy::reset()
{
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _color = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
    _rotAxis = 0;
    _bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
    _mxScale = glm::mat4(1.0f);
    _mxPos = glm::mat4(1.0f);
    _mxTRS = glm::mat4(1.0f);
    _mxRotX = glm::mat4(1.0f);
    _mxRotY = glm::mat4(1.0f);
    _mxRotZ = glm::mat4(1.0f);
    _mxRotation = glm::mat4(1.0f);
    _mxTransform = glm::mat4(1.0f);
    _mxFinal = glm::mat4(1.0f);
    _isDead = false;
    _speed = 0.4f;
    _hp = 5;
}
