#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <math.h>

// Screen Width and Height, this should be a global value passed in when this is
// Turned into a function
#define mapw 40
#define maph 16

// Map Properties
#define Barrier 1
#define tileTypes 4

// Actor Properties Things
#define actorCount 16
#define actorProperty 7
#define actorTypes 4
// Actor Property IDs
#define actorID 0
#define actorX 1
#define actorY 2
#define actorMoveCool 3

#define black 30
#define red 31
#define green 32
#define yellow 33
#define blue 34
#define magenta 35
#define cyan 36
#define lgray 37
#define gray 90
#define lred 91
#define lgreen 92
#define lyellow 93
#define lblue 94
#define lmagenta 95
#define lcyan 96
#define white 97

extern int map[maph][mapw];
extern int actors[actorCount][actorProperty];

extern int tileColors[tileTypes][2];
extern char tileChars[tileTypes];
extern int actorColors[actorTypes][2];
extern char actorChars[actorTypes];