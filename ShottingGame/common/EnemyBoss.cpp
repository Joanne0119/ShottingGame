#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <iostream>

#include "EnemyBoss.h"

EnemyBoss::EnemyBoss() : CEnemy()
{
    init();
};

EnemyBoss::EnemyBoss(glm::vec3 playerPos) : CEnemy()
{
    init();
    _playerPos = playerPos;
};

EnemyBoss::~EnemyBoss()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao); 
    if (_points != NULL) {
        delete[] _points;
        _points = nullptr;
    }
    if (_idx != NULL){
        delete[] _idx;
        _idx = nullptr;
    }
}

void EnemyBoss::init(){
    _angle = 0.0f;
    _vtxCount = 16;
    _indexCount = 42;
    _vtxAttrCount = 11;
    _phase = BossPhase::Entry;
    _maxHp = 30;
    _hp = 30;
    
    _points = new GLfloat[_vtxCount * _vtxAttrCount]{
        -0.5f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.3f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.3f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f, 0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.35f,  0.18f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.6f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.4f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.1f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  -0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.1f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f,  -0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.4f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.6f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.35f,  0.18f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
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
        12, 13, 14};
    setupVertexAttributes();
}

void EnemyBoss::draw()
{
    if (_state == EnemyState::Dead) return;

    if (_state == EnemyState::Exploding) {
        // 你可以畫個簡單的爆炸或先用不同顏色表示
        drawExplosion();
        return;
    }
    else if(_state == EnemyState::Alive){
        // Alive 狀態畫正常模型
        updateMatrix();
        glUseProgram(_shaderProg);
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
        
        for (auto it = _missiles.begin(); it != _missiles.end(); ++it) {
            (*it)->draw();
        }
//        _hpBarBG->draw();
//        _hpBarFG->draw();
        glUseProgram(_shaderProg);
    }
}

void EnemyBoss::drawExplosion()
{
    CEnemy::drawExplosion();
}


void EnemyBoss::update(float dt)
{
    _fireCooldown -= dt;
//    std::cout << "boss pos: (" << _pos.x << ',' << _pos.y << ')' << std::endl;
    
    for (auto it = _missiles.begin(); it != _missiles.end(); ) {
        (*it)->updateEnemy(dt);
        if ((*it)->isOutOfBounds()) {
            delete *it;               // 釋放記憶體
            it = _missiles.erase(it); // 移除元素
        } else {
            ++it;
        }
    }

    if (_state == EnemyState::Dead) return;

    if (_state == EnemyState::Exploding) {
        _explosionTimer -= dt;
        if (_explosionTimer <= 0) {
            _state = EnemyState::Dead;
        }
        return; // 爆炸中不更新移動
    }
    
    updatePhase();

    move(dt);
    
    if (_fireCooldown <= 0.0f) {
        shoot();
        _fireCooldown = getCooldownByPhase();
    }

    setPos(_pos);
}

void EnemyBoss::move(float dt)
{
    _angle += dt * 4.0f;

    switch (_phase) {
        case BossPhase::Entry:
            _pos.y -= dt;
            if (_pos.y <= 2.5f) {
                _pos.y = 2.5f;
                _phase = BossPhase::Phase1;
            }
            break;
        case BossPhase::Phase1:
            _pos.y = sin(_angle) * 0.2f + 2.5f;
            break;
        case BossPhase::Phase2:
            _pos.x = sin(_angle * 2.0f) * 1.5f;
            _pos.y = sin(_angle) * 0.3f + 2.5f;
            break;
        case BossPhase::Phase3:
            _pos.x = sin(_angle * 3.0f) * 2.0f;
            _pos.y = sin(_angle * 2.0f) * 0.5f + 2.5f;
            break;
        default:
            break;
    }
    setPos(_pos);
}

void EnemyBoss::updatePhase() {
    if (_hp <= 0.0f) {
        _state = EnemyState::Exploding;
        _phase = BossPhase::Dying;
        _explosionTimer = 1.5f;
        return;
    }

    if (_hp < _maxHp * 0.5f && _phase != BossPhase::Phase3) {
        _phase = BossPhase::Phase3;
    } else if (_hp < _maxHp * 0.7f && _phase != BossPhase::Phase2) {
        _phase = BossPhase::Phase2;
    }
}

int EnemyBoss::getMaxHp() { return _maxHp;}

void EnemyBoss::shoot()
{
    switch (_phase) {
        case BossPhase::Phase1:
            CEnemy::shoot();
            break;
        case BossPhase::Phase2:
            shootSpread(); // 多方向散射
            break;
        case BossPhase::Phase3:
            shootHoming(_playerPos); // 追蹤式
            break;
        default:
            break;
    }
}

void EnemyBoss::shootSpread() {
    int bulletCount = 7;
    float spreadAngle = glm::radians(60.0f); // 總角度 60 度
    float startAngle = -spreadAngle / 2.0f;

    for (int i = 0; i < bulletCount; ++i) {
        float angle = startAngle + i * (spreadAngle / (bulletCount - 1)); // 每顆子彈的角度
        glm::vec3 dir = glm::vec3(sin(angle), -cos(angle), 0.0f); // 向下並帶角度
        auto missile = new CMissile(_pos, 5.0f, dir);
        missile->setShaderID(_shaderProg);
        missile->setColor(glm::vec3(1.0f, 0.3f, 0.2f));
        missile->setScale(glm::vec3(0.12f));
        _missiles.push_back(missile);
    }
}

void EnemyBoss::shootHoming(glm::vec3 playerPos) {
    auto missile = new CMissile(_pos - glm::vec3(0, 0.5f, 0), 3.5f);
    missile->setShaderID(_shaderProg);
    missile->setColor(glm::vec3(0.3f, 0.9f, 1.0f));
    missile->setScale(glm::vec3(0.14f));
    missile->setRotX(180);
    missile->setTarget(playerPos); // 啟用追蹤
    _missiles.push_back(missile);
    _fireCooldown = _fireRate;
}

float EnemyBoss::getCooldownByPhase() {
    switch (_phase) {
        case BossPhase::Phase1: return 2.0f;
        case BossPhase::Phase2: return 2.0f;
        case BossPhase::Phase3: return 1.0f;
        default: return 1.5f;
    }
}


void EnemyBoss::reset()
{
    CEnemy::reset();
}
