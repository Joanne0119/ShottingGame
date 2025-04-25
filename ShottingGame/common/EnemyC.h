#pragma once
#include "CEnemy.h"

class EnemyC: public CEnemy
{
public:
    EnemyC();
    virtual ~EnemyC();
    virtual void draw() override;
    virtual void drawExplosion() override;
    virtual void reset() override;
    virtual void update(float dt) override;
    virtual void move(float dt) override;
    float _angle;
};
