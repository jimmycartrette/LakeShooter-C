#include "wasm4.h"
#include "utils.h"
#include "game.h"
#include "global.h"
#include <stdbool.h>

const char EXPLOSION[8] = {
    0b11111101,
    0b01111111,
    0b11101111,
    0b10111111,
    0b11111101,
    0b11011111,
    0b10111101,
    0b11101111,
};
const char BULLET[3] = {
    0b11101111,
    0b11101111,
    0b11101111,
};

typedef enum
{
    Left,
    Right,
    None
} LRDirection;
typedef struct
{
    char const *sprite;
    LRDirection dir;
    float posX, posY;
    int width;
    int height;
    float haccel, vaccel;
    float prePosX, prePosY;
    int animCount, animDelay;

} Ship;

static Ship myship;

typedef struct
{
    int x, y;
} Sprite;

typedef struct
{
    int x;
    int y;
    int offsetX;
    int offsetY;
    int width;
    int height;

} Camera;

typedef struct
{
    int x;
    int y;
    float velocity;
    int alive;
} Bullet;
Bullet bulletsra[MAX_BULLETS];
Bullet *bullets = bulletsra;

static Camera mycamera;
unsigned char previousGamepad;
static char *debugtext;

void start()
{

    PALETTE[0] = 0x2a3abd;
    PALETTE[1] = 0xcdc23c;
    PALETTE[2] = 0x6c9850;
    PALETTE[3] = 0x909090;

    Game_Init(&game);

    // for (int n = 0; n < MAX_BULLETS; n++)
    // {
    //     bullets[n].x = 0;
    //     bullets[n].y = 0;
    // }
}

void updatebullet(Bullet *spr)
{
    spr->x++;

    spr->y = spr->y + myship.posY - myship.prePosY;
}

void updatebullets()
{
    int n;
    //update/draw bullets
    for (n = 0; n < MAX_BULLETS; n++)
        if (bullets[n].alive)
        {
            if (bullets[n].y < mycamera.offsetY)
            {
                bullets[n].alive = false;
            }
            else
            {
                //updatebullet(bullets + n);
                bullets[n].y = bullets[n].y - 4;
                bullets[n].x = bullets[n].x + (myship.posX - myship.prePosX);

                //draw_sprite(BULLET[0], bullet_images[0], bullets[n]->x, bullets[n]->y);

                *DRAW_COLORS = 2;
                blit(BULLET, bullets[n].x, bullets[n].y, 8, 3, BLIT_1BPP);
            }
        }
}

// void moveShipLeft()
// {
//     myship.sprite = &SHIPRIGHT[0];
//     myship.dir = Left;
//     if (myship.posX - 1 > 0)
//     {
//         myship.posX = myship.posX - myship.haccel;
//     }
// }

// void moveShipRight()
// {

//     myship.sprite = &SHIPRIGHT[0];
//     myship.dir = Right;
//     if (myship.posX + 1 < PLAY_WIDTH - myship.width)
//     {
//         myship.posX = myship.posX + myship.haccel;
//     }
// }

void draw_land()
{
    float startRowF = (mycamera.y / 4) / TILESIZEY;
    int startRow = (int)startRowF;
    int endRow = startRow + (mycamera.height / TILESIZEY);
    mycamera.offsetY = -mycamera.y / 4 + startRow * TILESIZEY;
    for (int column = 0; column < PLAY_WIDTH / TILESIZEX; column++)
    {
        for (int row = startRow; row <= endRow; row++)
        {
            //adjustedrow = row;

            char result[3] = "";

            switch (*(game.m_worldmap + row * 10 + column))
            {
            case '.':
                *DRAW_COLORS = 1;

                break;
            case '*':
                *DRAW_COLORS = 3;

                break;
            default:
                break;
            }
            int y = (row - startRow) * TILESIZEY + mycamera.offsetY;
            rect(column * TILESIZEX, y, TILESIZEX, TILESIZEY);
        }
    }
}

void draw_status()
{
    *DRAW_COLORS = 4;
    rect(0, PLAY_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - PLAY_HEIGHT);
    char scoretext[10];
    itoa(scoretext, game.m_score);
    *DRAW_COLORS = 2;
    int scoreoffset = digitsofbase10(game.m_score, 0);
    text(scoretext, 100 - scoreoffset * 8, PLAY_HEIGHT + 4);
    *DRAW_COLORS = 0x10;
    rect(60, PLAY_HEIGHT + 14, 50, 11);
    *DRAW_COLORS = 1;
    line(70 + .30 * game.m_fuellevel / 10, PLAY_HEIGHT + 16, 70 + .3 * game.m_fuellevel / 10, PLAY_HEIGHT + 22);
    *DRAW_COLORS = 2;
    text("E", 62, PLAY_HEIGHT + 16);
    text("F", 101, PLAY_HEIGHT + 16);
}

bool detect_spritecollision(float posX, float posY, int width, int height, const char *sprite)
{

    for (int column = posX; column < posX + width; column++)
    {
        for (int row = posY; row < posY + height; row++)
        {
            // TODO add pixel check on myship.sprite
            if (sprite[row - (int)posY] & 1 << (7 - column - (int)posX))
            {
                unsigned char *checkframe = FRAMEBUFFER + row * 40 + column / 4;

                char buffer[256];
                int val = (*checkframe) >> (2 ^ (2 - (column % 4)));

                if ((val & 3) == 2)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
void fireatenemy()
{

    game.m_score += 10;
    int n;
    for (n = 0; n < MAX_BULLETS; n++)
    {
        if (!bullets[n].alive)
        {
            bullets[n].alive++;
            bullets[n].x = myship.posX;
            bullets[n].y = myship.posY;
            return;
        }
    }
}

void update()
{
    Game_Update(&game, 1);
    Game_Draw(&game);

    // if (game.m_state == GAMESTATE_GAMEOVER)
    // {
    //     char gamepad = *GAMEPAD1;
    //     if (gamepad & BUTTON_2)
    //     {
    //         start();
    //     }
    //     else
    //     {
    //         *DRAW_COLORS = 1;
    //         rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //         *DRAW_COLORS = 2;
    //         text("GAME OVER", SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 20);
    //     }
    // }
    // else
    // {

    //     draw_land();
    //     draw_status();
    //     myship.dir = None;
    //     myship.sprite = &SHIPFORWARD[0];

    //     char gamepad = *GAMEPAD1;
    //     unsigned char pressedThisFrame = gamepad & (gamepad ^ previousGamepad);
    //     previousGamepad = gamepad;
    //     myship.prePosY = myship.posY;
    //     myship.prePosX = myship.posX;

    //     if (game.m_tickssincecollision > 0)
    //     {
    //         if (game.m_tickssincecollision++ > 100)
    //         {
    //             game.m_state = GAMESTATE_GAMEOVER;
    //         }
    //         else
    //         {
    //             blit(EXPLOSION, myship.posX, myship.posY, myship.width, myship.height, BLIT_1BPP);
    //         }
    //     }
    //     else
    //     {
    //         mycamera.y = mycamera.y - myship.vaccel * 2.2;
    //         if (mycamera.y < 0)
    //         {
    //             mycamera.y = 1920;
    //         }
    //         if (gamepad & BUTTON_1)
    //         {
    //         }
    //         if (!(gamepad & BUTTON_LEFT || gamepad & BUTTON_RIGHT))
    //         {
    //             myship.haccel = 0;
    //         }
    //         else
    //         {
    //             if (myship.haccel < 1)
    //             {
    //                 myship.haccel += .04;
    //             }
    //             else if (myship.haccel < .3)
    //             {
    //                 myship.haccel += .03;
    //             }
    //         }
    //         if (gamepad & BUTTON_LEFT)
    //         {

    //             moveShipLeft();
    //         }
    //         if (gamepad & BUTTON_RIGHT)
    //         {
    //             moveShipRight();
    //         }
    //         if (!(gamepad & BUTTON_UP || gamepad & BUTTON_DOWN))
    //         {
    //             if (myship.vaccel > 1)
    //             {
    //                 myship.vaccel -= .04;
    //             }
    //             else
    //             {
    //                 myship.vaccel += .04;
    //             }
    //         }
    //         if (gamepad & BUTTON_UP)
    //         {
    //             if (myship.vaccel < 2)
    //             {
    //                 myship.vaccel += .04;
    //             }
    //             else if (myship.vaccel < 1.3)
    //             {
    //                 myship.vaccel += .03;
    //             }
    //         }
    //         else if (gamepad & BUTTON_DOWN)
    //         {
    //             if (myship.vaccel > .3)
    //             {
    //                 myship.vaccel -= .04;
    //             }
    //             else if (myship.vaccel > .7)
    //             {
    //                 myship.vaccel -= .03;
    //             }
    //         }
    //         if (pressedThisFrame & BUTTON_1)
    //         {
    //             fireatenemy();
    //         }
    //         bool shipcollided = detect_spritecollision(myship.posX, myship.posY, myship.width, myship.height, myship.sprite);
    //         if (shipcollided)
    //         {
    //             tone(250 | (0 << 16), (8 << 24) | (10 << 16) | 8 | (8 << 8), 34, 3 | (0 << 0));
    //             game.m_tickssincecollision++;
    //         }
    //         game.m_fuellevel -= (myship.vaccel * .03) / 10;
    //         if (game.m_fuellevel <= 0)
    //         {

    //             game.m_state = GAMESTATE_GAMEOVER;
    //         }

    //         *DRAW_COLORS = 2;
    //         blit(myship.sprite, myship.posX, myship.posY, myship.width, myship.height, BLIT_1BPP | (myship.dir == Left ? BLIT_FLIP_X : 0));
    //         updatebullets();

    //         if (game.m_ticks % 20 == 0)
    //         {
    //             tone(320 + (int)(myship.vaccel * (float)40) | (0 << 16), (0 << 24) | (0 << 16) | 20 | (0 << 8), 34, 3 | (0 << 2));
    //         }
    //     }
    // }
}
