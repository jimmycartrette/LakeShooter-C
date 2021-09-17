#pragma once

#include "gameobject.h"
#include "input.h"

struct Ship
{
    struct GameObject m_obj;
};

void Ship_Initialize(struct Ship *s);
void Ship_Update(struct Ship *s, struct Input *i);
void Ship_Draw(struct Ship *s);