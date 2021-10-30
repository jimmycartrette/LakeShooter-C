#include "enemy.h"
#include "wasm4.h"
#include "playarea.h"
#include "game.h"
#include "global.h"
#include "sound.h"
#include "utils.h"

const int SHIPWIDTH = 16;
const int SHIPHEIGHT = 4;
const int HELIWIDTH = 8;
const int HELIHEIGHT = 8;
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
const char HELISPRITEA[8] = {
    0b11111111,
    0b11111111,
    0b11100001,
    0b11111011,
    0b11100000,
    0b00000000,
    0b11111011,
    0b11110001};
const char HELISPRITEB[8] = {
    0b11111111,
    0b11000011,
    0b11111111,
    0b11111011,
    0b01100000,
    0b00000000,
    0b11111011,
    0b11110001};
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

void Enemys_Initialize(struct Enemys *enemys)
{
    for (int n = 0; n < MAXENEMYS; n++)
    {
        enemys->enemy[n].m_obj.m_posX = 0;
        enemys->enemy[n].m_obj.m_posY = 0;
        enemys->enemy[n].m_obj.m_alive = false;
        enemys->enemy[n].m_obj.m_startedMoving = false;
    }
}
void Enemys_Create(struct Enemys *enemys, struct PlayArea *p, enum ObjectType type)
{
    for (int i = 0; i < MAXENEMYS; i++)
    {
        if (!enemys->enemy[i].m_obj.m_alive)
        {
            enemys->enemy[i].m_obj.m_type = type;
            if (enemys->enemy[i].m_obj.m_type == OBJECT_SHIP)
            {

                enemys->enemy[i].m_obj.m_scoreworth = 30;
                enemys->enemy[i].m_obj.m_width = SHIPWIDTH;
                enemys->enemy[i].m_obj.m_height = SHIPHEIGHT;
            }
            if (enemys->enemy[i].m_obj.m_type == OBJECT_HELI)
            {
                enemys->enemy[i].m_obj.m_scoreworth = 60;
                enemys->enemy[i].m_obj.m_width = HELIWIDTH;
                enemys->enemy[i].m_obj.m_height = HELIHEIGHT;
            }
            enemys->enemy[i].m_obj.m_alive = true;
            enemys->enemy[i].m_obj.m_tickssincecollision = 0;
            enemys->enemy[i].m_obj.m_startedMoving = false;
            enemys->enemy[i].m_obj.m_dir = ((lsfr.m_lfsrvalue >> 3) % 2) == 1 ? DIRECTION_LEFT : DIRECTION_RIGHT;
            enemys->enemy[i].m_obj.m_islandwidth = p->m_playblocks[p->m_currenttopblock].m_islandwidth;
            enemys->enemy[i].m_obj.m_edgewidth = p->m_playblocks[p->m_currenttopblock].m_edgewidth;
            enemys->enemy[i].m_obj.m_posX = p->m_playblocks[p->m_currenttopblock].m_edgewidth + 1 +
                                            (lsfr.m_lfsrvalue % (80 - enemys->enemy[i].m_obj.m_width - p->m_playblocks[p->m_currenttopblock].m_islandwidth - p->m_playblocks[p->m_currenttopblock].m_edgewidth));
            if ((lsfr.m_lfsrvalue >> 3) % 2 == 0)
            {
                enemys->enemy[i].m_obj.m_posX = 160 - enemys->enemy[i].m_obj.m_posX - enemys->enemy[i].m_obj.m_width;
            }
            int toint = enemys->enemy[i].m_obj.m_posX;
            enemys->enemy[i].m_obj.m_posY = p->m_offsetY;

            return;
        }
    }
}
void Enemys_Update(struct Enemys *enemys, struct PlayArea *p)
{
    int n;
    for (n = 0; n < MAXENEMYS; n++)
        if (enemys->enemy[n].m_obj.m_alive)
        {

            if (((lsfr.m_lfsrvalue >> 3) % 6) == 1 && (int)enemys->enemy[n].m_obj.m_posY > 30)
            {
                enemys->enemy[n].m_obj.m_startedMoving = true;
            }
            GameObject_Update(&enemys->enemy[n].m_obj, &game);
            enemys->enemy[n].m_obj.m_posY -= p->m_changedy;
            if (enemys->enemy[n].m_obj.m_posY > 120)
            {
                enemys->enemy[n].m_obj.m_alive = false;
            }
        }
}
void Enemys_Draw(struct Enemys *enemys, uint8_t spriteindex)
{
    int n;
    for (n = 0; n < MAXENEMYS; n++)
    {
        if (enemys->enemy[n].m_obj.m_alive)
        {
            if (enemys->enemy[n].m_obj.m_tickssincecollision == 0)
            {
                *DRAW_COLORS = 4;
                if (enemys->enemy[n].m_obj.m_type == OBJECT_SHIP)
                {
                    blit(SHIPSPRITE, enemys->enemy[n].m_obj.m_posX,
                         enemys->enemy[n].m_obj.m_posY, enemys->enemy[n].m_obj.m_width,
                         enemys->enemy[n].m_obj.m_height,
                         BLIT_1BPP | (enemys->enemy[n].m_obj.m_dir == DIRECTION_LEFT ? BLIT_FLIP_X : 0));
                }
                if (enemys->enemy[n].m_obj.m_type == OBJECT_HELI)
                {
                    blit(spriteindex == 0 ? HELISPRITEA : HELISPRITEB, enemys->enemy[n].m_obj.m_posX,
                         enemys->enemy[n].m_obj.m_posY, enemys->enemy[n].m_obj.m_width,
                         enemys->enemy[n].m_obj.m_height,
                         BLIT_1BPP | (enemys->enemy[n].m_obj.m_dir == DIRECTION_LEFT ? BLIT_FLIP_X : 0));
                }
                // char buffer[4];
                // itoa(buffer, n);
                // if (DEBUG == 1)
                // {
                //     //text(buffer, enemys->enemy[n].m_obj.m_posX + 1, enemys->enemy[n].m_obj.m_posY);
                // }
            }
            else
            {
                *DRAW_COLORS = 4;
                blit(SHIPEXPLOSION, enemys->enemy[n].m_obj.m_posX, enemys->enemy[n].m_obj.m_posY, enemys->enemy[n].m_obj.m_width, enemys->enemy[n].m_obj.m_height, BLIT_1BPP);
            }
        }
    }
}