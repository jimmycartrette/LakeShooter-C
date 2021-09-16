#pragma once
#include "ship.h"

struct PlayArea
{
    int m_x, m_y;
    int m_offsetX, m_offsetY;
    int m_width, m_height;
    char *m_worldmap;
};

void PlayArea_Initialize(struct PlayArea *p);
void PlayArea_Update(struct PlayArea *p, struct Ship *ship, const int ticks);
void PlayArea_Draw(struct PlayArea *p);