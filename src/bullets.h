#pragma once
#define MAXBULLETS 4
#include "gameobject.h"
#include "playarea.h"
#include "jet.h"
#include "input.h"

struct Game;

struct Bullet
{
    struct GameObject m_obj;
};
struct Bullets
{
    struct Bullet bullet[MAXBULLETS];
};

void Bullets_Init(struct Bullets *bullets);
void Bullets_GenerateBullet(struct Bullets *bullets, struct Jet *jet, struct Game *game);
void Bullets_Update(struct Bullets *bullets, struct PlayArea *playarea, struct Jet *jet);
void Bullets_Draw(struct Bullets *bullets);
void Bullets_Anything_CollisionDetect(struct Bullets *bullets, struct Game *game);