#include "ship.h"
#include "global.h"
#include "wasm4.h"

const char JETSTRAIGHT[16] = {
    0xF7, // |    X   |
    0xF7, // |    X   |
    0xF7, // |    X   |
    0xE3, // |   XXX  |
    0xC1, // |  XXXXX |
    0x80, // | XXXXXXX|
    0x80, // | XXXXXXX|
    0x94, // | XX X XX|
    0xB6, // | X  X  X|
    0xF7, // |    X   |
    0xE3, // |   XXX  |
    0xC1, // |  XXXXX |
    0xD5, // |  X X X |
    0xFF, // |        |
    0xFF, // |        |
    0xFF, // |        |
};

const char JETMOVE[16] = {
    0xF7, // |    X   |
    0xF7, // |    X   |
    0xF7, // |    X   |
    0xE7, // |   XX   |
    0xC3, // |  XXXX  |
    0xC1, // |  XXXXX |
    0xC1, // |  XXXXX |
    0xD1, // |  X XXX |
    0xF5, // |    X X |
    0xF7, // |    X   |
    0xE7, // |   XX   |
    0xC3, // |  XXXX  |
    0xD1, // |  X XXX |
    0xFD, // |      X |
    0xFF, // |        |
    0xFF, // |        |
};

const char JETEXPLOSION[16] = {
    0xFF,
    0xF7,
    0xFB,
    0xEF,
    0xBB,
    0xDE, 0xF7, 0xBF, 0xFF, 0xEF, 0xF7, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF

};

void Ship_Initialize(struct Ship *s)
{
    struct GameObject *o = &s->m_obj;
    GameObject_Initialize(o);

    o->m_type = OBJECT_SHIP;
    o->m_dir = DIRECTION_UP;
    o->m_width = 8;
    o->m_height = 16;
    o->m_posX = PLAY_WIDTH / 2 - o->m_width / 2;
    o->m_posY = PLAY_HEIGHT - 15;
    o->m_prePosX = o->m_posX;
    o->m_prePosY = o->m_posY;
    o->m_tickssincecollision = 0;
    o->m_haccel = 0;
    o->m_vaccel = 1;
    o->m_spritefacingup = &JETSTRAIGHT[0];
    o->m_spritefacingright1 = &JETMOVE[0];
    o->m_spriteexplosion = &JETEXPLOSION[0];
}
void Ship_Update(struct Ship *s, struct Input *i)
{
    s->m_obj.m_prePosX = s->m_obj.m_posX;
    s->m_obj.m_prePosY = s->m_obj.m_posY;
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
void Ship_Draw(struct Ship *s, struct Game *game)
{
    GameObject_Draw(&s->m_obj, game);
}
void Ship_CollisionDetect(struct Ship *s)
{
    GameObject_CollisionDetect(&s->m_obj);
}