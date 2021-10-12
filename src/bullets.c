#include "bullets.h"
#include "playarea.h"
#include "wasm4.h"
#include "jet.h"
#include "utils.h"
#include "sound.h"

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
        bullets->bullet[n].m_obj.m_width = 8;
        bullets->bullet[n].m_obj.m_height = 3;
        bullets->bullet[n].m_obj.m_alive = false;
        //bullets->bullet[n].m_obj.m_spritefacingup = BULLET;
    }
}
void Bullets_GenerateBullet(struct Bullets *bullets, struct Jet *jet, struct Game *game)
{

    int n;
    for (n = 0; n < MAXBULLETS; n++)
    {
        if (!bullets->bullet[n].m_obj.m_alive)
        {
            bullets->bullet[n].m_obj.m_alive = true;
            bullets->bullet[n].m_obj.m_width = 8;
            bullets->bullet[n].m_obj.m_height = 3;
            bullets->bullet[n].m_obj.m_posX = jet->m_obj.m_posX;
            bullets->bullet[n].m_obj.m_posY = jet->m_obj.m_posY + 6;
            Sound_PlayBulletShoot(game);
            return;
        }
    }
}
void Bullets_Update(struct Bullets *bullets, struct PlayArea *playarea, struct Jet *jet)
{
    int n;
    for (n = 0; n < MAXBULLETS; n++)
    {
        if (bullets->bullet[n].m_obj.m_alive)
        {

            GameObject_Update(&bullets->bullet[n].m_obj);
            if (bullets->bullet[n].m_obj.m_posY < playarea->m_offsetY)
            {
                bullets->bullet[n].m_obj.m_alive = false;
            }
            else
            {
                bullets->bullet[n].m_obj.m_posY -= 4;
                bullets->bullet[n].m_obj.m_posX += (jet->m_obj.m_posX - jet->m_obj.m_prePosX);
            }
        }
    }
}
void Bullets_Draw(struct Bullets *bullets)
{
    for (int n = 0; n < MAXBULLETS; n++)
        if (bullets->bullet[n].m_obj.m_alive)
        {
            *DRAW_COLORS = 2;
            blit(BULLET, bullets->bullet[n].m_obj.m_posX, bullets->bullet[n].m_obj.m_posY, bullets->bullet[n].m_obj.m_width, bullets->bullet[n].m_obj.m_height, BLIT_1BPP);
        }
}
void Bullets_CollisionDetect(struct Bullets *bullets, struct Game *game)
{
    for (int i = 0; i < MAXBULLETS; i++)
    {
        if (bullets->bullet[i].m_obj.m_alive)
        {

            // check land
            if (bullets->bullet[i].m_obj.m_posY > 0 && Detect_PixelCollision(bullets->bullet[i].m_obj.m_posX, bullets->bullet[i].m_obj.m_posY))
            {
                bullets->bullet[i].m_obj.m_alive = false;
                continue;
            }
            // check fuel
            for (int f = 0; f < MAXFUELS; f++)
            {
                if (game->m_fuels.fuel[f].m_obj.m_alive && game->m_fuels.fuel[f].m_obj.m_tickssincecollision == 0)
                {
                    // tracef("b %d fuel %d check", i, f);
                    if (GameObject_CollisionDetect(&game->m_fuels.fuel[f].m_obj, &bullets->bullet[i].m_obj))
                    {
                        //   tracef("fuel collision with bullet %d", f);
                        bullets->bullet[i]
                            .m_obj.m_alive = false;
                        GameObject_StartExplosion(&game->m_fuels.fuel[f].m_obj);
                        break;
                    }
                }
            }

            // check enemies
        }
    }
}