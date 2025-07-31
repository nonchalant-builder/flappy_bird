#include "iGraphics.h"
#include "iSound.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define S_W 1000
#define S_H 560
#define FLAP_STRENGTH 13.8f

Image frames[3];
Sprite bird;
int channel;
int Bgsound;
float birdVelocity = 0;

#define MAX 3
#define pipewidth 50
#define pipespace 350
#define pipenumber 4
#define PIPEGAP 150


bool flag = false;
int nextpipeindex;
int pipex[pipenumber];
int pipey[pipenumber];
int pipemove;
float PIPESPEED;
float GRAVITY;

int score1,score2,score3;
int highscore = 0;
int score = 0;
int scores[MAX] = {0, 0, 0};
int scoreCount = 0;
char SCORE[20];
int scoredPipe[pipenumber]; // Track which pipes have been scored

// game state for menu
enum GameState
{
    MENU,
    PAUSE,
    PLAYING,
    HELP,
    CREDIT,
    SELECT_DIFFICULTY,
    LEADERBOARD,
    GAME_OVER,
    EXIT
};
//enumeration for difficulty selection
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


//Highscore load(i.e file load for highscore)
void loadHighScore()
{
    FILE *fp = fopen("highscore.txt", "r");
    scoreCount = 0;
    if (fp != NULL)
    {
        char line[50];
        while (scoreCount < MAX && fgets(line, sizeof(line), fp) != NULL)
        {
            int readScore;
            if (sscanf(line, "%d", &readScore) == 1)
            {
                scores[scoreCount++] = readScore;
            }
        }
        fclose(fp);
    }
    
    else
    {
        printf("highscore.txt not found, initializing empty score list\n");
    }
    // Sort scores in descending order
    for (int i = 0; i < scoreCount - 1; i++)
    {
        for (int j = 0; j < scoreCount - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
    // Pad with zeros if fewer than MAX_SCORES
    while (scoreCount < MAX)
    {
        scores[scoreCount++] = 0;
    }
    highscore = scores[0];
}

void HS()
{
        // Replace the lowest score
        if(score > scores[1] && score < scores[0]){
              int t = scores[1];
              scores[1] = scores[2];
              scores[2] = t;
              scores[1] = score;
        }
        else if(score > scores[0]){
              //2nd and 3rd score replace
            int n = scores[1];
              scores[1] = scores[2];
              scores[2] = n;

              //highscore and second score replace
              scores[1] = scores[0];
             scores[0] = score;

        }
        // Write scores to file
        FILE *fp = fopen("highscore.txt", "w");
        if (fp != NULL)
        {
            for (int i = 0; i < MAX; i++)
            {
                fprintf(fp, "%d\n", scores[i]);
            }
           
            fclose(fp);
        }
        else
        {
            printf("Failed to open highscore.txt for writing\n");
        }
        highscore = scores[0];
}

void iDraw()
{
    iClear();
    if (currentState == MENU)
    
    {    if(!flag){
        iResumeSound(Bgsound);}
       iShowImage(0, 0, "assets/images/MenuImage.png");
    }
    else if (currentState == SELECT_DIFFICULTY)
    {   
        iShowImage(0, 0, "assets/images/DifficultyPage.png");
    }
   else if (currentState == PLAYING || currentState == PAUSE)
    {
        if (currentState == PLAYING)
        {
            iPauseSound(Bgsound);
        }
        if (difficulty == EASY)
        {
            PIPESPEED = 2.5;
            GRAVITY = 1.3;
            
            iShowImage(0, 0, "assets/images/BackGround.png");
            iSetColor(199, 112, 20);
        for (int i = 0; i < pipenumber; i++)
        {
            iFilledRectangle(pipex[i], 0, pipewidth, pipey[i]);
            iFilledRectangle(pipex[i], pipey[i] + PIPEGAP, pipewidth, S_H - (pipey[i] + PIPEGAP));
        }
        }
        else if (difficulty == MEDIUM)
        {
            PIPESPEED = 4.5;
            GRAVITY = 1.5;
           
            iShowImage(0, 0, "assets/images/MEDIUMPG.png");
            iSetColor(199, 112, 20);
        for (int i = 0; i < pipenumber; i++)
        {
            iFilledRectangle(pipex[i], 0, pipewidth, pipey[i]);
            iFilledRectangle(pipex[i], pipey[i] + PIPEGAP, pipewidth, S_H - (pipey[i] + PIPEGAP));
        }
        }
        else if (difficulty == HARD)
        {
            PIPESPEED = 5.8;
            GRAVITY = 1.8;
          
            iShowImage(0, 0, "assets/images/HARD.png");
            iSetColor(199, 112, 20);
        for (int i = 0; i < pipenumber; i++)
        {
            iFilledRectangle(pipex[i], 0, pipewidth, pipey[i]);
            iFilledRectangle(pipex[i], pipey[i] + PIPEGAP, pipewidth, S_H - (pipey[i] + PIPEGAP));
        }
        }
        iShowSprite(&bird);
        iSetColor(255, 255, 255);
        char scoreText[50];
        sprintf(SCORE, "%d", score);
        sprintf(scoreText, "Score: %d", score);
        iText(10, S_H - 30, scoreText, GLUT_BITMAP_HELVETICA_18);
        if (currentState == PAUSE)
        {    iSetColor(34, 160, 191);
            iShowImage(0, 0, "assets/images/Pause_Image.png");
            iTextAdvanced(564,349,SCORE,0.5,4.5);
        }
    }
    else if (currentState == HELP)

        iShowImage(0, 0, "assets/images/HELP.png");

    else if (currentState == CREDIT)

        iShowImage(0, 0, "assets/images/creditImage.png");

    else if (currentState == GAME_OVER)
    {
        iShowImage(0, 0, "assets/images/Gameover.png");
        iSetColor(34, 160, 191);
        char scoreText[50];
        char highscoreText[50];
        sprintf(scoreText, "%d", score);
        sprintf(highscoreText, "%d", highscore);
        iTextAdvanced(460, 179, scoreText, 0.5, 4.5);
        iSetColor(235, 52, 23);
        iTextAdvanced(460, 290, highscoreText, 0.5, 4.5); 
    }
    else if( currentState == PAUSE){
        iShowImage(0,0,"assets/images/Pause_Image.png");
    }
   else if (currentState == LEADERBOARD)
    {
        iShowImage(0, 0, "assets/images/LEADERboard.png");
        
        char scoreText[50];
        iSetColor(230, 11, 59);
            sprintf(scoreText, "%d", scores[0]);
            iTextAdvanced(339, 360, scoreText, .3,4.5);
            iSetColor(255,255,255);
            sprintf(scoreText, "%d", scores[1]);
            iTextAdvanced(81, 254, scoreText, .3,4.5);
            sprintf(scoreText, "%d", scores[2]);
            iTextAdvanced(81, 139, scoreText, .3,4.5);
        
    }
}
void checkCollision()
{
    for (int i = 0; i < pipenumber; i++)
    {

        if (bird.x + bird.frames[bird.currentFrame].width > pipex[i] &&
            bird.x < pipex[i] + pipewidth)
        {
            if (bird.y < pipey[i] || bird.y + bird.frames[bird.currentFrame].height > pipey[i] + PIPEGAP)
            {
                iStopSound(channel);
                iPlaySound("assets/sounds/hit.mp3", false);
                if (score > scores[2])
                {
                    HS();   // Save to file
                }
                currentState = GAME_OVER;
               
                return;
            }
        }
    }

    if (bird.y <= 0)
    {
        iPlaySound("assets/sounds/Music/die.wav", false);
        iStopSound(channel);
        if (score > scores[2])
                {
                
                    HS();   // Save to file
                }
        currentState = GAME_OVER;
        
        return;
    }
}

void resetGame()
{
    bird.x = 180;
    bird.y = S_H / 2;
    birdVelocity = 0;
    score = 0;
    nextpipeindex = 0;
    for (int i = 0; i < pipenumber; i++)
    {
        pipex[i] = S_W + i * pipespace; // Initialize pipes at consistent intervals
        pipey[i] = rand() % 201 + 100;
        scoredPipe[i] = false;
    }
}

void movepipes()
{
    if (currentState != PLAYING)
        return;

    for (int i = 0; i < pipenumber; i++)
    {
        pipex[i] -= PIPESPEED;

        // Score when bird passes pipe
        if (!scoredPipe[i] && bird.x > pipex[i] + pipewidth)
        {
            score++;
            scoredPipe[i] = true;
        }
        if (pipex[i] + pipewidth < 0)
        {
            // Find the rightmost on-screen pipe
            int rightmost = S_W; // Default to screen width if no pipes are on-screen
            bool found = false;
            for (int j = 0; j < pipenumber; j++)
            {
                if (pipex[j] >= 0 && pipex[j] <= S_W && pipex[j] > rightmost - pipespace)
                {
                    rightmost = pipex[j];
                    found = true;
                }
            }
            if (!found)
            {
                rightmost = S_W; // Ensure new pipe starts at the right edge
            }
            pipex[i] = rightmost + pipespace;
            pipey[i] = rand() % 201 + 100;
            scoredPipe[i] = false;
            nextpipeindex = (i + 1) % pipenumber;
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
  // printf("x = %d, y = %d\n", mx, my);
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
            if (mx >= 434 && mx <= 564 && my >= 325 && my <= 354)
            {
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                currentState = SELECT_DIFFICULTY;
            }
            else if (mx >= 452 && mx <= 545 && my >= 202 && my <= 229)
            {
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                currentState = HELP;
            }
            else if (mx >= 458 && mx <= 544 && my >= 77 && my <= 102)
            {
                exit(0);
            }
            else if (mx >= 356 && mx <= 643 && my >= 264 && my <= 294)
            {
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                currentState = LEADERBOARD;
            }
            else if (mx >= 428 && mx <= 574 && my >= 138 && my <= 165)
            {
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                currentState = CREDIT;
            }
        }
        else if (currentState == SELECT_DIFFICULTY)
        {
            if (mx >= 14 && mx <= 86 && my >= 534 && my <= 543)
            {
                currentState = MENU;
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
            }
            else if (mx >= 437 && mx <= 554 && my >= 283 && my <= 319)
            {
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                difficulty = EASY;
                channel = iPlaySound("assets/sounds/EASY.mp3", true);
                currentState = PLAYING;
                resetGame();
            }
            else if (mx >= 408 && mx <= 586 && my >= 222 && my <= 250)
            {
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                difficulty = MEDIUM;
                channel = iPlaySound("assets/sounds/Medium.mp3", true);
                currentState = PLAYING;
                resetGame();
            }
            else if (mx >= 437 && mx <= 556 && my >= 158 && my <= 182)
            {   
                difficulty = HARD;
                iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                channel = iPlaySound("assets/sounds/Hard.mp3", true);
                currentState = PLAYING;
                resetGame();
            }
        }
        else if (currentState == PLAYING)
        {
            
            birdVelocity = -FLAP_STRENGTH;
        }
        else if (currentState == HELP)
        {
            
            if (mx >=  16 && mx <= 110 && my >= 528 && my <= 542)
            {iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                currentState = MENU;
            }
        }
        else if (currentState == CREDIT)
        {
            
            if (mx >=  14 && mx <= 101 && my >= 529 && my <= 540)
            {iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
                currentState = MENU;
            }
        }
        else if( currentState == GAME_OVER || currentState == LEADERBOARD){
            if (mx >=  9 && mx <= 138 && my >= 530 && my <= 543)
            {iPlaySound("assets/sounds/click-tap-computer-mouse-352734.mp3", false);
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
                birdVelocity = -FLAP_STRENGTH;
            }

        }
   else if (key == 'q' || key == 'Q')
        {
            if (currentState == PLAYING)
            {
                currentState = PAUSE;
                iPauseSound(channel);
            }
        }
        else if (key == 'r' || key == 'R')
        {
            if (currentState == PAUSE)
            {
                currentState = PLAYING;
                iResumeSound(channel);
            }
            else if (currentState == PLAYING)
            {
                iResumeSound(channel);
            }
            else 
            {   
                iResumeSound(Bgsound);
            }
        }
        else if (key == 'e' || key == 'E')
        {
            if (currentState == PAUSE)
            {
                iStopSound(channel);
                currentState = MENU;
            }
        }
        else if (key == 'p' || key == 'P')
        {
            if (currentState == PLAYING)
                iPauseSound(channel);
            else {
                iPauseSound(Bgsound);
                flag =true;
            }
        }
        else if (key == 's' || key == 'S')
        {
            if (currentState == HELP)
                iStopSound(Bgsound);
        }
        else if (key == 'b' || key == 'B')
        {
            if (currentState == CREDIT || currentState == GAME_OVER)
            {
                currentState = MENU;
            }
        }
        else if(key  == 'A' || key == 'a'){

            if(currentState == PLAYING){
            iStopSound(channel);
            }
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
// keyboard functionality
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
// for updating game 
void updateGame()
{
    if (currentState != PLAYING)

        return;

    birdVelocity += GRAVITY; // Positive gravity pulls bird down
    bird.y -= birdVelocity;  // Subtract velocity to move down when positive
    iSetSpritePosition(&bird, bird.x, bird.y);

    iAnimateSprite(&bird);

   
    if (bird.y + bird.frames[bird.currentFrame].height > S_H)
    {
        bird.y = S_H - bird.frames[bird.currentFrame].height;
        birdVelocity = 0;
    }
    checkCollision();
}

void resources()
{
    iInitSprite(&bird, -1);
    iLoadFramesFromFolder(frames, "assets/images/sprites/Brd");
    iChangeSpriteFrames(&bird, frames, 3);
    iScaleSprite(&bird, 1.3);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    glutInit(&argc, argv);
    iInitializeSound();
    resources();
     loadHighScore();
    iSetTimer(10, updateGame);
    iSetTimer(16, movepipes);
    Bgsound = iPlaySound("assets/sounds/background_music.mp3",true,5);
    iInitialize(S_W, S_H, "Flappy Bird");

    return 0;
}
