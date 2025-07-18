#include "iGraphics.h"
#include "iSound.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define S_W 1000
#define S_H 560
#define GRAVITY 3.0f
#define FLAP_STRENGTH 9.0f
#define pipewidth 50
#define pipespace 250
#define pipegap 140
#define pipenumber 4


Image frames[14];
Sprite bird;
int channel;
float birdVelocity = 0;

int pipex[pipenumber]={190,440,690,940};
int pipey[pipenumber];
int pipemove=0;


enum GameState
{
    MENU,
    PLAYING,
    HELP,
    DIFFICULTY,
    SETTINGS,
    EXIT
};
enum GameDifficulty{
    EASY,
    MEDIUM,
    HARD
};
enum GameState currentState = MENU;
enum GameDifficulty diffculty = EASY;
/*
function iDraw() is called again and again by the system.
*/

void iDraw()
{
    // place your drawing codes here
    iClear();

    if (currentState == MENU)
    {
        iShowImage(0, 0, "assets/images/MenuImage.png");

    }
    else if (currentState == PLAYING)
    {
        iShowImage(0, 0, "assets/images/BackGround.bmp");

        iSetColor(21,235,78);
        for(int i=0;i<pipenumber;i++)
        {
            iFilledRectangle(pipex[i],0,pipewidth,pipey[i]);
            iFilledRectangle(pipex[i],pipey[i]+pipegap,pipewidth,S_H-(pipey[i]+pipegap));
        }

        iShowSprite(&bird);

        iSetColor(0, 0, 0);
        iText(S_W / 2 - 100, 50, "Press \"SPACE or LEFT key of mouse\" to flap");
    }
    else if(currentState == HELP)
        iShowImage(0,0,"assets/images/Instructions.png");


}

void InitPipe(){
        for(int i=0;i<pipenumber;i++)
    {
        pipex[i]=S_W+i*pipespace;
        pipey[i]=rand()%201+100;
    }

}

void movepipes()
{
    for(int i=0;i<pipenumber;i++)
    {
        pipex[i]-=2;
        if(pipex[i]+pipewidth<0)
        {
            int lastpipe=pipex[0];
            for(int j=1;j<pipenumber;j++)
            {
                if(pipex[j]>lastpipe)
                {
                    lastpipe=pipex[j];
                }
            }
            pipex[i]=lastpipe+pipespace;
            pipey[i]=rand()%201+100;
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
     printf("x = %d, y = %d\n", mx, my);
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
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (currentState == MENU)
        {
            if (mx >= 434 && mx <= 570 && my >= 281 && my <= 313)
            {
                InitPipe();
                currentState = PLAYING;
                iSetSpritePosition(&bird, S_W / 4, S_H / 2);
                birdVelocity = 0;
            }
            else if(mx >= 448 && mx <= 552 && my >= 213 && my <= 246)
            currentState = HELP;
            else if(mx >= 457 && mx <= 552 && my >= 77 && my <= 110)
                 exit(0);
        }
        else if (currentState == PLAYING)
        {
            birdVelocity = -FLAP_STRENGTH; // Negative velocity to flap upward
        }








    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
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
        else if (currentState == MENU)
        {
            InitPipe();
            currentState = PLAYING;
            iSetSpritePosition(&bird, S_W / 4, S_H / 2);
            birdVelocity = 0;
        }
    }
    if (key == 'p' || key == 'P')

        iPauseSound(channel);

    else if (key == 'r' || key == 'R')

        iResumeSound(channel);

    else if (key == 's' || key == 'S')

        iStopSound(channel);
    else if( key == 'b' || key =='B')
        currentState = MENU;
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
void resources()
{
    iInitSprite(&bird, -1);
    iLoadFramesFromFolder(frames, "assets/images/sprites/FB");
    iChangeSpriteFrames(&bird, frames, 14);
    iSetSpritePosition(&bird, 50, 100);
    iScaleSprite(&bird,1.5);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    glutInit(&argc, argv);
    iInitializeSound();
    resources();
    InitPipe();
    iSetTimer(5, updateGame);
    iSetTimer(16,movepipes);
    channel = iPlaySound("assets/sounds/NGGYU.MP3", true, 50);
    iInitialize(S_W, S_H, "Flappy Bird");

    return 0;
}
