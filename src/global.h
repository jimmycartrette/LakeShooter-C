#pragma once
#include "stdint.h"
#include <stdbool.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 160

#define TILESIZEY 20
#define TILESIZEX 16

#define PLAY_WIDTH 160
#define PLAY_HEIGHT 120

extern uint8_t counter;
extern bool LEVELDEBUGMODE;
extern uint8_t WANTEDLEVEL;
extern uint16_t INITIAL_LFSRSEED;

extern struct Game game;
extern struct Lsfr lsfr;