#include "gameobject.h"
#include "wasm4.h"
#include <stdbool.h>
#include "utils.h"
#include "sound.h"
#include "ship.h"

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
            if (o->m_type == OBJECT_SHIP)
            {
                Sound_PlayShipCollision(game);
            }
            else
            {
                Sound_PlayOtherCollision(game);
            }
        }
        *DRAW_COLORS = 2;
        if (o->m_type == OBJECT_SHIP)
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
void GameObject_Update(struct GameObject *o)
{
    if (o->m_type != OBJECT_SHIP)
    {
        if (o->m_tickssincecollision > 0)
        {
            if (o->m_tickssincecollision > 60)
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
}
bool GameObject_CollisionDetect(struct GameObject *o1, struct GameObject *o2)
{

    if (Detect_BoxCollision(o2->m_posX, o2->m_posY, o2->m_width, o2->m_height, o1->m_posX, o1->m_posY, o1->m_width, o1->m_height))
    {
        return true;
    }
    return false;
}