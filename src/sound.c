#include "sound.h"
#include "wasm4.h"
#include "game.h"

void PlaySoundEffect(struct SoundEffect *se, bool isbackground, struct Game *game)
{
    // if (isbackground && game->m_soundeffectcountdown == 0)
    // {
    //     return;
    // }
    tone(se->m_freq1 | se->m_freq2 << 16,
         se->m_attack << 24 | se->m_decay << 16 | se->m_sustain | se->m_release << 8,
         se->m_volume,
         se->m_channel | se->m_mode << 2);

    if (game->m_soundeffectcountdown > 0)
    {
        game->m_soundeffectcountdown--;
    }
    if (!isbackground)
    {
        game->m_soundeffectcountdown = se->m_sustain;
    }
}

void Sound_PlayBackgroundNoise(struct Game *game)
{

    if (game->m_ticks % 20 == 0)

    {
        struct SoundEffect bgnoise = {.m_freq1 = 320, .m_sustain = 20, .m_volume = 32, .m_channel = 3, .m_mode = 2};
        bgnoise.m_freq1 += (int)game->m_jet.m_obj.m_vaccel * (float)40;

        PlaySoundEffect(&bgnoise, true, game);
    }
}
void Sound_PlayJetCollision(struct Game *game)
{
    struct SoundEffect jetcollision = {.m_freq1 = 230, .m_attack = 8, .m_decay = 10, .m_sustain = 8, .m_release = 8, .m_volume = 34, .m_channel = 3};

    PlaySoundEffect(&jetcollision, false, game);
}
void Sound_PlayOtherCollision(struct Game *game)
{
    struct SoundEffect jetcollision = {.m_freq1 = 260, .m_attack = 4, .m_decay = 6, .m_sustain = 5, .m_release = 4, .m_volume = 34, .m_channel = 3};

    PlaySoundEffect(&jetcollision, false, game);
}
void Sound_PlayBulletShoot(struct Game *game)
{
    struct SoundEffect bulletshoot = {.m_freq1 = 270, .m_freq2 = 220, .m_attack = 8, .m_decay = 4, .m_sustain = 14, .m_release = 4, .m_volume = 35};

    PlaySoundEffect(&bulletshoot, false, game);
}
void Sound_PlayFuelUp(struct Game *game)
{
    struct SoundEffect fuelup = {.m_freq1 = 830, .m_freq2 = 860, .m_attack = 8, .m_decay = 0, .m_sustain = 10, .m_release = 0, .m_volume = 37, .m_channel = 1, .m_mode = 2};

    PlaySoundEffect(&fuelup, false, game);
}
void Sound_PlayFuelAlarm(struct Game *game)
{
    struct SoundEffect fuelalarm = {.m_freq1 = 220, .m_freq2 = 300, .m_attack = 8, .m_decay = 16, .m_sustain = 14, .m_release = 0, .m_volume = 40, .m_channel = 1, .m_mode = 1};

    PlaySoundEffect(&fuelalarm, false, game);
}