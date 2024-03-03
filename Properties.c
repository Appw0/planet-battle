#include "project.h"

// actorCount, can go as high as needed (zero id monsters do nothing)
int actors[actorCount][actorProperty]={{1,10,5,0,0,10}};

// Matrix of tile colors by tile ID
// Foreground, Background
int tileColors[tileTypes][2] = {
	black, black,
	yellow, black,
	green, black,
	cyan, black
};
char tileChars[tileTypes] = " #..";

// Matrix of actor colors by actor type ID
// Foreground, Background
int actorColors[monsterTypes][2] = {
	black, black,
	yellow, black,
	red, black,
	red, black,
  black, red
};
char actorChars[monsterTypes] = " @oXX";
