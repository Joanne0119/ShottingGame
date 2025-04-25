#pragma once
#include "CEnemy.h"

class EnemyA: public CEnemy
{
public:
    EnemyA();
    virtual ~EnemyA();
    virtual void draw() override;
    virtual void drawExplosion() override;
    virtual void reset() override;
    virtual void update(float dt) override;
    virtual void move(float dt) override;
    virtual void shoot() override;
    float _angle;
};
