#include "fuel.h"
#include "wasm4.h"
#include "playarea.h"
#include "game.h"
#include "sound.h"

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
void Fuels_Create(struct Fuels *fuels, int x, int y)
{

    for (int i = 0; i < MAXFUELS; i++)
    {
        if (!fuels->fuel[i].m_obj.m_alive)
        {
            fuels->fuel[i].m_obj.m_alive = true;
            fuels->fuel[i].m_obj.m_posX = x;
            fuels->fuel[i].m_obj.m_posY = y;

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
            fuels->fuel[n].m_obj.m_posY -= p->m_changedy;

            if (fuels->fuel[n].m_obj.m_posY > 120)
            {
                fuels->fuel[n].m_obj.m_alive = false;
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

            *DRAW_COLORS = 40;
            blit(FUELSPRITE, fuels->fuel[n].m_obj.m_posX, fuels->fuel[n].m_obj.m_posY, fuels->fuel[n].m_obj.m_width, fuels->fuel[n].m_obj.m_height, BLIT_1BPP);
        }
    }
}
void Fuels_CollisionDetect(struct Fuels *fuels, struct Ship *ship, struct Game *game)
{
    int n;
    for (n = 0; n < MAXFUELS; n++)
        if (fuels->fuel[n].m_obj.m_alive)
        {
            if (GameObject_CollisionDetect(&fuels->fuel[n].m_obj, ship))
            {

                if (ship->fuelingtickscountdown == 24)
                {
                    Sound_PlayFuelUp(game);
                }
                else
                {
                    ship->fuelingtickscountdown = 25;
                }

                if (game->m_fuellevel > 995)
                {
                    game->m_fuellevel = 1000;
                }
                if (game->m_fuellevel <= 995)
                {
                    game->m_fuellevel += 5;
                }
            }
        }
}