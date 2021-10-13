#pragma once
#define MAXFUELS 3
#include "gameobject.h"
#include "playarea.h"

struct Fuel
{
    struct GameObject m_obj;
};

struct Fuels
{
    struct Fuel fuel[MAXFUELS];
};

void Fuels_Initialize(struct Fuels *fuels);
void Fuels_Update(struct Fuels *fuels, struct PlayArea *p);
void Fuels_Draw(struct Fuels *fuels);
void Fuels_Create(struct Fuels *fuels, struct PlayArea *p);
void Fuels_Jet_CollisionDetect(struct Fuels *fuels, struct Jet *jet, struct Game *game);