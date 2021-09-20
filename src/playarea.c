#include "playarea.h"
#include "global.h"
#include "utils.h"
#include "wasm4.h"
#include "ship.h"

void Generate_PlayBlock_Pattern(struct PlayBlock *playblock)
{
    for (uint16_t row = 0; row < 20; row++)
    {
        for (uint16_t column = 0; column < 10; column++)
        {

            unsigned char field = 0b00000000;
            if (column >= playblock->m_edgewidth / 8)
            {

                field = 0b11111111;
                if (playblock->m_edgewidth - column * 8 > 0)
                {
                    field >>= 8 - (playblock->m_edgewidth - column * 8);
                }
            }
            playblock->m_displaypattern[row * 10 + column] = field;
        }
    }
}

void Generate_PlayBlock(bool start, const char seed, struct PlayBlock *previousplayblock, struct PlayBlock *generatedplayblock)
{

    if (seed % 4 == 0 || start)
    {

        generatedplayblock->m_edgewidth = 10 + (abs(seed) % 50);
        if (generatedplayblock->m_edgewidth < 30)
        {
            if (seed & 2)
            {
                generatedplayblock->m_islandwidth = 10;
            }
            else
            {
                generatedplayblock->m_islandwidth = 0;
            }
        }
    }
    else
    {

        generatedplayblock->m_edgewidth = previousplayblock->m_edgewidth;
        generatedplayblock->m_islandwidth = previousplayblock->m_islandwidth;
        //   generatedplayblock.m_transitionspeed = 0;
    }
    tracef("playblock width is %d", generatedplayblock->m_edgewidth);
    Generate_PlayBlock_Pattern(generatedplayblock);
}

void PlayArea_Initialize(struct PlayArea *p)
{
    p->m_y = 0;
    p->m_x = 0;
    p->m_width = PLAY_WIDTH;
    p->m_height = PLAY_HEIGHT;
    p->m_currenttopblock = 0;
    for (uint8_t i = 0; i < 7; i++)
    {
        bool isstart = i == 0;
        Generate_PlayBlock(isstart, lsfr.m_lfsrvalue, &p->m_playblocks[i - 1], &p->m_playblocks[i]);
        lfsr_next(&lsfr);
        // tracef("lfsr is %x", lsfr.m_lfsrvalue);
        // tracef("playblock %x has width %d", i, p->m_playblocks[i].m_edgewidth);
    }
}

void PlayArea_Update(struct PlayArea *p, struct Ship *ship, int gameticks, const int ticks)
{
    p->m_y = p->m_y - ship->m_obj.m_vaccel * 2.2 * ticks;
    if (p->m_y < 0)
    {
        p->m_y = 1920;
    }

    if (gameticks % 20 == 0)
    {

        if (p->m_currenttopblock == 0)
        {
            p->m_currenttopblock = 7;
            trace("topblock is over");
        }
        else
        {
            p->m_currenttopblock--;
        }
        Generate_PlayBlock(false, lsfr.m_lfsrvalue, &p->m_playblocks[p->m_currenttopblock + 1], &p->m_playblocks[p->m_currenttopblock]);
        lfsr_next(&lsfr);
        // tracef("lfsr is %x", lsfr.m_lfsrvalue);
        // tracef("playblock %x has width %d", p->m_currenttopblock, p->m_playblocks[p->m_currenttopblock].m_edgewidth);
    }
}
void PlayArea_NewDraw(struct PlayArea *p)
{
    float startRowF = (p->m_y / 4) / TILESIZEY;
    int startRow = (int)startRowF;
    p->m_offsetY = -p->m_y / 4 + startRow * TILESIZEY;

    *DRAW_COLORS = 3;
    for (uint8_t pb = 0; pb < 7; pb++)
    {
        uint8_t wantedindex = (pb + p->m_currenttopblock) % 7;
        blit(p->m_playblocks[wantedindex].m_displaypattern, 0, (pb * 20) + p->m_offsetY, 80, 20, 0);
        blit(p->m_playblocks[wantedindex].m_displaypattern, 80, (pb * 20) + p->m_offsetY, 80, 20, BLIT_FLIP_X);
    }
}
// void PlayArea_Draw(struct PlayArea *p)
// {
//     float startRowF = (p->m_y / 4) / TILESIZEY;
//     int startRow = (int)startRowF;
//     int endRow = startRow + (p->m_height / TILESIZEY);
//     p->m_offsetY = -p->m_y / 4 + startRow * TILESIZEY;
//     for (int column = 0; column < PLAY_WIDTH / TILESIZEX; column++)
//     {
//         for (int row = startRow; row <= endRow; row++)
//         {
//             //adjustedrow = row;

//             char result[3] = "";

//             switch (*(p->m_worldmap + row * 11 + column))
//             {
//             case '.':
//                 *DRAW_COLORS = 1;

//                 break;
//             case '*':
//                 *DRAW_COLORS = 3;

//                 break;
//             default:
//                 break;
//             }
//             int y = (row - startRow) * TILESIZEY + p->m_offsetY;
//             rect(column * TILESIZEX, y, TILESIZEX, TILESIZEY);
//         }
//     }
// }