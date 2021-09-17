#pragma once

#include "gameobject.h"
#include "input.h"

struct Ship
{
    struct GameObject m_obj;
};

void Ship_Initialize(struct Ship *ship);
void Ship_Update(struct Ship *ship, struct Input *i);
void Ship_Draw(struct Ship *ship);
void Ship_CollisionDetect(struct Ship *ship);