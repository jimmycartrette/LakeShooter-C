#pragma once
#include "jet.h"
#include <stdint.h>

struct Gate
{
};
struct PlayBlock
{
    int16_t m_edgewidth;
    uint8_t m_edgetransitionspeed;
    uint8_t m_islandtransitionspeed;
    uint8_t m_islandwidth;
    bool m_hasbridge;
    char m_displaypattern[200];
};

struct PlayArea
{
    int32_t m_x, m_y;
    int32_t m_previousx, m_previousy;
    int16_t m_changedy;
    int16_t m_offsetX, m_offsetY;
    uint16_t m_width, m_height;
    uint8_t m_currenttopblock;
    struct PlayBlock m_playblocks[7];
};

void PlayArea_Initialize(struct PlayArea *p);
bool PlayArea_Update(struct PlayArea *p, struct Jet *jet, int gameticks, const int ticks); // true if created playblock
void Generate_PlayBlock(uint8_t index, bool first, bool start, bool noisland, const char seed, struct PlayBlock *previousplayblock, struct PlayBlock *generatedplayblock);
void PlayArea_Draw(struct PlayArea *p);
void PlayArea_NewDraw(struct PlayArea *p);