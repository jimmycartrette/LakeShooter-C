#include "gameobject.h"
#include "wasm4.h"
#include <stdbool.h>
#include "utils.h"

bool Detect_SpriteCollision(float posX, float posY, int width, int height, const char *sprite)
{

    for (int column = posX; column < posX + width; column++)
    {
        for (int row = posY; row < posY + height; row++)
        {
            // TODO add pixel check on myship.sprite
            if (sprite[row - (int)posY] & 1 << (7 - column - (int)posX))
            {
                unsigned char *checkframe = FRAMEBUFFER + row * 40 + column / 4;

                int val = (*checkframe) >> (2 ^ (2 - (column % 4)));
                trace("val is");
                char colorvalue[10];
                itoa(colorvalue, val);
                trace(colorvalue);
                if ((val & 3) == 2)
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
void GameObject_Draw(struct GameObject *o)
{

    *DRAW_COLORS = 2;
    blit(o->m_dir == DIRECTION_UP ? o->m_spritefacingup : o->m_spritefacingright1, o->m_posX, o->m_posY, o->m_width, o->m_height, BLIT_1BPP | (o->m_dir == DIRECTION_LEFT ? BLIT_FLIP_X : 0));
}
void GameObject_Update(struct GameObject *o)
{
    if (o->m_type == OBJECT_SHIP || o->m_type == OBJECT_BULLET)
    {
        if (Detect_SpriteCollision(o->m_posX, o->m_posY, o->m_width, o->m_height, o->m_spritefacingup))
        {
            o->m_tickssincecollision++;
        }
    }
}