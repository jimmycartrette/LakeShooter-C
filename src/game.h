#pragma once

#include "stdint.h"
#include "jet.h"
#include "playarea.h"
#include "input.h"
#include "bullets.h"
#include "fuel.h"
#include "cloud.h"
#include "ship.h"

enum GameState
{
    GAMESTATE_INIT = 0,
    GAMESTATE_BEGINLEVEL = 1,
    GAMESTATE_PLAY = 2,
    GAMESTATE_JETCOLLISION = 3,
    GAMESTATE_FUELGONE = 4,
    GAMESTATE_ENDLIFE = 5,
    GAMESTATE_GAMEOVER = 6
};

struct Game
{
    int64_t m_ticks;
    int8_t m_lives_left;
    enum GameState m_state;
    int64_t m_fuellevel;
    uint16_t m_savedlsfr;
    int16_t m_score;
    int64_t m_tickssincecollision;
    int16_t m_soundeffectcountdown;
    char *m_worldmap;

    struct Input m_input;

    struct Jet m_jet;
    struct PlayArea m_playarea;
    struct Bullets m_bullets;
    struct Ships m_ships;
    struct Fuels m_fuels;
    struct Clouds m_clouds;
};

void Game_Init(struct Game *game);
void Game_UpdateBackground(struct Game *game, const int ticks);
void Game_UpdateObjects(struct Game *game);
void Game_DrawBackground(struct Game *game);
void Game_DrawObjects(struct Game *game);
void Game_NewGame(struct Game *game);
