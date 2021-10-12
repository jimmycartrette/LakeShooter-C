#pragma once
#include "game.h"

struct SoundEffect
{
    uint16_t m_freq1;
    uint16_t m_freq2;
    uint16_t m_attack;
    uint16_t m_decay;
    uint16_t m_sustain;
    uint16_t m_release;
    uint16_t m_volume;
    uint8_t m_channel;
    uint8_t m_mode;
};

void Sound_PlayBackgroundNoise(struct Game *game);
void Sound_PlayJetCollision(struct Game *game);
void Sound_PlayOtherCollision(struct Game *game);
void Sound_PlayBulletShoot(struct Game *game);
void Sound_PlayFuelUp(struct Game *game);
void Sound_PlayFuelAlarm(struct Game *game);