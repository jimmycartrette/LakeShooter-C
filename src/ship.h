#pragma once

#include "gameobject.h"
#include "input.h"

struct Ship
{
    struct GameObject m_obj;
    int8_t fuelingtickscountdown;
};

struct Game;

void Ship_Initialize(struct Ship *ship);
void Ship_Update(struct Ship *ship, struct Input *i);
void Ship_Draw(struct Ship *ship, struct Game *game);
void Ship_CollisionDetect(struct Ship *ship);