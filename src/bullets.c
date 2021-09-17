#include "bullets.h"
#include "playarea.h"
#include "wasm4.h"
#include "ship.h"
#include "utils.h"

const char BULLET[3] = {
    0b11101111,
    0b11101111,
    0b11101111,
};

void Bullets_Init(struct Bullets *bullets)
{
    for (int n = 0; n < MAXBULLETS; n++)
    {
        bullets->bullet[n].m_obj.m_posX = 0;
        bullets->bullet[n].m_obj.m_posY = 0;
        bullets->bullet[n].m_obj.m_alive = false;
    }
}
void Bullets_GenerateBullet(struct Bullets *bullets, struct Ship *ship)
{

    int n;
    for (n = 0; n < MAXBULLETS; n++)
    {
        if (!bullets->bullet[n].m_obj.m_alive)
        {
            bullets->bullet[n].m_obj.m_alive = true;
            bullets->bullet[n].m_obj.m_posX = ship->m_obj.m_posX;
            bullets->bullet[n].m_obj.m_posY = ship->m_obj.m_posY;
            return;
        }
    }
}
void Bullets_Update(struct Bullets *bullets, struct PlayArea *playarea, struct Ship *ship)
{
    int n;
    for (n = 0; n < MAXBULLETS; n++)
        if (bullets->bullet[n].m_obj.m_alive)
        {
            if (bullets->bullet[n].m_obj.m_posY < playarea->m_offsetY)
            {
                bullets->bullet[n].m_obj.m_alive = false;
            }
            else
            {
                bullets->bullet[n].m_obj.m_posY -= 4;
                bullets->bullet[n].m_obj.m_posX += (ship->m_obj.m_posX - ship->m_obj.m_prePosX);
            }
        }
}
void Bullets_Draw(struct Bullets *bullets)
{
    int n;
    for (n = 0; n < MAXBULLETS; n++)
        if (bullets->bullet[n].m_obj.m_alive)
        {
            *DRAW_COLORS = 3;
            blit(BULLET, bullets->bullet[n].m_obj.m_posX, bullets->bullet[n].m_obj.m_posY, 8, 3, BLIT_1BPP);
        }
}
void Bullets_CollisionDetect(struct Bullets *bullets)
{
    bullets->bullet->m_obj.m_vaccel = 1;
}