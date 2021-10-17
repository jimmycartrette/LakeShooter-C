#pragma once
#define MAXCLOUDS 6
#include "gameobject.h"

struct Game;

enum SpeedModifier
{
    SPEED_FAST = 0,
    SPEED_NORMAL = 1,
    SPEED_SLOW = 2
};

struct Cloud
{
    struct GameObject m_obj;
    int8_t m_cloudindex;
    bool m_hflip;
    bool m_vflip;
    enum SpeedModifier m_speedmodifier;
};
struct Clouds
{
    struct Cloud cloud[MAXCLOUDS];
};

void Clouds_Init(struct Clouds *clouds);
void Clouds_GenerateCloud(struct Game *game);
void Clouds_Update(struct Clouds *clouds, struct Game *game);
void Clouds_Draw(struct Clouds *clouds);