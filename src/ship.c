#include "ship.h"
#include "wasm4.h"
#include "playarea.h"
#include "game.h"
#include "global.h"
#include "sound.h"
#include "utils.h"

const int SHIPWIDTH = 16;
const int SHIPHEIGHT = 4;

const char SHIPSPRITE[16] = {
    0b11111010,
    0b01111011,
    0b11111010,
    0b00011011,
    0b11000000,
    0b00000000,
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
        ships->ship[n].m_obj.m_scoreworth = 30;
        ships->ship[n].m_obj.m_type = OBJECT_SHIP;
        ships->ship[n].m_obj.m_alive = false;
        ships->ship[n].m_obj.m_startedMoving = false;
    }
}
void Ships_Create(struct Ships *ships, struct PlayArea *p)
{
    for (int i = 0; i < MAXSHIPS; i++)
    {
        if (!ships->ship[i].m_obj.m_alive)
        {
            ships->ship[i].m_obj.m_scoreworth = 30;
            ships->ship[i].m_obj.m_alive = true;
            ships->ship[i].m_obj.m_width = SHIPWIDTH;
            ships->ship[i].m_obj.m_height = SHIPHEIGHT;
            ships->ship[i].m_obj.m_tickssincecollision = 0;
            ships->ship[i].m_obj.m_startedMoving = false;
            ships->ship[i].m_obj.m_dir = ((lsfr.m_lfsrvalue >> 3) % 2) == 1 ? DIRECTION_LEFT : DIRECTION_RIGHT;
            ships->ship[i].m_obj.m_islandwidth = p->m_playblocks[p->m_currenttopblock].m_islandwidth;
            ships->ship[i].m_obj.m_edgewidth = p->m_playblocks[p->m_currenttopblock].m_edgewidth;
            ships->ship[i].m_obj.m_posX = p->m_playblocks[p->m_currenttopblock].m_edgewidth + 1 +
                                          (lsfr.m_lfsrvalue % (80 - ships->ship[i].m_obj.m_width - p->m_playblocks[p->m_currenttopblock].m_islandwidth - p->m_playblocks[p->m_currenttopblock].m_edgewidth));
            if ((lsfr.m_lfsrvalue >> 3) % 2 == 0)
            {
                ships->ship[i].m_obj.m_posX = 160 - ships->ship[i].m_obj.m_posX - ships->ship[i].m_obj.m_width;
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

            if (((lsfr.m_lfsrvalue >> 3) % 6) == 1 && (int)ships->ship[n].m_obj.m_posY > 30)
            {
                ships->ship[n].m_obj.m_startedMoving = true;
            }
            GameObject_Update(&ships->ship[n].m_obj, &game);
            ships->ship[n].m_obj.m_posY -= p->m_changedy;
            if (ships->ship[n].m_obj.m_posY > 120)
            {
                ships->ship[n].m_obj.m_alive = false;
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
                blit(SHIPSPRITE, ships->ship[n].m_obj.m_posX, ships->ship[n].m_obj.m_posY, ships->ship[n].m_obj.m_width, ships->ship[n].m_obj.m_height, BLIT_1BPP | (ships->ship[n].m_obj.m_dir == DIRECTION_LEFT ? BLIT_FLIP_X : 0));
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