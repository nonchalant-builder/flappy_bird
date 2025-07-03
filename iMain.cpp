#include "iGraphics.h"
#include "iSound.h"
#include <stdio.h>
#include <stdlib.h>

#define S_W 1000
#define S_H 560
#define GRAVITY 1.3f
#define FLAP_STRENGTH 14.3f

Image frames[14];
Sprite bird;
int channel;
float birdVelocity = 0;

#define pipewidth 50
#define pipespace 250
#define pipegap 132
#define pipenumber 4

int pipex[pipenumber] = {190, 440, 690, 940};
int pipey[pipenumber];
int pipemove;
float PIPESPEED;

enum GameState
{
    MENU,
    PLAYING,
    HELP,
    SETTINGS,
    CREDIT,
    SELECT_DIFFICULTY,
    LEADERBOARD,
    EXIT
};
enum GameDifficulty
{
    EASY,
    MEDIUM,
    HARD
};
enum GameState currentState = MENU;
enum GameDifficulty difficulty = EASY;
/*
function iDraw() is called again and again by the system.
*/

void iDraw() {
    iClear();
    if (currentState == MENU) {
        iShowImage(0, 0, "assets/images/MenuImage.png");
    }
    else if (currentState == SELECT_DIFFICULTY) {
        iShowImage(0, 0, "assets/images/DifficultyPage.png");
    }
    else if (currentState == PLAYING) {
        if (difficulty == EASY) {
            PIPESPEED = 2.5;
            iShowImage(0, 0, "assets/images/BackGround.png");
        } else if (difficulty == MEDIUM) {
            PIPESPEED = 4.5;
            iShowImage(0, 0, "assets/images/MediumPage.bmp");
        } else if (difficulty == HARD) {
            PIPESPEED = 6.0;
            iShowImage(0, 0, "assets/images/HardPage.bmp");
        }
        iSetColor(58, 126, 59);
        for (int i = 0; i < pipenumber; i++) {
            iFilledRectangle(pipex[i], 0, pipewidth, pipey[i]);
            iFilledRectangle(pipex[i], pipey[i] + pipegap, pipewidth, S_H - (pipey[i] + pipegap));
        }
        iShowSprite(&bird);
    }
    else if (currentState == HELP) {
        iShowImage(0, 0, "assets/images/Instructions.png");
    }
}

void movepipes()
{
    for (int i = 0; i < pipenumber; i++)
    {
        pipex[i] -= PIPESPEED;
        if (pipex[i] + pipewidth < 0)
        {
            int lastpipe = pipex[0];
            for (int j = 0; j < pipenumber; j++)
            {
                if (pipex[j] > lastpipe)
                {
                    lastpipe = pipex[j];
                }
            }
            pipex[i] = lastpipe + pipespace;
            pipey[i] = rand() % 201 + 100;
        }
    }
}

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
  //  printf("x = %d, y = %d\n", mx, my);
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentState == MENU) {
            if (mx >= 434 && mx <= 564 && my >= 325 && my <= 354) {
                currentState = SELECT_DIFFICULTY;
            }
            else if (mx >= 452 && mx <= 545 && my >= 202 && my <= 229) {
                currentState = HELP;
            }
            else if (mx >= 458 && mx <= 544 && my >= 77 && my <= 102) {
                exit(0);
            }
            else if (mx >= 356 && mx <= 643 && my >= 264 && my <= 294) {
                currentState = LEADERBOARD;
            }
            else if (mx >= 428 && mx <= 574 && my >= 138 && my <= 165) {
                currentState = CREDIT;
            }
        }
        else if (currentState == SELECT_DIFFICULTY) {
            if (mx >= 438 && mx <= 554 && my >= 289 && my <= 317) {
                difficulty = EASY;
                currentState = PLAYING;
            }
            else if (mx >= 408 && mx <= 586 && my >= 222 && my <= 250) {
                difficulty = MEDIUM;
                currentState = PLAYING;
            }
            else if (mx >= 437 && mx <= 556 && my >= 158 && my <= 182) {
                difficulty = HARD;
                currentState = PLAYING;
            }
        }
        else if (currentState == PLAYING) {
            birdVelocity = -FLAP_STRENGTH;
        }
        else if (currentState == HELP) {
            if (mx >= 14 && mx <= 81 && my >= 531 && my <= 553) {
                currentState = MENU;
            }
        }
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if (key == ' ')
    {
        if (currentState == PLAYING)
        {
            birdVelocity = -FLAP_STRENGTH; // Flap upward
        }
    }
    if (key == 'p' || key == 'P')

        iPauseSound(channel);

    else if (key == 'r' || key == 'R')

        iResumeSound(channel);

    if (currentState == PLAYING && (difficulty == EASY || difficulty == MEDIUM || difficulty == HARD))
    {
        if (key == 'b' || key == 'B')
            currentState = MENU;
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        iIncreaseVolume(channel, 5);
        break;
    case GLUT_KEY_DOWN:
        iDecreaseVolume(channel, 5);
        break;
    default:
        break;
    }
}
void updateGame()
{
    if (currentState != PLAYING)
        return;

    birdVelocity += GRAVITY; // Positive gravity pulls bird down
    bird.y -= birdVelocity;  // Subtract velocity to move down when positive
    iSetSpritePosition(&bird, bird.x, bird.y);

    iAnimateSprite(&bird);

    if (bird.y < 0)
    {
        bird.y = 0;
        birdVelocity = 0;
    }
    if (bird.y + bird.frames[bird.currentFrame].height > S_H)
    {
        bird.y = S_H - bird.frames[bird.currentFrame].height;
        birdVelocity = 0;
    }
}
void InitPipe() {
    for (int i = 0; i < pipenumber; i++) {
        pipex[i] = S_W + i * pipespace;
        pipey[i] = rand() % 201 + 100;
    }
}

void resources()
{
    iInitSprite(&bird, -1);
    iLoadFramesFromFolder(frames, "assets/images/sprites/FB");
    iChangeSpriteFrames(&bird, frames, 14);
    iSetSpritePosition(&bird, 50, 100);
    iScaleSprite(&bird, 1.3);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    glutInit(&argc, argv);
    iInitializeSound();
    resources();
    InitPipe();
    iSetTimer(5, updateGame);
    iSetTimer(16, movepipes);
    channel = iPlaySound("assets/sounds/NGGYU.MP3", true, 50);
    iInitialize(S_W, S_H - 10, "Flappy Bird");

    return 0;

    return 0;
}
