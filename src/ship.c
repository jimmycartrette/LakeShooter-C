#include "ship.h"
#include "global.h"
#include "wasm4.h"

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
    o->m_spritefacingright1 = &SHIPFACINGRIGHT[0];
}
void Ship_Update(struct Ship *s, struct Input *i)
{
    if (!(Input_GamepadLeftHeld(i) || Input_GamepadRightHeld(i)))
    {
        s->m_obj.m_haccel = 0;
        s->m_obj.m_dir = DIRECTION_UP;
    }
    else
    {
        if (s->m_obj.m_haccel < 1)
        {
            s->m_obj.m_haccel += .04;
        }
        else if (s->m_obj.m_haccel < .3)
        {
            s->m_obj.m_haccel += .03;
        }
    }
    if (Input_GamepadLeftHeld(i))
    {
        s->m_obj.m_dir = DIRECTION_LEFT;
        if (s->m_obj.m_posX - 1 > 0)
        {
            s->m_obj.m_posX = s->m_obj.m_posX - s->m_obj.m_haccel;
        }
    }
    if (Input_GamepadRightHeld(i))
    {
        s->m_obj.m_dir = DIRECTION_RIGHT;
        if (s->m_obj.m_posX + 1 < PLAY_WIDTH - s->m_obj.m_width)
        {
            s->m_obj.m_posX = s->m_obj.m_posX + s->m_obj.m_haccel;
        }
    }
    if (!(Input_GamepadUpHeld(i) || Input_GamepadDownHeld(i)))
    {
        if (s->m_obj.m_vaccel > 1)
        {
            s->m_obj.m_vaccel -= .04;
        }
        else
        {
            s->m_obj.m_vaccel += .04;
        }
    }
    if (Input_GamepadUpHeld(i))
    {
        if (s->m_obj.m_vaccel < 2)
        {
            s->m_obj.m_vaccel += .04;
        }
        else if (s->m_obj.m_vaccel < 1.3)
        {
            s->m_obj.m_vaccel += .03;
        }
    }
    if (Input_GamepadDownHeld(i))
    {
        if (s->m_obj.m_vaccel > .5)
        {
            s->m_obj.m_vaccel -= .04;
        }
        else if (s->m_obj.m_vaccel > .8)
        {
            s->m_obj.m_vaccel -= .03;
        }
    }
    GameObject_Update(&s->m_obj);
}
void Ship_Draw(struct Ship *s)
{
    GameObject_Draw(&s->m_obj);
}