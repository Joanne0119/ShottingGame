#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <list>

#include "Cplayer.h"
#include "initshader.h"
#include "CTrapezid.h"
#include "CTriangle.h"
#include "CPenta.h"

CPlayer::CPlayer(GLint shaderProg) {
    //scale
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _topScale    = glm::vec3(0.5f, 0.5f, 1.0f);
    _bodyScale   = glm::vec3(0.5f, 0.7f, 1.0f);
    _wingsScale  = glm::vec3(1.0f, 0.25f, 1.0f);
    _windowScale = glm::vec3(0.15f, 0.175f, 1.0f);
    _bottomScale = glm::vec3(0.9f, 0.25f, 1.0f);
    _fireScale   = glm::vec3(0.25f, 0.25f, 1.0f);
    //color
    _topColor = glm::vec3(0.25f, 0.28f, 1.0f);
    _bodyColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _wingsColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _windowColor = glm::vec3(0.25f, 0.28f, 1.0f);
    _bottomColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _fireColor = glm::vec3(0.8f, 0.25f, 0.0f);
    //pos
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _topPos = glm::vec3(0.0f, -1.3f, 0.0f);
    _bodyPos = glm::vec3(0.0f, -1.8f, 0.0f);
    _wingsPos = glm::vec3(0.0f, -1.8f, 0.0f);
    _windowPos = glm::vec3(0.0f, -1.8f, 0.0f);
    _bottomPos = glm::vec3(0.0f, -2.4f, 0.0f);
    _firePos[0] = glm::vec3(-0.16f, -2.6f, 0.0f);
    _firePos[1] = glm::vec3(0.16f, -2.6f, 0.0f);
    //rot
    _topRotX = 0.0f; _topRotY = 0.0f; _topRotZ = 0.0f;
    _bodyRotX = 0.0f; _bodyRotY = 0.0f; _bodyRotZ = 0.0f;
    _wingsRotX = 0.0f; _wingsRotY = 0.0f; _wingsRotZ = 0.0f;
    _windowRotX = 0.0f; _windowRotY = 0.0f; _windowRotZ = 0.0f;
    _bottomRotX = 0.0f; _bottomRotY = 0.0f; _bottomRotZ = 0.0f;
    _fireRotX = 0.0f; _fireRotY = 0.0f; _fireRotZ = 180.0f;
    _bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
    _fireRate = 0.2f;
    _fireCooldown = 0.0f;
    _mxScale = glm::mat4(1.0f);
    _mxPos = glm::mat4(1.0f);
    _mxRotX = glm::mat4(1.0f);
    _mxRotY = glm::mat4(1.0f);
    _mxRotZ = glm::mat4(1.0f);
    _mxRotation = glm::mat4(1.0f);
    _mxTRS = glm::mat4(1.0f);
    _mxTransform = glm::mat4(1.0f);
    _mxFinal = glm::mat4(1.0f);
    
    //action
    _isDead = false;
    _hp = 5;
    _maxHp = 5;
    _state = PlayerState::Alive;
    _style = PlayerState::Style1;
    _hurtTimer = 0;
    
    _isHit = false;
    _hitEffectDuration = 0.4f;
    _hitTimer = 0.0f;
    _flashInterval = 0.1f;
    _flashTimer = 0.0f;
    _isVisible = true;
    
    top = new CTriangle;
    body = new CTrapezid;
    wings = new CTrapezid;
    window = new CTrapezid;
    bottom = new CTriangle;
    fire =  new CTriangle[2];
    
    _shaderProg = shaderProg;
    top->setShaderID(_shaderProg);
    top->setPos(_topPos);
    top->setColor(_topColor);
    top->setScale(_topScale);
    
    body->setShaderID(_shaderProg);
    body->setPos(_bodyPos);
    body->setColor(_bodyColor);
    body->setScale(_bodyScale);
    
    wings->setShaderID(_shaderProg);
    wings->setPos(_wingsPos);
    wings->setColor(_wingsColor);
    wings->setScale(_wingsScale);
    
    window->setShaderID(_shaderProg);
    window->setPos(_windowPos);
    window->setColor(_windowColor);
    window->setScale(_windowScale);
    
    bottom->setShaderID(_shaderProg);
    bottom->setPos(_bottomPos);
    bottom->setColor(_bottomColor);
    bottom->setScale(_bottomScale);
    
    for (int i = 0; i < 2; i++)
    {
        fire[i].setShaderID(_shaderProg);
        fire[i].setColor(_fireColor);
        fire[i].setScale(_fireScale);
        fire[i].setPos(_firePos[i]);
        fire[i].setRotZ(_fireRotZ);
    }
    
}

CPlayer::~CPlayer()
{
    if(top != nullptr) delete top;
    if(body != nullptr) delete body;
    if(wings != nullptr) delete wings;
    if(window != nullptr) delete window;
    if(bottom != nullptr) delete bottom;
    if(fire != nullptr) delete [] fire;
}

void CPlayer::update(float dt){
    _fireCooldown -= dt;
    
    if (_isHit) {
        _hitTimer -= dt;
        _flashTimer -= dt;
        
        // 控制閃爍間隔
        if (_flashTimer <= 0.0f) {
            _flashTimer = _flashInterval;  // 重置閃爍計時器
            _isVisible = !_isVisible;      // 切換可見性狀態
            
            // 切換顏色
            if (_isVisible) {
                setColor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));       // 顯示時為白色閃爍
            } else {
                if (_style == PlayerState::Style1){
                    PlayerState::Style style = PlayerState::Style1;
                    setStyleColor(style);
                }
                else if (_style == PlayerState::Style2){
                    PlayerState::Style style = PlayerState::Style2;
                    setStyleColor(style);
                }
                else if(_style == PlayerState::Style3){
                    PlayerState::Style style = PlayerState::Style3;
                    setStyleColor(style);
                }
                
            }
        }
        
        
        // 檢查整個受擊效果是否結束
        if (_hitTimer <= 0.0f) {
            _isHit = false;
            _isVisible = true;
            
            if (_style == PlayerState::Style1){
                PlayerState::Style style = PlayerState::Style1;
                setStyleColor(style);
            }
            else if (_style == PlayerState::Style2){
                PlayerState::Style style = PlayerState::Style2;
                setStyleColor(style);
            }
            else if(_style == PlayerState::Style3){
                PlayerState::Style style = PlayerState::Style3;
                setStyleColor(style);
            }
        }
    }
    
    
    for (auto it = _missiles.begin(); it != _missiles.end(); ) {
        (*it)->update(dt);
        if ((*it)->isOutOfBounds()) {
            delete *it;               // 釋放記憶體
            it = _missiles.erase(it); // 移除元素
        } else {
            ++it;
        }
    }
    
    if (_state == PlayerState::Dead) return;

//    if (_state == PlayerState::Hurt1 || _state == PlayerState::Hurt2) {
//        _hurtTimer -= dt;
//        if (_hurtTimer <= 0) {
//            _state = PlayerState::Alive;
//        }
//        return; // 爆炸中不更新移動
//    }
    // 每幀累積向下移動
    _fireScale.y += 0.08f * dt;
    _firePos[0].y -= 0.08f * dt;
    _firePos[1].y -= 0.08f * dt;

    // 如果火焰移動到太下面，就重設回原點
    if (_fireScale.y > 0.35f) {
        _fireScale.y = 0.25f;
        _firePos[0].y = -2.6f;
        _firePos[1].y = -2.6f;
    }
    setPos(_topPos, _bodyPos, _wingsPos, _windowPos, _bottomPos, _firePos[0], _firePos[1]);
    setScale(_topScale, _bodyScale, _wingsScale, _windowScale, _bottomScale, _fireScale);
}

void CPlayer::draw()
{
    if (_state == PlayerState::Dead) return;
    //MARK: - Style1
    
    if (_style == PlayerState::Style1){
        setPos(
               glm::vec3(0.0f, -1.3f, 0.0f),
               glm::vec3(0.0f, -1.8f, 0.0f),
               glm::vec3(0.0f, -1.8f, 0.0f),
               glm::vec3(0.0f, -1.8f, 0.0f),
               glm::vec3(0.0f, -2.4f, 0.0f),
               glm::vec3(-0.16f, -2.6f, 0.0f),
               glm::vec3(0.16f, -2.6f, 0.0f));
        
        setColor(glm::vec3(0.25f, 0.28f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.28f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.8f, 0.25f, 0.0f));
        setScale(
                 glm::vec3(0.5f, 0.5f, 1.0f),
                 glm::vec3(0.5f, 0.7f, 1.0f),
                 glm::vec3(1.0f, 0.25f, 1.0f),
                 glm::vec3(0.15f, 0.175f, 1.0f),
                 glm::vec3(0.9f, 0.25f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 1.0f));
        
        if (_isHit) {
            setColor(glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.8f, 0.25f, 0.0f));
        }
        else {
            if (_state == PlayerState::Hurt1) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bodyRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
                
            }
            else if(_state == PlayerState::Hurt2) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bodyRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
                
            }
        }
    }
    //MARK: - Style2
    else if(_style == PlayerState::Style2){
        setPos(
               glm::vec3(-0.6f, -2.15f, 0.0f),
               glm::vec3(0.0f, -1.5f, 0.0f),
               glm::vec3(0.0f, -1.9f, 0.0f),
               glm::vec3(0.0f, -1.5f, 0.0f),
               glm::vec3(0.6f, -2.15f, 0.0f),
               glm::vec3(-0.6f, -2.3f, 0.0f),
               glm::vec3(0.6f, -2.3f, 0.0f));
        
        setColor(glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.28f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.8f, 0.25f, 0.0f));
        setScale(
                 glm::vec3(0.9f, 0.25f, 1.0f),
                 glm::vec3(0.6f, 0.4f, 1.0f),
                 glm::vec3(1.3f, 0.25f, 1.0f),
                 glm::vec3(0.15f, 0.175f, 1.0f),
                 glm::vec3(0.9f, 0.25f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 1.0f));
       
        if (_isHit) {
            setColor(glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.8f, 0.25f, 0.0f));
        }
        else {
            if (_state == PlayerState::Hurt1) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
            else if(_state == PlayerState::Hurt2) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
        }
    }
    //MARK: - Style3
    else if(_style == PlayerState::Style3){
        setPos(
               glm::vec3(-0.39, -1.35f, 0.0f),
               glm::vec3(0.0f, -1.6f, 0.0f),
               glm::vec3(0.0f, -2.0f, 0.0f),
               glm::vec3(0.0f, -1.6f, 0.0f),
               glm::vec3(0.39f, -1.35f, 0.0f),
               glm::vec3(0.0f, -2.4f, 0.0f),
               glm::vec3(0.0f, -2.7f, 0.0f));
        
        setColor(glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.25f, 0.28f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 0.25f),
                 glm::vec3(0.8f, 0.25f, 0.0f));
        setScale(
                 glm::vec3(0.4f, 0.2f, 1.0f),
                 glm::vec3(0.6f, 0.4f, 1.0f),
                 glm::vec3(0.5f, 0.3f, 1.0f),
                 glm::vec3(0.15f, 0.1f, 1.0f),
                 glm::vec3(0.4f, 0.2f, 1.0f),
                 glm::vec3(0.25f, 0.25f, 1.0f));
        if (_isHit) {
            setColor(glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.8f, 0.25f, 0.0f));
        }
        else {
            if (_state == PlayerState::Hurt1) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
            else if(_state == PlayerState::Hurt2) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
        }
    }
    if (!_isHit || _isVisible) {
        top->draw();
        body->draw();
        wings->draw();
        window->draw();
        bottom->draw();
        fire[0].draw();
        fire[1].draw();
    }
    for (auto it = _missiles.begin(); it != _missiles.end(); ++it) {
        (*it)->draw();
    }
    
    
}

void CPlayer::shoot() {
    glm::vec3 currentPos;
    if(_style == PlayerState::Style1) currentPos = _topPos;
    else if (_style == PlayerState::Style2 || _style == PlayerState::Style3) currentPos = _bodyPos;
    if (_fireCooldown <= 0.0f) {
        auto missile = new CMissile(currentPos + _pos + glm::vec3(0.0f, 0.5f, 0.0f), 3.0f);
        missile->setShaderID(_shaderProg);
        missile->setColor(glm::vec3(0.5f, 0.5f, 0.8f));
        missile->setScale(glm::vec3(0.1f,0.1f,0.1f));
        _missiles.push_back(missile);
        _fireCooldown = _fireRate;
    }
}

std::list<CMissile*>& CPlayer::getMissiles() {
    return _missiles;
}

void CPlayer::printMissiles() {
    for (auto* missile : _missiles) {
        glm::vec3 position = missile->getPos();
        std::cout << "Missile position: ("
                  << position.x << ", " << position.y << ", " << position.z << ")\n";
    }
}

void CPlayer::setState(PlayerState::State state) { _state = state; }
PlayerState::State CPlayer::getState() const { return _state; }

void CPlayer::setStyleColor(PlayerState::Style style){
    switch(style){
        case PlayerState::Style1:
            setColor(glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.8f, 0.25f, 0.0f));
            if (_state == PlayerState::Hurt1) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bodyRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
                
            }
            else if(_state == PlayerState::Hurt2) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bodyRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
                
            }
            break;
        case PlayerState::Style2:
            setColor(glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.8f, 0.25f, 0.0f));
            if (_state == PlayerState::Hurt1) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
            else if(_state == PlayerState::Hurt2) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
            break;
        case PlayerState::Style3:
            setColor(glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.25f, 0.28f, 1.0f),
                     glm::vec3(0.25f, 0.25f, 0.25f),
                     glm::vec3(0.8f, 0.25f, 0.0f));
            if (_state == PlayerState::Hurt1) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
            else if(_state == PlayerState::Hurt2) {
                glm::vec3 _topRedColor = glm::vec3(0.45f, 0.25f, 0.25f);
                glm::vec3 _bodyRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _wingsRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _windowRedColor = glm::vec3(0.45f, 0.28f, 1.0f);
                glm::vec3 _bottomRedColor = glm::vec3(0.75f, 0.25f, 0.25f);
                glm::vec3 _firRedeColor = glm::vec3(1.0f, 0.25f, 0.0f);
                
                setColor(_topRedColor, _bodyRedColor, _wingsRedColor, _windowRedColor, _bottomRedColor, _firRedeColor);
            }
            break;
    }
}

void CPlayer::setStyle(PlayerState::Style style) { _style = style; }
PlayerState::Style CPlayer::getStyle() const { return _style; }

bool CPlayer::isDead() const{
    return _isDead;
}

void CPlayer::onHit(int damage){
    if (_isDead || !_isVisible) return;
    _hp -= damage;
    if (_hp <= 0) {
        _isDead = true;
        _state = PlayerState::Dead;
        std::cout << "player is dead" << std::endl;
    }
    else if(_hp < 5 && _hp > 2){
        _state = PlayerState::Hurt1;
        _hurtTimer = 0.3;
        std::cout << "player hurt1, hp is " << _hp << std::endl;
    }
    else {
        _state = PlayerState::Hurt2;
        _hurtTimer = 0.3;
        std::cout << "player hurt2, hp is " << _hp << std::endl;
    }
    _isHit = true;
    _hitTimer = _hitEffectDuration;
    _flashTimer = _flashInterval;
    _isVisible = true;
    if (_style == PlayerState::Style1){
        PlayerState::Style style = PlayerState::Style1;
        setStyleColor(style);
    }
    else if (_style == PlayerState::Style2){
        PlayerState::Style style = PlayerState::Style2;
        setStyleColor(style);
    }
    else if(_style == PlayerState::Style3){
        PlayerState::Style style = PlayerState::Style3;
        setStyleColor(style);
    }
}

int CPlayer::getHp(){
    return _hp;
}

void CPlayer::setColor(glm::vec3 vTopColor, glm::vec3 vBodyColor, glm::vec3 vWingsColor, glm::vec3 vWindowColor, glm::vec3 vBottomColor, glm::vec3 vFireColor)
{
    _topColor = vTopColor;
    _bodyColor = vBodyColor;
    _wingsColor = vWingsColor;
    _windowColor = vWindowColor;
    _bottomColor = vBottomColor;
    _fireColor = vFireColor;
    
    top->setColor(vTopColor);
    body->setColor(vBodyColor);
    wings->setColor(vWingsColor);
    window->setColor(vWindowColor);
    bottom->setColor(vBottomColor);
    fire[0].setColor(vFireColor);
    fire[1].setColor(vFireColor);
    
}

void CPlayer::setScale(glm::vec3 vTopScale, glm::vec3 vBodyScale, glm::vec3 vWingsScale, glm::vec3 vWindowScale, glm::vec3 vBottomScale, glm::vec3 vFireScale)
{
    _topScale = vTopScale;
    _bodyScale = vBodyScale;
    _wingsScale = vWingsScale;
    _windowScale = vWindowScale;
    _bottomScale = vBottomScale;
    _fireScale = vFireScale;
    
    _bScale = true;
    top->setScale(vTopScale);
    body->setScale(vBodyScale);
    wings->setScale(vWingsScale);
    window->setScale(vWindowScale);
    bottom->setScale(vBottomScale);
    fire[0].setScale(vFireScale);
    fire[1].setScale(vFireScale);
}

void CPlayer::setScale(glm::vec3 _vScale){
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 pivot = glm::vec3(0.0f, -1.8f, 0.0f);
    glm::vec3 scale = _vScale;  // 假設Y軸縮放2倍

    // 1. 平移至原點
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -pivot);

    // 2. 縮放
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

    // 3. 平移回來並按縮放調整距離
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), pivot * scale);

    // 最終的變換矩陣
    model = translateBack * scaling * translateToOrigin;
    
    setTransformMatrix(model);
    
//        _topScale *= _scale;
//        _bodyScale *= _scale;
//        _windowScale *= _scale;
//        _wingsScale *= _scale;
//        _bottomScale *= _scale;
//        _fireScale *= _scale;
//    top->setScale(_topScale);
//    body->setScale(_bodyScale);
//    wings->setScale(_wingsScale);
//    window->setScale(_windowScale);
//    bottom->setScale(_bottomScale);
//    fire[0].setScale(_fireScale);
//    fire[1].setScale(_fireScale);
    
//    top->setScale(_topScale *= _scale);
//    body->setScale(_bodyScale *= _scale);
//    wings->setScale(_wingsScale *= _scale);
//    window->setScale(_windowScale *= _scale);
//    bottom->setScale(_bottomScale *= _scale);
//    fire[0].setScale(_fireScale *= _scale);
//    fire[1].setScale(_fireScale *= _scale);
//    setPos(_pos);
    
}

void CPlayer::setPos(glm::vec3 _vPos){
    _topPos = _vPos + _topPos;
    _bodyPos = _vPos + _bodyPos;
    _wingsPos = _vPos + _wingsPos;
    _windowPos = _vPos + _windowPos;
    _bottomPos = _vPos + _bottomPos;
    _firePos[0] = _vPos + _firePos[0];
    _firePos[1] = _vPos + _firePos[1];
    
    top->setPos(_topPos);
    body->setPos(_bodyPos);
    wings->setPos(_windowPos);
    window->setPos(_windowPos);
    bottom->setPos(_bottomPos);
    fire[0].setPos(_firePos[0]);
    fire[1].setPos(_firePos[1]);
}

void CPlayer::setPos(glm::vec3 vTopPos, glm::vec3 vBodyPos, glm::vec3 vWingsPos, glm::vec3 vWindowPos, glm::vec3 vBottomPos, glm::vec3 vFirePos1, glm::vec3 vFirePos2)
{
    _bPos = true;
    _topPos = vTopPos;
    _bodyPos = vBodyPos;
    _wingsPos = vWingsPos;
    _windowPos = vWindowPos;
    _bottomPos = vBottomPos;
    _firePos[0] = vFirePos1;
    _firePos[1] = vFirePos2;
    top->setPos(vTopPos);
    body->setPos(vBodyPos);
    wings->setPos(vWingsPos);
    window->setPos(vWindowPos);
    bottom->setPos(vBottomPos);
    fire[0].setPos(vFirePos1);
    fire[1].setPos(vFirePos2);
}

glm::vec3 CPlayer::getPos() {
//    if (name == "top") return _topPos;
//    else if (name == "body") return _bodyPos;
//    else if (name == "wings") return _wingsPos;
//    else if (name == "window") return _windowPos;
//    else if (name == "bottom") return _bottomPos;
//    else if (name == "fire0") return _firePos[0];
//    else if (name == "fire1") return _firePos[1];
//    else {
//        std::cerr << "Invalid position name: " << name << std::endl;
//        return glm::vec3(0.0f);
//    }
    glm::vec3 newPos = _pos + _bodyPos;
//    std::cout << "player ("<< newPos.x << ',' << newPos.y << ',' << newPos.z << ')' << std::endl;
    return newPos;
}

void CPlayer::setTransformMatrix(glm::mat4 mxMatrix)
{
    _bOnTransform = _bTransform = true;
    _mxTransform = mxMatrix;
    top->setTransformMatrix(mxMatrix);
    body->setTransformMatrix(mxMatrix);
    wings->setTransformMatrix(mxMatrix);
    window->setTransformMatrix(mxMatrix);
    bottom->setTransformMatrix(mxMatrix);
    fire[0].setTransformMatrix(mxMatrix);
    fire[1].setTransformMatrix(mxMatrix);
    
    _pos = glm::vec3(mxMatrix[3][0], mxMatrix[3][1], mxMatrix[3][2]);
    _scale = glm::vec3 (glm::length(glm::vec3(mxMatrix[0][0], mxMatrix[0][1], mxMatrix[0][2])),
                    glm::length(glm::vec3(mxMatrix[1][0], mxMatrix[1][1], mxMatrix[1][2])),
                    glm::length(glm::vec3(mxMatrix[2][0], mxMatrix[2][1], mxMatrix[2][2])));
//    std::cout << "_pos : (" << _pos.x << "," << _pos.y << "," << _pos.z << ") \n";
//    std::cout << "_scale : ("<< _scale.x << "," << _scale.y << "," << _scale.z << ") \n";
}

void CPlayer::reset()
{
    //scale
    _topScale = glm::vec3(1.0f, 1.0f, 1.0f);
    _bodyScale = glm::vec3(1.0f, 1.4f, 1.0f);
    _wingsScale = glm::vec3(2.0f, 0.5f, 1.0f);
    _windowScale = glm::vec3(2.0f, 0.5f, 1.0f);
    _bottomScale = glm::vec3(1.8f, 0.5f, 1.0f);
    _fireScale = glm::vec3(0.5f, 0.5f, 1.0f);
    //color
    _topColor = glm::vec3(0.25f, 0.28f, 1.0f);
    _bodyColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _wingsColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _windowColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _bottomColor = glm::vec3(0.25f, 0.25f, 0.25f);
    _fireColor = glm::vec3(0.8f, 0.25f, 0.0f);
    //pos
    _topPos = glm::vec3(0.0f, -0.85f, 0.0f);
    _bodyPos = glm::vec3(0.0f, -1.8f, 0.0f);
    _wingsPos = glm::vec3(0.0f, -1.8f, 0.0f);
    _windowPos = glm::vec3(0.0f, -1.8f, 0.0f);
    _bottomPos = glm::vec3(0.0f, -2.9f, 0.0f);
    _firePos[0] = glm::vec3(-0.35f, -3.2f, 0.0f);
    _firePos[1] = glm::vec3(0.35f, -3.2f, 0.0f);
    //rot
    _topRotX = 0.0f; _topRotY = 0.0f; _topRotZ = 0.0f;
    _bodyRotX = 0.0f; _bodyRotY = 0.0f; _bodyRotZ = 0.0f;
    _wingsRotX = 0.0f; _wingsRotY = 0.0f; _wingsRotZ = 0.0f;
    _windowRotX = 0.0f; _windowRotY = 0.0f; _windowRotZ = 0.0f;
    _bottomRotX = 0.0f; _bottomRotY = 0.0f; _bottomRotZ = 0.0f;
    _fireRotX = 0.0f; _fireRotY = 0.0f; _fireRotZ = 180.0f;
}

