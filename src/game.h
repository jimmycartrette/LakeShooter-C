#pragma once

#include "stdint.h"
#include "jet.h"
#include "playarea.h"
#include "input.h"
#include "bullets.h"
#include "fuel.h"

enum GameState
{
    GAMESTATE_STARTUP = 0,
    GAMESTATE_PLAY = 1,
    GAMESTATE_JETCOLLISION = 2,
    GAMESTATE_GAMEOVER = 3
};

struct Game
{
    int64_t m_ticks;
    int8_t m_state;
    int64_t m_fuellevel;
    int64_t m_score;
    int64_t m_tickssincecollision;
    int16_t m_soundeffectcountdown;
    char *m_worldmap;

    struct Input m_input;

    struct Jet m_jet;
    struct PlayArea m_playarea;
    struct Bullets m_bullets;
    struct Fuels m_fuels;
};

void Game_Init(struct Game *game);
void Game_UpdateBackground(struct Game *game, const int ticks);
void Game_UpdateObjects(struct Game *game);
void Game_DrawBackground(struct Game *game);
void Game_DrawObjects(struct Game *game);
void Game_NewGame(struct Game *game);
