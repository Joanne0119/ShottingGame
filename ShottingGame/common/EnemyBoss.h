#pragma once
#include "CEnemy.h"

namespace BossPhase {
enum Phase { Entry,      // 出場中
        Phase1,     // 血量 100%~60%
        Phase2,     // 血量 60%~30%
        Phase3,     // 血量 30% 以下
        Dying
    };
};

class EnemyBoss: public CEnemy
{
public:
    EnemyBoss();
    EnemyBoss(glm::vec3 playerPos);
    virtual ~EnemyBoss();
    virtual void init();
    virtual void draw() override;
    virtual void drawExplosion() override;
    virtual void reset() override;
    virtual void update(float dt) override;
    virtual void move(float dt) override;
    virtual void shoot() override;
    float _angle;
    virtual void updatePhase();
    virtual float getCooldownByPhase();
    BossPhase::Phase _phase;
    int getMaxHp();
protected:
    void shootSpread();
    void shootHoming(glm::vec3 playerPos);
    glm::vec3 _playerPos;
    int _maxHp;
};
