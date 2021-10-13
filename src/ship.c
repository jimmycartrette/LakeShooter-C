#include "ship.h"
#include "wasm4.h"
#include "playarea.h"
#include "game.h"
#include "global.h"
#include "sound.h"
#include "utils.h"

const int SHIPWIDTH = 12;
const int SHIPHEIGHT = 4;

const char SHIPSPRITE[12] = {
    0b11111100,
    0b11111100,
    0b11111100,
    0b11111100,
    0b00000000,
    0b00000000,
};

const char SHIPEXPLOSION[8] = {
    0b11111101,
    0b01111111,
    0b11101111,
    0b10111111,
    0b11111101,
    0b11011111,
    0b10111101,
    0b11101111,
};

void Ships_Initialize(struct Ships *ships)
{
    for (int n = 0; n < MAXSHIPS; n++)
    {
        ships->ship[n].m_obj.m_posX = 0;
        ships->ship[n].m_obj.m_posY = 0;
        ships->ship[n].m_obj.m_width = SHIPWIDTH;
        ships->ship[n].m_obj.m_height = SHIPHEIGHT;
        ships->ship[n].m_obj.m_type = OBJECT_SHIP;
        ships->ship[n].m_obj.m_alive = false;
    }
}
void Ships_Create(struct Ships *ships, struct PlayArea *p)
{
    for (int i = 0; i < MAXSHIPS; i++)
    {
        if (!ships->ship[i].m_obj.m_alive)
        {
            ships->ship[i].m_obj.m_alive = true;
            ships->ship[i].m_obj.m_width = SHIPWIDTH;
            ships->ship[i].m_obj.m_height = SHIPHEIGHT;
            ships->ship[i].m_obj.m_tickssincecollision = 0;
            ships->ship[i].m_obj.m_posX = p->m_playblocks[p->m_currenttopblock].m_edgewidth + 1 + (lsfr.m_lfsrvalue % (80 - ships->ship[i].m_obj.m_width - p->m_playblocks[p->m_currenttopblock].m_islandwidth - p->m_playblocks[p->m_currenttopblock].m_edgewidth));
            if (lsfr.m_lfsrvalue % 2 == 0)
            {
                ships->ship[i].m_obj.m_posX = 160 - ships->ship[i].m_obj.m_posX;
            }
            int toint = ships->ship[i].m_obj.m_posX;
            ships->ship[i].m_obj.m_posY = p->m_offsetY;

            return;
        }
    }
}
void Ships_Update(struct Ships *ships, struct PlayArea *p)
{
    int n;
    for (n = 0; n < MAXSHIPS; n++)
        if (ships->ship[n].m_obj.m_alive)
        {
            GameObject_Update(&ships->ship[n].m_obj);
            ships->ship[n].m_obj.m_posY -= p->m_changedy;
            if (ships->ship[n].m_obj.m_posY > 120)
            {
                ships->ship[n].m_obj.m_alive = false;
                // tracef("ship cleanup %d", n);
            }
        }
}
void Ships_Draw(struct Ships *ships)
{
    int n;
    for (n = 0; n < MAXSHIPS; n++)
    {
        if (ships->ship[n].m_obj.m_alive)
        {
            if (ships->ship[n].m_obj.m_tickssincecollision == 0)
            {
                *DRAW_COLORS = 4;
                blit(SHIPSPRITE, ships->ship[n].m_obj.m_posX, ships->ship[n].m_obj.m_posY, ships->ship[n].m_obj.m_width, ships->ship[n].m_obj.m_height, BLIT_1BPP);
                // char buffer[4];
                // itoa(buffer, n);
                // if (DEBUG == 1)
                // {
                //     //text(buffer, ships->ship[n].m_obj.m_posX + 1, ships->ship[n].m_obj.m_posY);
                // }
            }
            else
            {
                *DRAW_COLORS = 4;
                blit(SHIPEXPLOSION, ships->ship[n].m_obj.m_posX, ships->ship[n].m_obj.m_posY, ships->ship[n].m_obj.m_width, ships->ship[n].m_obj.m_height, BLIT_1BPP);
            }
        }
    }
}