#include "wasm4.h"
#include "utils.h"
#include "game.h"
#include "global.h"
#include <stdbool.h>
#include "sound.h"

unsigned char previousGamepad;
static char *debugtext;

void start()
{

    PALETTE[0] = 0x2a3abd; // 1 = blue
    PALETTE[1] = 0xcdc23c; // 2 = yellow
    PALETTE[2] = 0x6c9850; // 3 = green
    PALETTE[3] = 0x909090; // 4 = grey
    lfsr_start(0xB3C4u, &lsfr);

    game.m_level = 1;
    game.m_lives_left = 5;
    game.m_score = 0;
    game.m_state = GAMESTATE_INIT;
    game.m_savedlsfr = lsfr.m_lfsrvalue;
    //tracef("initial saved lsfr is %d", game.m_savedlsfr);
}

void update()
{
    Game_Init(&game);
    Game_UpdateBackground(&game, 1);
    Game_DrawBackground(&game);
    Game_UpdateObjects(&game);
    Game_DrawObjects(&game);
}
