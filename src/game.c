#include "game.h"
#include "global.h"
#include "wasm4.h"
#include "utils.h"
#include "jet.h"
#include "gameobject.h"
#include "sound.h"
#include "bullets.h"
#include "fuel.h"
#include "global.h"

void Draw_Status(struct Game *game)
{
    *DRAW_COLORS = 4;
    rect(0, PLAY_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - PLAY_HEIGHT);
    char scoretext[10];
    itoa(scoretext, game->m_score);
    *DRAW_COLORS = 2;
    int scoreoffset = digitsofbase10(game->m_score, 0);
    text(scoretext, 100 - scoreoffset * 8, PLAY_HEIGHT + 4);
    *DRAW_COLORS = 0x10;
    rect(60, PLAY_HEIGHT + 14, 50, 11);
    *DRAW_COLORS = 1;
    line(70 + .30 * game->m_fuellevel / 100, PLAY_HEIGHT + 16, 70 + .3 * game->m_fuellevel / 100, PLAY_HEIGHT + 22);
    *DRAW_COLORS = 2;
    text("E", 62, PLAY_HEIGHT + 16);
    text("F", 101, PLAY_HEIGHT + 16);
    char leveltext[3];
    itoa(leveltext, game->m_level);
    int leveloffset = digitsofbase10(game->m_level, 0);

    text(leveltext, 20 - leveloffset * 8, PLAY_HEIGHT + 26);
    if (DEBUG == 1)
    {
        char buffer[3];
        itoa(buffer, game->m_playarea.m_currenttopblock);
        // text(buffer, 20, 140);
    }
}

void Game_Init(struct Game *game)
{
    if (game->m_state == GAMESTATE_INIT)
    {
        game->m_fuellevel = 10000;
        game->m_ticks = 0;
        game->m_tickssincecollision = 0;
        // tracef("init running setting lsfr to %d", game->m_savedlsfr);
        lsfr.m_lfsrvalue = game->m_savedlsfr;
        Jet_Initialize(&game->m_jet);
        PlayArea_Initialize(&game->m_playarea);
        Bullets_Init(&game->m_bullets);
        Fuels_Initialize(&game->m_fuels);
        Enemys_Initialize(&game->m_enemys);
        Enemys_Create(&game->m_enemys, &game->m_playarea, OBJECT_HELI);
        game->m_state = GAMESTATE_BEGINLEVEL;
    }
}
void Game_UpdateBackground(struct Game *game, const int ticks)
{
    game->m_ticks += ticks;
    Input_Update(&game->m_input);
    if (game->m_fuellevel <= 0)
    {
        game->m_state = GAMESTATE_FUELGONE;
    }
    if (game->m_state == GAMESTATE_GAMEOVER && Input_GamepadButtonPress(&game->m_input, 2))
    {
        Game_NewGame(game);
    }
    switch (game->m_state)
    {
    case GAMESTATE_INIT:
        break;
    case GAMESTATE_BEGINLEVEL:
        if (Input_GamepadButtonPress(&game->m_input, 0) || Input_GamepadButtonPress(&game->m_input, 1))
        {
            for (uint8_t i = 0; i < 7; i++)
            {
                if (game->m_playarea.m_playblocks[i].m_hasbridge)
                {
                    game->m_playarea.m_playblocks[game->m_playarea.m_bridgeblockindex].m_hasbridge = false;
                    // tracef("bbi is %d", game->m_playarea.m_bridgeblockindex);

                    Generate_PlayBlock(game->m_playarea.m_currentblockindex, false, false, true, lsfr.m_lfsrvalue, &game->m_playarea.m_playblocks[(i + 1) % 7], &game->m_playarea.m_playblocks[i], game->m_playarea.m_bridgeblockindex);
                }
            }

            game->m_state = GAMESTATE_PLAY;
        }
        break;
    case GAMESTATE_FUELGONE:
        game->m_state = GAMESTATE_ENDLIFE;
        break;
    case GAMESTATE_ENDLIFE:

        if (game->m_lives_left-- == 0)
        {
            game->m_state = GAMESTATE_GAMEOVER;
        }
        else
        {
            game->m_state = GAMESTATE_INIT;
        }
        break;
    case GAMESTATE_GAMEOVER:
        if (Input_GamepadButtonPress(&game->m_input, 0) || Input_GamepadButtonPress(&game->m_input, 1))
        {
            game->m_state = GAMESTATE_INIT;
        }
        break;

    case GAMESTATE_JETCOLLISION:
        if (game->m_tickssincecollision++ > 100)
        {
            game->m_state = GAMESTATE_ENDLIFE;
        }

        break;
    case GAMESTATE_PLAY:
        if (game->m_tickssincecollision == 1)
        {
            game->m_state = GAMESTATE_JETCOLLISION;
            return;
        }
        if (game->m_jet.fuelingtickscountdown == 0)
        {
            Sound_PlayBackgroundNoise(game);
        }
        else
        {
            game->m_jet.fuelingtickscountdown--;
        }
        // tracef("y is %d", game->m_playarea.m_y);

        if (LEVELDEBUGMODE && WANTEDLEVEL > game->m_level)
        {

            game->m_jet.m_obj.m_vaccel = 64;
        }
        if (LEVELDEBUGMODE && WANTEDLEVEL == game->m_level)
        {
            game->m_jet.m_obj.m_vaccel = 1;
        }
        bool isNewBlock = PlayArea_Update(&game->m_playarea, &game->m_jet, game->m_ticks);
        if (isNewBlock && (lsfr.m_lfsrvalue >> 1) % 5 == 0)
        {
            Fuels_Create(&game->m_fuels, &game->m_playarea);
        }
        else if (isNewBlock && (lsfr.m_lfsrvalue >> 2) % 3 == 0)
        {
            Enemys_Create(&game->m_enemys, &game->m_playarea, OBJECT_SHIP);
        }
        else if (isNewBlock && (lsfr.m_lfsrvalue >> 4) % 2 == 0)
        {
            Enemys_Create(&game->m_enemys, &game->m_playarea, OBJECT_HELI);
        }
        // if (isNewBlock && (lsfr.m_lfsrvalue >> 4) % 4 == 0)
        // {
        //     Clouds_GenerateCloud(game);
        // }
        Jet_Update(&game->m_jet, &game->m_input, game);

        if (game->m_jet.m_obj.m_tickssincecollision > 0)
        {
            game->m_tickssincecollision++;
        }
        if (!LEVELDEBUGMODE)
        {
            game->m_fuellevel -= (game->m_jet.m_obj.m_vaccel * 4);
        }
        if (game->m_fuellevel < 1800 && game->m_ticks % 33 == 0)
        {
            Sound_PlayFuelAlarm(game);
        }

        break;
    }
}
void Game_UpdateObjects(struct Game *game)
{

    switch (game->m_state)
    {
    case GAMESTATE_INIT:
        break;
    case GAMESTATE_BEGINLEVEL:
        break;
    case GAMESTATE_FUELGONE:
        break;
    case GAMESTATE_ENDLIFE:
        break;

    case GAMESTATE_GAMEOVER:
        break;
    case GAMESTATE_JETCOLLISION:

        break;
    case GAMESTATE_PLAY:

        // if (game->m_ticks % 100 == 0)
        // {

        //     Fuels_Create(&game->m_fuels, &game->m_playarea);
        // }
        if (Input_GamepadButtonPress(&game->m_input, 1))
        {
            Bullets_GenerateBullet(&game->m_bullets, &game->m_jet, game);
        }
        if (!LEVELDEBUGMODE)
        {
            Jet_Land_CollisionDetect(&game->m_jet);
            Jet_Enemys_CollisionDetect(&game->m_jet, &game->m_enemys, game);
        }
        Bullets_Anything_CollisionDetect(&game->m_bullets, game);
        Bullets_Update(&game->m_bullets, &game->m_playarea, &game->m_jet, game);
        Fuels_Jet_CollisionDetect(&game->m_fuels, &game->m_jet, game);

        Fuels_Update(&game->m_fuels, &game->m_playarea, game);
        Enemys_Update(&game->m_enemys, &game->m_playarea);
        Clouds_Update(&game->m_clouds, game);
        // fuels colliding with jet

        break;
    }
}
void Game_DrawBackground(struct Game *game)
{
    switch (game->m_state)
    {
    case GAMESTATE_INIT:
    case GAMESTATE_BEGINLEVEL:
    case GAMESTATE_FUELGONE:
    case GAMESTATE_ENDLIFE:
    case GAMESTATE_PLAY:
    case GAMESTATE_JETCOLLISION:
        PlayArea_NewDraw(&game->m_playarea);
        break;
    case GAMESTATE_GAMEOVER:
        *DRAW_COLORS = 1;
        rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        *DRAW_COLORS = 2;
        text("GAME OVER", SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 20);

        break;
    }
}
void Game_DrawObjects(struct Game *game)
{
    switch (game->m_state)
    {
    case GAMESTATE_INIT:
        break;

    case GAMESTATE_FUELGONE:
        break;
    case GAMESTATE_ENDLIFE:
        break;
    case GAMESTATE_GAMEOVER:
        *DRAW_COLORS = 2;
        text("press X to restart", 10, 50);
        break;
    case GAMESTATE_BEGINLEVEL:
        *DRAW_COLORS = 2;
        text("press X to start", 16, 50);
    case GAMESTATE_JETCOLLISION:
    case GAMESTATE_PLAY:

        Fuels_Draw(&game->m_fuels);
        Enemys_Draw(&game->m_enemys, counter < 2 ? 0 : 1);
        if (++counter == 4)
        {
            counter = 0;
        }
        Jet_Draw(&game->m_jet, game);
        Bullets_Draw(&game->m_bullets);
        Draw_Status(game);
        Clouds_Draw(&game->m_clouds);
        break;
    }
}
void Game_NewGame(struct Game *game)
{
    Game_Init(game);
}