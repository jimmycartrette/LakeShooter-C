#include "fuel.h"
#include "wasm4.h"
#include "playarea.h"
#include "game.h"
#include "global.h"
#include "sound.h"
#include "utils.h"

const char FUELSPRITE[23] = {
    0b00000000,
    0b00101011,
    0b01101011,
    0b00101011,
    0b01100011,
    0b11111111,
    0b00101111,
    0b01101111,
    0b00101111,
    0b01101111,
    0b00100011,
    0b00000000,
    0b00101011,
    0b01101011,
    0b00101011,
    0b01100011,
    0b11111111,
    0b00101111,
    0b01101111,
    0b00101111,
    0b01101111,
    0b00100011,
    0b00000000,

};

const char FUELEXPLOSION[8] = {
    0b11111101,
    0b01111111,
    0b11101111,
    0b10111111,
    0b11111101,
    0b11011111,
    0b10111101,
    0b11101111,
};

void Fuels_Initialize(struct Fuels *fuels)
{
    for (int n = 0; n < MAXFUELS; n++)
    {
        fuels->fuel[n].m_obj.m_posX = 0;
        fuels->fuel[n].m_obj.m_posY = 0;
        fuels->fuel[n].m_obj.m_width = 7;
        fuels->fuel[n].m_obj.m_height = 23;
        fuels->fuel[n].m_obj.m_type = OBJECT_FUEL;
        fuels->fuel[n].m_obj.m_alive = false;
    }
}
void Fuels_Create(struct Fuels *fuels, struct PlayArea *p)
{
    for (int i = 0; i < MAXFUELS; i++)
    {
        if (!fuels->fuel[i].m_obj.m_alive)
        {
            fuels->fuel[i].m_obj.m_alive = true;
            fuels->fuel[i].m_obj.m_width = 7;
            fuels->fuel[i].m_obj.m_height = 23;
            fuels->fuel[i].m_obj.m_tickssincecollision = 0;
            fuels->fuel[i].m_obj.m_posX = p->m_playblocks[p->m_currenttopblock].m_edgewidth + 1 + (lsfr.m_lfsrvalue % (80 - fuels->fuel[i].m_obj.m_width - p->m_playblocks[p->m_currenttopblock].m_islandwidth - p->m_playblocks[p->m_currenttopblock].m_edgewidth));
            if (lsfr.m_lfsrvalue % 2 == 0)
            {
                fuels->fuel[i].m_obj.m_posX = 160 - fuels->fuel[i].m_obj.m_posX;
            }
            int toint = fuels->fuel[i].m_obj.m_posX;
            fuels->fuel[i].m_obj.m_posY = p->m_offsetY;

            return;
        }
    }
}
void Fuels_Update(struct Fuels *fuels, struct PlayArea *p)
{
    int n;
    for (n = 0; n < MAXFUELS; n++)
        if (fuels->fuel[n].m_obj.m_alive)
        {
            GameObject_Update(&fuels->fuel[n].m_obj);
            fuels->fuel[n].m_obj.m_posY -= p->m_changedy;
            if (fuels->fuel[n].m_obj.m_posY > 120)
            {
                fuels->fuel[n].m_obj.m_alive = false;
                // tracef("fuel cleanup %d", n);
            }
        }
}
void Fuels_Draw(struct Fuels *fuels)
{
    int n;
    for (n = 0; n < MAXFUELS; n++)
    {
        if (fuels->fuel[n].m_obj.m_alive)
        {
            if (fuels->fuel[n].m_obj.m_tickssincecollision == 0)
            {
                *DRAW_COLORS = 40;
                blit(FUELSPRITE, fuels->fuel[n].m_obj.m_posX, fuels->fuel[n].m_obj.m_posY, fuels->fuel[n].m_obj.m_width, fuels->fuel[n].m_obj.m_height, BLIT_1BPP);
                // char buffer[4];
                // itoa(buffer, n);
                // if (DEBUG == 1)
                // {
                //     //text(buffer, fuels->fuel[n].m_obj.m_posX + 1, fuels->fuel[n].m_obj.m_posY);
                // }
            }
            else
            {
                *DRAW_COLORS = 4;
                blit(FUELEXPLOSION, fuels->fuel[n].m_obj.m_posX, fuels->fuel[n].m_obj.m_posY, fuels->fuel[n].m_obj.m_width, fuels->fuel[n].m_obj.m_height, BLIT_1BPP);
            }
        }
    }
}
void Fuels_AndShipCollisionDetect(struct Fuels *fuels, struct Ship *ship, struct Game *game)
{
    int n;
    for (n = 0; n < MAXFUELS; n++)
        if (fuels->fuel[n].m_obj.m_alive)
        {
            if (GameObject_CollisionDetect(&fuels->fuel[n].m_obj, &ship->m_obj) && fuels->fuel[n].m_obj.m_tickssincecollision == 0)
            {

                if (ship->fuelingtickscountdown == 24)
                {
                    Sound_PlayFuelUp(game);
                }
                else
                {
                    ship->fuelingtickscountdown = 25;
                }

                if (game->m_fuellevel <= 9950)
                {
                    game->m_fuellevel += 50;
                }
            }
        }
}