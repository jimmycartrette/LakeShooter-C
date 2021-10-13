#pragma once
#include "gameobject.h"

#define MAXSHIPS 3

struct PlayArea;
struct Ship
{
    struct GameObject m_obj;
};

struct Ships
{
    struct Ship ship[MAXSHIPS];
};

void Ships_Initialize(struct Ships *ships);
void Ships_Update(struct Ships *ships, struct PlayArea *p);
void Ships_Draw(struct Ships *ships);
void Ships_Create(struct Ships *ships, struct PlayArea *p);
void Ships_AndJetCollisionDetect(struct Ships *ships, struct Jet *jet, struct Game *game);