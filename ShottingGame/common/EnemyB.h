#pragma once
#include "CEnemy.h"

class EnemyB: public CEnemy
{
public:
    EnemyB();
    virtual ~EnemyB();
    virtual void draw() override;
    virtual void drawExplosion() override;
    virtual void reset() override;
    virtual void update(float dt) override;
    virtual void move(float dt) override;
};
