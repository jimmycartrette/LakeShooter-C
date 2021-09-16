#include "game.h"
#include "global.h"
#include "wasm4.h"
#include "utils.h"

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
    game->m_tickssincecollision = 0;

    Ship_Initialize(&game->m_ship);
    PlayArea_Initialize(&game->m_playarea);
}
void Game_Update(struct Game *game, const int ticks)
{
    game->m_ticks += ticks;
    Input_Update(&game->m_input);
    switch (game->m_state)
    {
    case GAMESTATE_STARTUP:
        game->m_state = GAMESTATE_PLAY;
        break;
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
            PlayArea_Update(&game->m_playarea, &game->m_ship, ticks);
        }
        break;
    case GAMESTATE_GAMEOVER:
        if (Input_GamepadButtonPress(&game->m_input, 2))
        {
            Game_NewGame(game);
        }
        break;
    }
}
void Game_Draw(struct Game *game)
{
    switch (game->m_state)
    {
    case GAMESTATE_PLAY:
        PlayArea_Draw(&game->m_playarea);
        Draw_Status(game);
        break;
    case GAMESTATE_GAMEOVER:
        *DRAW_COLORS = 1;
        rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        *DRAW_COLORS = 2;
        text("GAME OVER", SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 20);

        break;
    }
}
void Game_NewGame(struct Game *game)
{
    game->m_fuellevel = 1;
}