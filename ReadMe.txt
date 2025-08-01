## Flappy Bird game using iGraphics and iSound ##


# Project Overview:
This is a Flappy Bird-style game created in C using the 'iGrpahics.h' and 'iSound.h' libraries. The game includes animations, sound effects, obstacle collision 
detection,score tracking, atop 3 leaderboard and a difficulty selection menu. It also lets players enter their names and shows who got the highest scores.


# Features:
-> Flappy Bird mechanics (flap and fall)
-> Animated bird frames
-> Moving pipes with gaps
-> Gravity + flap strength physics
-> Difficulty selection: 1.Easy
                         2.Medium
                         3.Hard
-> Real-time score tracking
-> Game over detection and restart
-> Background music and sound effects
-> Leaderboard system: stores top 3 scores with names
-> Displays high scores at game over
-> Playercam enter their name before playing


# File structure:
-> 'main.c': Main game code (your file)
-> 'iGraphics.h'/'iSound.h': Graphics and sound libraries (required)
-> 'score.txt': Store top 3 scores
-> 'name.txt.':Stores names of top 3 players
-> 'bgm.wav','flap.wav','dead.wav': Sound effects
-> 'images/': Contains bird sprites, background, menu images,pipe, etc.


# Controls:
-> 'SPACE' key or 'Mouse click': Bird flaps 
-> Mouse click: For selecting difficulty and menu items


# How to Compile and Run:
-> Install the 'iGraphics' and 'iSound' libraries properly in your Compiler.
-> Place all the images and sound files in the correct folder (matching your code).
-> Compile 'main.c' using a C compiler that supports 'graphics.h' style headers.
-> Run the executables.
-> Game will start with: 1.A name input screen
                         2.Difficulty selection
                         3.Gameplay


# Leaderboard Details:
-> Top 3 scores are saved permanently in 'score.txt.
-> Corresponding player names are stored in 'name.txt'
-> After game over, If your score is in the top 3: 1.It replaces the lowest score
                                                   2.Updates are shown instantly on the screen
                                                   3.Displays 'New 1st High Score','New 2nd High score'or'New 3rd High Score'
-> Leaderboard is shown at the end of each game


# Authors:
-> Mahir Tajwar
-> Afia Ayesha Mithi

This project was built as a fun way to learn graphics, file handling and gameplay ligic in C.


# Notes:
-> Make sure all image and sound files are in the correct folder.
-> Tested with standard iGraphics library setup.
-> Leaderboard file will be created automatically if not found.




