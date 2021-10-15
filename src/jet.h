#pragma once

#include "gameobject.h"
#include "input.h"
#include "ship.h"

struct Jet
{
    struct GameObject m_obj;
    int8_t fuelingtickscountdown;
};

struct Game;

void Jet_Initialize(struct Jet *jet);
void Jet_Update(struct Jet *jet, struct Input *i, struct Game *game);
void Jet_Draw(struct Jet *jet, struct Game *game);
void Jet_Land_CollisionDetect(struct Jet *jet);
void Jet_Ships_CollisionDetect(struct Jet *jet, struct Ships *ships, struct Game *game);