#include "global.h"
#include "game.h"
#include "utils.h"

uint8_t counter = 0;
bool LEVELDEBUGMODE = false;
uint8_t WANTEDLEVEL = 11;
uint16_t INITIAL_LFSRSEED = 0xB3C4u;

struct Game game;
struct Lsfr lsfr;
