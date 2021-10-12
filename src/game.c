#include "game.h"
#include "global.h"
#include "wasm4.h"
#include "utils.h"
#include "ship.h"
#include "gameobject.h"
#include "sound.h"
#include "bullets.h"
#include "fuel.h"

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
    if (DEBUG == 1)
    {
        char buffer[3];
        itoa(buffer, game->m_playarea.m_currenttopblock);
        // text(buffer, 20, 140);
    }
}

void Game_Init(struct Game *game)
{

    game->m_state = GAMESTATE_STARTUP;
    game->m_fuellevel = 10000;
    game->m_score = 0;
    game->m_ticks = 0;

    Ship_Initialize(&game->m_ship);
    PlayArea_Initialize(&game->m_playarea);
    Bullets_Init(&game->m_bullets);
    Fuels_Initialize(&game->m_fuels);
}
void Game_UpdateBackground(struct Game *game, const int ticks)
{
    game->m_ticks += ticks;
    Input_Update(&game->m_input);
    if (game->m_fuellevel <= 0)
    {
        game->m_state = GAMESTATE_GAMEOVER;
    }
    if (game->m_state == GAMESTATE_GAMEOVER && Input_GamepadButtonPress(&game->m_input, 2))
    {
        Game_NewGame(game);
    }
    switch (game->m_state)
    {
    case GAMESTATE_STARTUP:
        game->m_state = GAMESTATE_PLAY;
        break;
    case GAMESTATE_SHIPCOLLISION:
        if (game->m_tickssincecollision++ > 100)
        {
            game->m_state = GAMESTATE_GAMEOVER;
        }

        break;
    case GAMESTATE_PLAY:
        if (game->m_tickssincecollision == 1)
        {
            game->m_state = GAMESTATE_SHIPCOLLISION;
            return;
        }
        if (game->m_ship.fuelingtickscountdown == 0)
        {
            Sound_PlayBackgroundNoise(game);
        }
        else
        {
            game->m_ship.fuelingtickscountdown--;
        }
        // tracef("y is %d", game->m_playarea.m_y);
        if (PlayArea_Update(&game->m_playarea, &game->m_ship, game->m_ticks, ticks) && lsfr.m_lfsrvalue % 5 == 0)
        {
            Fuels_Create(&game->m_fuels, &game->m_playarea);
        }
        Ship_Update(&game->m_ship, &game->m_input);

        if (game->m_ship.m_obj.m_tickssincecollision > 0)
        {
            game->m_tickssincecollision++;
        }
        // if (DEBUG == 0)
        // {
        game->m_fuellevel -= (game->m_ship.m_obj.m_vaccel * 4);
        // }
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
    case GAMESTATE_SHIPCOLLISION:

        break;
    case GAMESTATE_PLAY:

        // if (game->m_ticks % 100 == 0)
        // {

        //     Fuels_Create(&game->m_fuels, &game->m_playarea);
        // }
        if (Input_GamepadButtonPress(&game->m_input, 1))
        {
            Bullets_GenerateBullet(&game->m_bullets, &game->m_ship, game);
        }
        Ship_CollisionDetect(&game->m_ship);
        Bullets_CollisionDetect(&game->m_bullets, game);
        Bullets_Update(&game->m_bullets, &game->m_playarea, &game->m_ship);
        Fuels_CollisionDetect(&game->m_fuels, &game->m_ship, game);
        Fuels_Update(&game->m_fuels, &game->m_playarea);
        // fuels colliding with ship

        break;
    }
}
void Game_DrawBackground(struct Game *game)
{
    switch (game->m_state)
    {
    case GAMESTATE_PLAY:
    case GAMESTATE_SHIPCOLLISION:
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
    case GAMESTATE_SHIPCOLLISION:
    case GAMESTATE_PLAY:
        Ship_Draw(&game->m_ship, game);
        Bullets_Draw(&game->m_bullets);
        Fuels_Draw(&game->m_fuels);
        Draw_Status(game);
        break;
    }
}
void Game_NewGame(struct Game *game)
{
    Game_Init(game);
}