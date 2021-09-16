#include "playarea.h"
#include "global.h"
#include "utils.h"
#include "wasm4.h"
#include "ship.h"

const char WORLDMAP[32][11] = {

    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "***....***",
    "****..****",
    "***....***",
    "*........*",
    ".....*....",
    "....***...",
    "....***...",
    ".....*....",
    "*........*",
    "*........*",
    "**......**",
    "**..**..**",
    "**...*****",
    "**....****",
    "**.....***",
    "**.....***",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
};

void PlayArea_Initialize(struct PlayArea *p)
{
    p->m_y = 0;
    p->m_width = PLAY_WIDTH;
    p->m_height = PLAY_HEIGHT;
    p->m_worldmap = (char *)WORLDMAP;
}

void PlayArea_Update(struct PlayArea *p, struct Ship *ship, const int ticks)
{
    p->m_y = p->m_y - ship->m_obj.m_vaccel * 2.2 * ticks;
}
void PlayArea_Draw(struct PlayArea *p)
{
    float startRowF = (p->m_y / 4) / TILESIZEY;
    int startRow = (int)startRowF;
    int endRow = startRow + (p->m_height / TILESIZEY);
    p->m_offsetY = -p->m_y / 4 + startRow * TILESIZEY;
    for (int column = 0; column < PLAY_WIDTH / TILESIZEX; column++)
    {
        for (int row = startRow; row <= endRow; row++)
        {
            //adjustedrow = row;

            char result[3] = "";

            switch (*(p->m_worldmap + row * 11 + column))
            {
            case '.':
                *DRAW_COLORS = 1;

                break;
            case '*':
                *DRAW_COLORS = 3;

                break;
            default:
                break;
            }
            int y = (row - startRow) * TILESIZEY + p->m_offsetY;
            rect(column * TILESIZEX, y, TILESIZEX, TILESIZEY);
        }
    }
}