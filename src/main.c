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
    Game_Init(&game);
}

void update()
{
    Game_UpdateBackground(&game, 1);

    Game_DrawBackground(&game);
    Game_UpdateObjects(&game);
    Game_DrawObjects(&game);
}
