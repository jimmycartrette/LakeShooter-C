#include "gameobject.h"
#include "wasm4.h"
#include <stdbool.h>
#include "utils.h"
#include "sound.h"
#include "jet.h"

const char EXPLOSION[8] = {
    0b11111101,
    0b01111111,
    0b11101111,
    0b10111111,
    0b11111101,
    0b11011111,
    0b10111101,
    0b11101111,
};

bool Detect_BoxCollision(float posXA, float posYA, int widthA, int heightA, float posXB, float posYB, int widthB, int heightB)
{
    if (posXA + widthA > posXB && posXA < posXB + widthB && posYA + heightA > posYA && posYA < posYB + heightB)
    {
        return true;
    }
    return false;
}
bool Detect_PixelCollision(int posX, int posY)
{

    unsigned char *checkframe = FRAMEBUFFER + posY * 40 + posX / 4;

    int val = (*checkframe) >> (2 ^ (2 - (posX % 4)));
    if ((val & 3) == 2) // if hit green
    {

        return true;
    }
    return false;
}
bool Detect_SpriteCollision(float posX, float posY, int width, int height, const char *sprite)
{

    for (int column = posX; column < posX + width; column++)
    {

        for (int row = posY; row < posY + height; row++)
        {
            if (sprite[row - (int)posY] & 1 << (7 - column - (int)posX))
            {
                unsigned char *checkframe = FRAMEBUFFER + row * 40 + column / 4;

                int val = (*checkframe) >> (2 ^ (2 - (column % 4)));
                if ((val & 3) == 2) // if hit green
                {

                    return true;
                }
            }
        }
    }
    return false;
}
void GameObject_Initialize(struct GameObject *o)
{
    o->m_type = OBJECT_NONE;
    o->m_alive = false;
}
void GameObject_Draw(struct GameObject *o, struct Game *game)
{
    if (o->m_tickssincecollision > 0)
    {

        if (o->m_tickssincecollision == 1)
        {
            if (o->m_type == OBJECT_JET)
            {
                Sound_PlayJetCollision(game);
            }
            else
            {
                Sound_PlayOtherCollision(game);
            }
        }
        o->m_tickssincecollision++;
        *DRAW_COLORS = 2;
        if (o->m_type == OBJECT_JET)
        {
            blit(o->m_spriteexplosion, o->m_posX, o->m_posY, o->m_width, o->m_height, BLIT_1BPP);
        }
        else
        {
            blit(EXPLOSION, o->m_posX, o->m_posY, 8, 8, BLIT_1BPP);
        }
        return;
    }
    else
    {
        *DRAW_COLORS = 2;
        blit(o->m_dir == DIRECTION_UP ? o->m_spritefacingup : o->m_spritefacingright1, o->m_posX, o->m_posY, o->m_width, o->m_height, BLIT_1BPP | (o->m_dir == DIRECTION_LEFT ? BLIT_FLIP_X : 0));
    }
}
void GameObject_Update(struct GameObject *o, struct Game *g)
{
    if (o->m_type != OBJECT_JET)
    {
        if (o->m_tickssincecollision > 0)
        {
            if (o->m_tickssincecollision > 14)
            {
                o->m_alive = false;
                o->m_tickssincecollision = 0;
            }
            else
            {
                o->m_tickssincecollision++;
            }
        }
    }
    if (o->m_startedMoving && o->m_tickssincecollision == 0)
    {
        if (g->m_ticks % 2 == 0)
        {
            if (o->m_dir == DIRECTION_LEFT)
            {
                if (o->m_posX-- <= o->m_edgewidth - 1 || (o->m_islandwidth > 0 && o->m_posX > 80 && o->m_posX-- <= 80 + o->m_islandwidth + 1))
                {
                    o->m_dir = DIRECTION_RIGHT;
                }
                else
                {
                    o->m_posX--;
                }
            }
            if (o->m_dir == DIRECTION_RIGHT)
            {
                if (o->m_posX++ >= 160 - o->m_width - o->m_edgewidth - 1 || (o->m_islandwidth > 0 && o->m_posX < 80 && o->m_posX++ >= 80 - o->m_islandwidth - 1))
                {
                    o->m_dir = DIRECTION_LEFT;
                }
                else
                {
                    o->m_posX++;
                }
            }
        }
    }
}
bool GameObject_CollisionDetect(struct GameObject *o1, struct GameObject *o2)
{

    if (Detect_BoxCollision(o2->m_posX, o2->m_posY, o2->m_width, o2->m_height, o1->m_posX, o1->m_posY, o1->m_width, o1->m_height))
    {
        return true;
    }
    return false;
}
void GameObject_StartExplosion(struct GameObject *o1)
{
    o1->m_spriteexplosion = EXPLOSION;
    o1->m_posX = o1->m_posX + (o1->m_width / 2) - 4;
    o1->m_posY = o1->m_posY + (o1->m_height / 2) - 4;
    o1->m_width = 8;
    o1->m_height = 8;

    o1->m_tickssincecollision++;
}