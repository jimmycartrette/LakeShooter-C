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
    line(70 + .30 * game->m_fuellevel / 10, PLAY_HEIGHT + 16, 70 + .3 * game->m_fuellevel / 10, PLAY_HEIGHT + 22);
    *DRAW_COLORS = 2;
    text("E", 62, PLAY_HEIGHT + 16);
    text("F", 101, PLAY_HEIGHT + 16);
}

void Game_Init(struct Game *game)
{

    game->m_state = GAMESTATE_STARTUP;
    game->m_fuellevel = 1000;
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
    if (game->m_state == GAMESTATE_STARTUP)
    {
        game->m_state = GAMESTATE_PLAY;
    }
    switch (game->m_state)
    {
    case GAMESTATE_PLAY:
        if (game->m_tickssincecollision > 0)
        {
            if (game->m_tickssincecollision++ > 100)
            {
                game->m_state = GAMESTATE_GAMEOVER;
            }
        }
        else
        {
            if (game->m_ship.fuelingtickscountdown == 0)
            {
                Sound_PlayBackgroundNoise(game);
            }
            else
            {
                game->m_ship.fuelingtickscountdown--;
            }
            PlayArea_Update(&game->m_playarea, &game->m_ship, game->m_ticks, ticks);
            Ship_Update(&game->m_ship, &game->m_input);

            if (game->m_ship.m_obj.m_tickssincecollision > 0)
            {
                game->m_tickssincecollision++;
            }
            game->m_fuellevel -= (game->m_ship.m_obj.m_vaccel * .03) / 10;
        }
        break;
    }
}
void Game_UpdateObjects(struct Game *game)
{

    switch (game->m_state)
    {
    case GAMESTATE_PLAY:
        if (game->m_tickssincecollision == 0)
        {

            if (game->m_ticks % 100 == 0)
            {
                Fuels_Create(&game->m_fuels, 30, 40);
            }
            if (Input_GamepadButtonPress(&game->m_input, 1))
            {
                Bullets_GenerateBullet(&game->m_bullets, &game->m_ship, game);
            }
            Ship_CollisionDetect(&game->m_ship);
            Bullets_Update(&game->m_bullets, &game->m_playarea, &game->m_ship);
            Fuels_Update(&game->m_fuels, &game->m_playarea);
            Fuels_CollisionDetect(&game->m_fuels, &game->m_ship, game);
        }
        break;
    }
}
void Game_DrawBackground(struct Game *game)
{
    switch (game->m_state)
    {
    case GAMESTATE_PLAY:
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