#pragma once
#include "stdint.h"
#include <stdbool.h>

enum ObjectType
{
    OBJECT_NONE = 0,
    OBJECT_JET = 1,
    OBJECT_BULLET = 2,
    OBJECT_FUEL = 3,
    OBJECT_SHIP = 4,
    OBJECT_HELI = 5
};

enum Direction
{
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
};
struct Game;
struct Jet;

struct GameObject
{
    uint8_t m_type;
    double m_posX, m_posY;
    uint8_t m_width, m_height;
    double m_prePosX, m_prePosY;
    float m_vaccel, m_haccel;
    uint8_t m_tickssincecollision;
    uint8_t m_scoreworth;
    bool m_startedMoving;
    uint8_t m_islandwidth;
    uint8_t m_edgewidth;
    enum Direction m_dir;
    bool m_animated;
    bool m_alive;
    char const *m_spritefacingright1;
    char const *m_spritefacingright2;
    char const *m_spritefacingup;
    char const *m_spriteexplosion;
};

void GameObject_Initialize(struct GameObject *o);
void GameObject_Update(struct GameObject *o, struct Game *game);
void GameObject_Draw(struct GameObject *o, struct Game *game);
bool GameObject_CollisionDetect(struct GameObject *o1, struct GameObject *o2);
void GameObject_StartExplosion(struct GameObject *o1);
bool Detect_SpriteCollision(float posX, float posY, int width, int height, const char *sprite);
bool Detect_PixelCollision(int posX, int posY);
bool Detect_BoxCollision(float posXA, float posYA, int widthA, int heightA, float posXB, float posYB, int widthB, int heightB);