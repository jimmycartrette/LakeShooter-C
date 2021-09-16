#pragma once

#define MAX_BULLETS 20

#include "stdint.h"
#include "ship.h"
#include "playarea.h"
#include "input.h"

enum GameState
{
    GAMESTATE_STARTUP = 0,
    GAMESTATE_PLAY = 1,
    GAMESTATE_GAMEOVER = 2
};

struct Game
{
    int64_t m_ticks;
    int8_t m_state;
    int64_t m_fuellevel;
    int64_t m_score;
    int64_t m_tickssincecollision;
    char *m_worldmap;

    struct Input m_input;

    struct Ship m_ship;
    struct PlayArea m_playarea;
};

void Game_Init(struct Game *game);
void Game_Update(struct Game *game, const int ticks);
void Game_Draw(struct Game *game);
void Game_NewGame(struct Game *game);
