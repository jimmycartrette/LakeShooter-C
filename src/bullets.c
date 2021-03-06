#include "bullets.h"
#include "playarea.h"
#include "wasm4.h"
#include "jet.h"
#include "utils.h"
#include "sound.h"
#include "global.h"

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
void Bullets_Update(struct Bullets *bullets, struct PlayArea *playarea, struct Jet *jet, struct Game *game)
{
    int n;
    for (n = 0; n < MAXBULLETS; n++)
    {
        if (bullets->bullet[n].m_obj.m_alive)
        {

            GameObject_Update(&bullets->bullet[n].m_obj, game);
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
void Bullets_Anything_CollisionDetect(struct Bullets *bullets, struct Game *game)
{
    for (int i = 0; i < MAXBULLETS; i++)
    {
        if (bullets->bullet[i].m_obj.m_alive)
        {
            for (uint8_t x = 0; x < 7; x++)
            {
                if (game->m_playarea.m_playblocks[x].m_hasbridge == true)
                {
                    uint8_t topblock = game->m_playarea.m_currenttopblock;

                    int8_t attempt = (8 + x - topblock) % 7;
                    // tracef("topblock is %d block wbridge is %d attempt is %d", topblock, x, attempt);
                    uint8_t bridgeY = (attempt * 20) - abs(game->m_playarea.m_offsetY);
                    // tracef("bridgey is %d and bullety is %d", bridgeY, (int)bullets->bullet[i].m_obj.m_posY);

                    if (bridgeY > 40 && bullets->bullet[i].m_obj.m_posY < bridgeY)
                    {
                        bullets->bullet[i].m_obj.m_alive = false;
                        game->m_playarea.m_playblocks[x].m_hasbridge = false;
                        if (game->m_playarea.m_currentblockindex > 12)
                        {
                            game->m_savedlsfr = lsfr.m_lfsrvalue;
                            //   tracef("saving lsfr as %d", game->m_savedlsfr);
                        }
                        game->m_playarea.m_bridgeblockindex += 40;
                        game->m_levelblocksrendered = 0;
                        game->m_level++;
                        Generate_PlayBlock(game->m_playarea.m_currentblockindex, false, false, true, lsfr.m_lfsrvalue, &game->m_playarea.m_playblocks[(x + 1) % 7], &game->m_playarea.m_playblocks[x], game->m_playarea.m_bridgeblockindex);
                    }
                }
            }

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
            for (int f = 0; f < MAXENEMYS; f++)
            {
                if (game->m_enemys.enemy[f].m_obj.m_alive && game->m_enemys.enemy[f].m_obj.m_tickssincecollision == 0)
                {
                    // tracef("b %d enemy %d check", i, f);
                    if (GameObject_CollisionDetect(&game->m_enemys.enemy[f].m_obj, &bullets->bullet[i].m_obj))
                    {

                        bullets->bullet[i]
                            .m_obj.m_alive = false;
                        game->m_score = game->m_score + game->m_enemys.enemy[f].m_obj.m_scoreworth;
                        GameObject_StartExplosion(&game->m_enemys.enemy[f].m_obj);

                        break;
                    }
                }
            }
        }
    }
}