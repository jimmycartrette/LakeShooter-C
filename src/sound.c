#include "sound.h"
#include "wasm4.h"

void Sound_PlayBackgroundNoise(struct Game *game)
{
    if (game->m_ticks % 20 == 0)
    {
        tone(320 + (int)(game->m_ship.m_obj.m_vaccel * (float)40) | (0 << 16), (0 << 24) | (0 << 16) | 20 | (0 << 8), 34, 3 | (0 << 2));
    }
}
void Sound_PlayShipCollision()
{

    tone(250 | (0 << 16), (8 << 24) | (10 << 16) | 8 | (8 << 8), 34, 3 | (0 << 0));
}