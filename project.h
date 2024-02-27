#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

// Screen Width and Height, this should be a global value passed in when this is
// Turned into a function
#define mapw 40
#define maph 16

// Map Properties
#define Barrier 1

// Actor Properties Things
#define actorCount 16
#define actorProperty 7
// Actor Property IDs
#define actorID 0
#define actorX 1
#define actorY 2
#define actorMoveCool 3

extern int map[maph][mapw];
extern int actors[actorCount][actorProperty];