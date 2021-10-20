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
    uint16_t m_currentblockindex;
    uint16_t m_bridgeblockindex;
    struct PlayBlock m_playblocks[7];
};

void PlayArea_Initialize(struct PlayArea *p);
bool PlayArea_Update(struct PlayArea *p, struct Jet *jet, int gameticks); // true if created playblock
void Generate_PlayBlock(uint16_t currentplayblockindex, bool first, bool start, bool noisland, const char seed, struct PlayBlock *previousplayblock, struct PlayBlock *generatedplayblock, uint16_t bridgeblockindex);
void PlayArea_Draw(struct PlayArea *p);
void PlayArea_NewDraw(struct PlayArea *p);