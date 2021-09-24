#pragma once
#include "ship.h"
#include <stdint.h>

struct PlayBlock
{
    int16_t m_edgewidth;
    uint8_t m_edgetransitionspeed;
    uint8_t m_islandtransitionspeed;
    uint8_t m_islandwidth;
    char m_displaypattern[200];
};

struct PlayArea
{
    int16_t m_x, m_y;
    int16_t m_previousx, m_previousy;
    int16_t m_changedy;
    int16_t m_offsetX, m_offsetY;
    uint16_t m_width, m_height;
    uint8_t m_currenttopblock;
    struct PlayBlock m_playblocks[7];
};

void PlayArea_Initialize(struct PlayArea *p);
bool PlayArea_Update(struct PlayArea *p, struct Ship *ship, int gameticks, const int ticks); // true if created playblock
void PlayArea_Draw(struct PlayArea *p);
void PlayArea_NewDraw(struct PlayArea *p);