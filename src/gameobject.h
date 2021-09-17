#pragma once
#include "stdint.h"
#include <stdbool.h>

enum ObjectType
{
    OBJECT_NONE = 0,
    OBJECT_SHIP = 1,
    OBJECT_BULLET = 2
};

enum Direction
{
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
};

struct GameObject
{
    uint8_t m_type;
    float m_posX, m_posY;
    uint8_t m_width, m_height;
    float m_prePosX, m_prePosY;
    float m_vaccel, m_haccel;
    uint8_t m_tickssincecollision;
    enum Direction m_dir;
    bool m_animated;
    bool m_alive;
    char const *m_spritefacingright1;
    char const *m_spritefacingright2;
    char const *m_spritefacingup;
};

void GameObject_Initialize(struct GameObject *o);
void GameObject_Update(struct GameObject *o);
void GameObject_Draw(struct GameObject *o);