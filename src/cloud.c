#include "cloud.h"
#include "game.h"
#include "utils.h"
#include "playarea.h"
#include "global.h"
#include "wasm4.h"

const int CLOUDWIDTH = 8;
const int CLOUDHEIGHT = 8;

const char CLOUDS[3][8] = {
    {
        0b11000011,
        0b10000011,
        0b00000011,
        0b10000001,
        0b11000001,
        0b11100000,
        0b11110001,
        0b11111011,
    },
    {
        0b11111111,
        0b11000001,
        0b11000011,
        0b10000001,
        0b00000001,
        0b10000000,
        0b00010001,
        0b00111111,
    },
    {
        0b11001001,
        0b10000001,
        0b10000011,
        0b11000011,
        0b00001101,
        0b10001100,
        0b11010001,
        0b11111111,
    },
};
void Clouds_Init(struct Clouds *clouds)
{
    for (int n = 0; n < MAXCLOUDS; n++)
    {
        clouds->cloud[n].m_obj.m_posX = 0;
        clouds->cloud[n].m_obj.m_posY = 0;
        clouds->cloud[n].m_obj.m_width = CLOUDWIDTH;
        clouds->cloud[n].m_obj.m_height = CLOUDHEIGHT;
        clouds->cloud[n].m_obj.m_alive = false;
    }
}
void Clouds_GenerateCloud(struct Game *game)
{
    for (int n = 0; n < MAXCLOUDS; n++)
    {
        if (!game->m_clouds.cloud[n].m_obj.m_alive)
        {
            game->m_clouds.cloud[n].m_obj.m_alive = true;
            game->m_clouds.cloud[n].m_cloudindex = (lsfr.m_lfsrvalue >> 6) % 3;
            uint8_t speedmod = (lsfr.m_lfsrvalue >> 8) % 3;
            game->m_clouds.cloud[n].m_speedmodifier = speedmod == 0 ? SPEED_SLOW : speedmod == 1 ? SPEED_FAST
                                                                                                 : SPEED_NORMAL;
            game->m_clouds.cloud[n].m_hflip = (lsfr.m_lfsrvalue >> 6) % 2;
            game->m_clouds.cloud[n].m_vflip = (lsfr.m_lfsrvalue >> 1) % 2;
            game->m_clouds.cloud[n].m_obj.m_width = CLOUDWIDTH;
            game->m_clouds.cloud[n].m_obj.m_height = CLOUDHEIGHT;
            game->m_clouds.cloud[n].m_obj.m_posY = game->m_playarea.m_offsetY;
            if ((lsfr.m_lfsrvalue >> 5) % 2 == 0)
            {
                game->m_clouds.cloud[n].m_obj.m_posX = lsfr.m_lfsrvalue % (game->m_playarea.m_playblocks[game->m_playarea.m_currenttopblock].m_edgewidth - CLOUDWIDTH);
            }
            else
            {
                game->m_clouds.cloud[n].m_obj.m_posX = 160 - (lsfr.m_lfsrvalue % (game->m_playarea.m_playblocks[game->m_playarea.m_currenttopblock].m_edgewidth - CLOUDWIDTH));
            }

            return;
        }
    }
}
void Clouds_Update(struct Clouds *clouds, struct Game *game)
{
    for (int n = 0; n < MAXCLOUDS; n++)
    {
        if (clouds->cloud[n].m_speedmodifier == SPEED_SLOW && game->m_ticks % 2)
        {

            return;
        }

        clouds->cloud[n].m_obj.m_posY -= game->m_playarea.m_changedy;
        if (clouds->cloud[n].m_speedmodifier == SPEED_FAST)
        {
            {
                clouds->cloud[n].m_obj.m_posY -= game->m_playarea.m_changedy;
            }
            if (clouds->cloud[n].m_obj.m_posY > 120)
            {
                clouds->cloud[n].m_obj.m_alive = false;
            }
        }
    }
}

void Clouds_Draw(struct Clouds *clouds)
{
    for (int n = 0; n < MAXCLOUDS; n++)
    {
        if (clouds->cloud[n].m_obj.m_alive)
        {

            *DRAW_COLORS = 4;

            blit(CLOUDS[clouds->cloud[n].m_cloudindex], clouds->cloud[n].m_obj.m_posX, clouds->cloud[n].m_obj.m_posY, clouds->cloud[n].m_obj.m_width, clouds->cloud[n].m_obj.m_height, BLIT_1BPP | (clouds->cloud[n].m_hflip ? BLIT_FLIP_X : 0) | (clouds->cloud[n].m_vflip ? BLIT_FLIP_Y : 0));
        }
    }
}
