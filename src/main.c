#include "wasm4.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 160

#define TILESIZEY 20
#define TILESIZEX 16

const char WORLDMAP[32][10] = {

    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
    "***....***",
    "****..****",
    "***....***",
    "*........*",
    ".....*....",
    "....***...",
    "....***...",
    ".....*....",
    "*........*",
    "*........*",
    "**......**",
    "**..**..**",
    "**...*****",
    "**....****",
    "**.....***",
    "**.....***",
    "**......**",
    "**......**",
    "**......**",
    "**......**",
};

const char SHIPFORWARD[8] = {
    0b11100111,
    0b11000011,
    0b10000001,
    0b00100100,
    0b01100110,
    0b11100111,
    0b11000011,
    0b10011001,
};
const char SHIPRIGHT[8] = {
    0b11001111,
    0b10000111,
    0b00000011,
    0b00001101,
    0b01001111,
    0b10001111,
    0b10000011,
    0b01011011,
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
    int posX;
    int posY;
    int width;
    int height;

} Ship;

static Ship myship;

typedef struct
{
    int x;
    int y;
    int offsetX;
    int offsetY;
    int width;
    int height;

} Camera;

static Camera mycamera;

void start()
{

    PALETTE[0] = 0x2a3abd;
    PALETTE[1] = 0xcdc23c;
    PALETTE[2] = 0x335715;
    PALETTE[3] = 0x7c3f58;

    myship.dir = None;
    myship.width = 8;
    myship.height = 8;
    myship.posX = SCREEN_WIDTH / 2 - myship.width / 2;
    myship.posY = SCREEN_HEIGHT - 20;
    myship.sprite = &SHIPFORWARD[0];

    mycamera.width = SCREEN_WIDTH;
    mycamera.height = SCREEN_HEIGHT;
}

void moveShipLeft()
{
    if (myship.posX - 1 > 0)
    {
        myship.posX--;
        myship.sprite = &SHIPRIGHT[0];
        myship.dir = Left;
    }
}

void moveShipRight()
{
    if (myship.posX + 1 < SCREEN_WIDTH - myship.width)
    {
        myship.posX++;
        myship.sprite = &SHIPRIGHT[0];
        myship.dir = Right;
    }
}

void moveShipUp()
{
    if (myship.posY - 1 > 120)
    {
        myship.posY--;
    }
}

void moveShipDown()
{
    if (myship.posY + 1 < SCREEN_WIDTH - myship.height)
    {
        myship.posY++;
    }
}

void draw_land()
{
    float startRowF = mycamera.y / TILESIZEY;
    int startRow = (int)startRowF;
    // if (startRow > 16)
    // {
    //     startRow = startRow - 16;
    // }
    int endRow = startRow + (mycamera.height / TILESIZEY);

    mycamera.offsetY = -mycamera.y + startRow * TILESIZEY;

    for (int column = 0; column < SCREEN_WIDTH / TILESIZEX; column++)
    {
        for (int row = startRow; row <= endRow; row++)
        {
            //adjustedrow = row;
            switch (WORLDMAP[row][column])
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

void update()
{

    mycamera.y--;
    if (mycamera.y < 0)
    {
        mycamera.y = 480;
    }
    draw_land();
    myship.dir = None;
    myship.sprite = &SHIPFORWARD[0];

    char gamepad = *GAMEPAD1;
    if (gamepad & BUTTON_1)
    {
        *DRAW_COLORS = 4;
    }
    if (gamepad & BUTTON_LEFT)
    {
        moveShipLeft();
    }
    if (gamepad & BUTTON_RIGHT)
    {
        moveShipRight();
    }
    if (gamepad & BUTTON_UP)
    {
        moveShipUp();
    }
    if (gamepad & BUTTON_DOWN)
    {
        moveShipDown();
    }
    *DRAW_COLORS = 2;
    blit(myship.sprite, myship.posX, myship.posY, myship.width, myship.height, BLIT_1BPP | (myship.dir == Left ? BLIT_FLIP_X : 0));
}
