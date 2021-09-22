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

void GameObject_Initialize(struct GameObject *o)
{
    o->m_type = OBJECT_NONE;
    o->m_alive = false;
}
void GameObject_Draw(struct GameObject *o, struct Game *game)
{
    if (o->m_type == OBJECT_SHIP && o->m_tickssincecollision > 0)
    {

        if (o->m_tickssincecollision == 1)
        {
            Sound_PlayShipCollision(game);
        }
        *DRAW_COLORS = 2;
        blit(o->m_spriteexplosion, o->m_posX, o->m_posY, o->m_width, o->m_height, BLIT_1BPP);
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
    o->m_height = o->m_height;
}
bool GameObject_CollisionDetect(struct GameObject *o, struct Ship *ship)
{
    // check if ship colliding with land by pixel
    if (o->m_type == OBJECT_FUEL)
    {
        if (Detect_BoxCollision(ship->m_obj.m_posX, ship->m_obj.m_posY, ship->m_obj.m_width, ship->m_obj.m_height, o->m_posX, o->m_posY, o->m_width, o->m_height))
        {
            return true;
        }
    }
    return false;
    // check if ship colliding with fuel by box
    // check if ship colliding with enemy by box
}