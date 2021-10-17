#include "playarea.h"
#include "global.h"
#include "utils.h"
#include "wasm4.h"
#include "jet.h"
#include "fuel.h"

void Generate_PlayBlock_Pattern(struct PlayBlock *playblock, struct PlayBlock *previousplayblock)
{

    for (uint16_t row = 0; row < 20; row++)
    {
        int wantedislandwidth = playblock->m_islandwidth;
        int wantededgewidth = playblock->m_edgewidth;
        if (row > 19 - playblock->m_edgetransitionspeed)
        {
            int transitionindex = 20 - row;
            if (previousplayblock->m_edgewidth != playblock->m_edgewidth)
            {
                wantededgewidth = playblock->m_edgewidth + (((playblock->m_edgetransitionspeed - transitionindex) * (previousplayblock->m_edgewidth - playblock->m_edgewidth)) / playblock->m_edgetransitionspeed);
            }
        }
        if (row > 19 - playblock->m_islandtransitionspeed)
        {
            int transitionindex = 20 - row;
            // transition rows
            //     //row 17 3
            //     //row 18 2
            //     //row 19 1

            // islandtransitions
            if (previousplayblock->m_islandwidth != playblock->m_islandwidth)
            {
                wantedislandwidth = playblock->m_islandwidth + (((playblock->m_islandtransitionspeed - transitionindex) * (previousplayblock->m_islandwidth - playblock->m_islandwidth)) / playblock->m_islandtransitionspeed);
            }
        }
        for (uint16_t column = 0; column < 10; column++)
        {
            unsigned char field = 0b00000000;

            if (column >= wantededgewidth / 8)
            {
                field = 0b11111111u;
                bool shouldshifthisblock = wantededgewidth - column * 8 > 0;
                if (shouldshifthisblock)
                {
                    uint16_t shiftrightby = (wantededgewidth - column * 8);

                    field >>= shiftrightby;
                }
            }
            if (column >= 9 - wantedislandwidth / 8)
            {
                int shiftleft = wantedislandwidth - (8 * (9 - column));
                if (shiftleft > 8)
                {
                    shiftleft = 8;
                }
                field <<= wantedislandwidth - (8 * (9 - column));
            }
            if (playblock->m_hasbridge && row > 12)
            {
                if (row % 2 == 1)
                {
                    field = 0b0000000u;
                }
                else
                {
                    field = 0b1111111u;
                }
            }
            playblock->m_displaypattern[row * 10 + column] = field;
        }
    }
}

void Generate_PlayBlock(uint8_t index, bool first, bool start, bool noisland, const char seed, struct PlayBlock *previousplayblock, struct PlayBlock *generatedplayblock)
{

    if (seed % 4 == 0 || start)
    {

        generatedplayblock->m_edgewidth = 10 + (abs(seed) % 50);

        if (generatedplayblock->m_edgewidth < 25)
        {
            if (seed % 2 == 0 && !noisland)
            {
                generatedplayblock->m_islandwidth = 5 + ((abs(seed)) % generatedplayblock->m_edgewidth);
            }
            else
            {
                generatedplayblock->m_islandwidth = 0;
            }
        }
        else
        {
            generatedplayblock->m_islandwidth = 0;
        }
    }
    else
    {

        generatedplayblock->m_edgewidth = previousplayblock->m_edgewidth;
        generatedplayblock->m_islandwidth = previousplayblock->m_islandwidth;
    }
    if (first)
    {
        generatedplayblock->m_islandwidth = 0;

        if (index < 3)
        {
            generatedplayblock->m_edgewidth = 40;
        }
        else if (index == 3)
        {
            generatedplayblock->m_edgewidth = 60;
            generatedplayblock->m_hasbridge = true;
        }
        else
        {
            generatedplayblock->m_edgewidth = 60;
        }
    }
    generatedplayblock->m_edgetransitionspeed = 2 + (abs(seed) % 5);
    generatedplayblock->m_islandtransitionspeed = 2 + (abs(seed) >> 2 % 5);

    Generate_PlayBlock_Pattern(generatedplayblock, start ? generatedplayblock : previousplayblock);
}

void PlayArea_Initialize(struct PlayArea *p)
{

    p->m_y = 983040;
    p->m_x = 0;
    p->m_width = PLAY_WIDTH;
    p->m_height = PLAY_HEIGHT;
    p->m_currenttopblock = 0;
    p->m_offsetX = 0;
    p->m_offsetY = 0;
    for (uint8_t i = 0; i < 7; i++)
    {
        bool isstart = i == 0;
        bool noisland = true;
        Generate_PlayBlock(i, true, isstart, noisland, lsfr.m_lfsrvalue, &p->m_playblocks[i - 1], &p->m_playblocks[i]);

        lfsr_next(&lsfr);
        // tracef("lfsr is %x", lsfr.m_lfsrvalue);
        // tracef("playblock %x has width %d", i, p->m_playblocks[i].m_edgewidth);
    }
    // override initial playblocks with start values
}

bool PlayArea_Update(struct PlayArea *p, struct Jet *jet, int gameticks, const int ticks)
{
    p->m_previousx = p->m_x;
    p->m_previousy = p->m_y;

    p->m_y = p->m_y - jet->m_obj.m_vaccel * 2.2 * ticks;

    int previousyoffset = p->m_offsetY;

    float startRowF = (p->m_y / 4) / TILESIZEY;
    int startRow = (int)startRowF;
    p->m_offsetY = -p->m_y / 4 + startRow * TILESIZEY;
    p->m_changedy = (p->m_y / 4) - (p->m_previousy / 4);

    int32_t abspreviouslymodtwenty = abs(previousyoffset) % 20 < abs(p->m_offsetY) % 20;

    gameticks;
    if (abspreviouslymodtwenty && previousyoffset != p->m_offsetY)
    {

        if (p->m_currenttopblock == 0)
        {
            p->m_currenttopblock = 6;
        }
        else
        {
            p->m_currenttopblock--;
        }
        Generate_PlayBlock(0, false, false, false, lsfr.m_lfsrvalue, &p->m_playblocks[(p->m_currenttopblock + 1) % 7], &p->m_playblocks[p->m_currenttopblock]);

        lfsr_next(&lsfr);
        return true;
        // tracef("lfsr is %x", lsfr.m_lfsrvalue);
        // tracef("playblock %x has width %d", p->m_currenttopblock, p->m_playblocks[p->m_currenttopblock].m_edgewidth);
    }
    return false;
}
void PlayArea_NewDraw(struct PlayArea *p)
{
    // float startRowF = (p->m_y / 4) / TILESIZEY;
    // int startRow = (int)startRowF;
    // p->m_offsetY = -p->m_y / 4 + startRow * TILESIZEY;

    *DRAW_COLORS = 3;
    for (uint8_t pb = 0; pb < 7; pb++)
    {
        uint8_t wantedindex = (pb + p->m_currenttopblock) % 7;
        *DRAW_COLORS = 3;
        blit(p->m_playblocks[wantedindex].m_displaypattern, 0, (pb * 20) + p->m_offsetY, 80, 20, 0);
        blit(p->m_playblocks[wantedindex].m_displaypattern, 80, (pb * 20) + p->m_offsetY, 80, 20, BLIT_FLIP_X);
        char buffer[4];
        itoa(buffer, wantedindex);
        *DRAW_COLORS = 1;
        if (DEBUG == 1)
        {
            text(buffer, 0, (pb * 20) + p->m_offsetY);
        }
    }
}