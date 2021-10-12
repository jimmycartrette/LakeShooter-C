#pragma once

#include "gameobject.h"
#include "input.h"

struct Jet
{
    struct GameObject m_obj;
    int8_t fuelingtickscountdown;
};

struct Game;

void Jet_Initialize(struct Jet *jet);
void Jet_Update(struct Jet *jet, struct Input *i);
void Jet_Draw(struct Jet *jet, struct Game *game);
void Jet_CollisionDetect(struct Jet *jet);