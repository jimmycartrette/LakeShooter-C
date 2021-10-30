#pragma once
#include "gameobject.h"

#define MAXENEMYS 5

struct PlayArea;
struct Enemy
{
    struct GameObject m_obj;
};

struct Enemys
{
    struct Enemy enemy[MAXENEMYS];
};

void Enemys_Initialize(struct Enemys *enemys);
void Enemys_Update(struct Enemys *enemys, struct PlayArea *p);
void Enemys_Draw(struct Enemys *enemys, uint8_t spriteindex);
void Enemys_Create(struct Enemys *enemys, struct PlayArea *p, enum ObjectType type);
void Enemys_AndJetCollisionDetect(struct Enemys *enemys, struct Jet *jet, struct Game *game);