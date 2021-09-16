#include "ship.h"
#include "global.h"

const char SHIPFACINGUP[8] = {
    0b11101111,
    0b11000111,
    0b10000011,
    0b00000001,
    0b00101001,
    0b01101101,
    0b11000111,
    0b00101001,
};
const char SHIPFACINGRIGHT[8] = {
    0b11001111,
    0b10000111,
    0b00000011,
    0b00001101,
    0b01001111,
    0b10001111,
    0b10000011,
    0b01011011,
};

void Ship_Initialize(struct Ship *s)
{
    struct GameObject *o = &s->m_obj;
    GameObject_Initialize(o);

    o->m_type = OBJECT_SHIP;
    o->m_dir = DIRECTION_UP;
    o->m_width = 8;
    o->m_height = 8;
    o->m_posX = PLAY_WIDTH / 2 - o->m_width / 2;
    o->m_posY = PLAY_HEIGHT - 10;
    o->m_prePosX = o->m_posX;
    o->m_prePosY = o->m_posY;
    o->m_haccel = 0;
    o->m_vaccel = 1;
    o->m_spritefacingup = &SHIPFACINGUP[0];
}