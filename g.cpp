#include "iGraphics.h"
#include "iSound.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 560
#define GRAVITY 0.5f
#define FLAP_STRENGTH -10.0f

Image frames[14];
Sprite bird;
int channel;
float birdVelocity = 0;


enum GameState {
    MENU,
    PLAYING
};
enum GameState currentState = MENU;

void iDraw() {
    iClear();
    
    if (currentState == MENU) {
        iShowImage(0, 0, "assets/images/MenuImage.png");
        iSetColor(255, 255, 255);
        iText(450, 250, "START", GLUT_BITMAP_HELVETICA_18);
    }
    else if (currentState == PLAYING) {
        // Draw sky background
        iSetColor(135, 206, 235);
        iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Draw bird
        iShowSprite(&bird);
        
        // Draw instructions
        iSetColor(0, 0, 0);
        iText(SCREEN_WIDTH/2 - 100, 50, "Press SPACE to flap", GLUT_BITMAP_HELVETICA_18);
    }
}

void updateGame() {
    if (currentState != PLAYING) return;

    // Apply gravity (makes bird fall down)
    birdVelocity += GRAVITY;
    bird.y += birdVelocity;
    iSetSpritePosition(&bird, bird.x, bird.y);

    // Animate the bird
    iAnimateSprite(&bird);

    // Keep bird within screen bounds
    if (bird.y < 0) {
        bird.y = 0;
        birdVelocity = 0;
    }
    if (bird.y + bird.frames[bird.currentFrame].height > SCREEN_HEIGHT) {
        bird.y = SCREEN_HEIGHT - bird.frames[bird.currentFrame].height;
        birdVelocity = 0;
    }
}

void iMouse(int button, int state, int mx, int my) {
   
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentState == MENU) {
            // Convert y-coordinate
            int glY = SCREEN_HEIGHT - my;
            
            // Check if click is within START text area
            if (mx >= 450 && mx <= 550 && glY >= 230 && glY <= 270) {
                currentState = PLAYING;
                // Reset bird position
                iSetSpritePosition(&bird, SCREEN_WIDTH/4, SCREEN_HEIGHT/2);
                birdVelocity = 0;
            }
        }
        else if (currentState == PLAYING) {
            // Flap on click (negative velocity makes bird go up)
            birdVelocity = FLAP_STRENGTH;
        }
    }
}

void iKeyboard(unsigned char key) {
    if (key == ' ') {
        if (currentState == PLAYING) {
            // Space makes bird flap upwards
            birdVelocity = FLAP_STRENGTH;
        }
        else if (currentState == MENU) {
            currentState = PLAYING;
            iSetSpritePosition(&bird, SCREEN_WIDTH/4, SCREEN_HEIGHT/2);
            birdVelocity = 0;
        }
    }
    else if (key == 27) { // ESC key
        if (currentState == PLAYING) {
            currentState = MENU;
        }
    }
}


void iMouseMove(int mx, int my) {
  
}

void iMouseDrag(int mx, int my) {
  
}

void iMouseWheel(int dir, int mx, int my) {
   
}

void iSpecialKeyboard(unsigned char key) {
    switch (key) {
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
void resources() {
    iInitSprite(&bird, -1);
    iLoadFramesFromFolder(frames, "assets/images/sprites/FB");
    iChangeSpriteFrames(&bird, frames, 14);
    iSetSpritePosition(&bird, SCREEN_WIDTH/4, SCREEN_HEIGHT/2);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    iInitializeSound();
    resources();
    
    // Set up animation timer (16ms for ~60fps)
    iSetTimer(16, updateGame);
    
    channel = iPlaySound("assets/sounds/NGGYU.MP3", true, 50);
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
    return 0;
}