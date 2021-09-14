#include "wasm4.h"
#include "utils.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 160

#define TILESIZEY 20
#define TILESIZEX 16

#define PLAY_WIDTH 160
#define PLAY_HEIGHT 120

#define MAX_BULLETS 20

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
    0b11101111,
    0b11000111,
    0b10000011,
    0b00000001,
    0b00101001,
    0b01101101,
    0b11000111,
    0b00101001,
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
const char BULLET[3] = {
    0b11101111,
    0b11101111,
    0b11101111,
};

typedef enum
{
    false,
    true
} bool;

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

typedef struct
{
    bool over;
    int score;
} Game;
static Game game;

static Camera mycamera;
unsigned char previousGamepad;
static char *debugtext;

void start()
{

    PALETTE[0] = 0x2a3abd;
    PALETTE[1] = 0xcdc23c;
    PALETTE[2] = 0x335715;
    PALETTE[3] = 0x909090;

    myship.dir = None;
    myship.width = 8;
    myship.height = 8;
    myship.posX = PLAY_WIDTH / 2 - myship.width / 2;
    myship.posY = PLAY_HEIGHT - 10;
    myship.prePosX = myship.posX;
    myship.prePosY = myship.posY;
    myship.sprite = &SHIPFORWARD[0];
    myship.vaccel = 1;
    myship.haccel = 0;
    myship.animDelay = 4;
    mycamera.width = PLAY_WIDTH;
    mycamera.height = PLAY_HEIGHT;

    for (int n = 0; n < MAX_BULLETS; n++)
    {
        //bullets[n] = malloc(sizeof(Sprite));

        bullets[n].x = 0;
        bullets[n].y = 0;
    }
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

void moveShipLeft()
{
    myship.sprite = &SHIPRIGHT[0];
    myship.dir = Left;
    if (myship.posX - 1 > 0)
    {
        myship.posX = myship.posX - myship.haccel;
    }
}

void moveShipRight()
{

    myship.sprite = &SHIPRIGHT[0];
    myship.dir = Right;
    if (myship.posX + 1 < PLAY_WIDTH - myship.width)
    {
        myship.posX = myship.posX + myship.haccel;
    }
}

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
int digitsofbase10(int num, int counter)
{
    if (num < 10)
    {
        return counter;
    }
    return digitsofbase10(num / 10, counter + 1);
}
void draw_status()
{
    *DRAW_COLORS = 4;
    rect(0, PLAY_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - PLAY_HEIGHT);
    char scoretext[10];
    itoa(scoretext, game.score);
    *DRAW_COLORS = 2;
    int scoreoffset = digitsofbase10(game.score, 0);
    text(scoretext, 100 - scoreoffset * 8, PLAY_HEIGHT + 4);
}

bool collision_detected()
{
    return false;
    for (int column = myship.posX; column < myship.posX + myship.width; column++)
    {
        for (int row = myship.posY; row < myship.posY + myship.height; row++)
        {
            // TODO add pixel check on myship.sprite
            if (myship.sprite[row - (int)myship.posY] & 1 << (7 - column - (int)myship.posX))
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

    game.score += 10;
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
    if (game.over)
    {
        // *DRAW_COLORS = 1;
        // rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        // *DRAW_COLORS = 2;
        // text("GAME OVER", SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 20);
    }
    else
    {
        mycamera.y = mycamera.y - myship.vaccel * 4;
        if (mycamera.y < 0)
        {
            mycamera.y = 1920;
        }
        draw_land();
        draw_status();
        myship.dir = None;
        myship.sprite = &SHIPFORWARD[0];

        char gamepad = *GAMEPAD1;
        unsigned char pressedThisFrame = gamepad & (gamepad ^ previousGamepad);
        previousGamepad = gamepad;
        myship.prePosY = myship.posY;
        myship.prePosX = myship.posX;
        if (gamepad & BUTTON_1)
        {
        }
        if (!(gamepad & BUTTON_LEFT || gamepad & BUTTON_RIGHT))
        {
            myship.haccel = 0;
        }
        else
        {
            if (myship.haccel < 1)
            {
                myship.haccel += .04;
            }
            else if (myship.haccel < .3)
            {
                myship.haccel += .03;
            }
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
            myship.vaccel = 1.4;
        }
        else if (gamepad & BUTTON_DOWN)
        {
            //moveShipDown();
            myship.vaccel = .5;
        }
        else
        {
            myship.vaccel = 1;
        }
        if (pressedThisFrame & BUTTON_1)
        {
            fireatenemy();
        }
        bool collided = collision_detected();
        if (collided)
        {
            game.over = true;
        }
        *DRAW_COLORS = 2;
        blit(myship.sprite, myship.posX, myship.posY, myship.width, myship.height, BLIT_1BPP | (myship.dir == Left ? BLIT_FLIP_X : 0));
        updatebullets();
    }
}
