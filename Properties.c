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
int actorColors[actorTypes][2] = {
	black, black,
	yellow, black,
	red, black,
	red, black
};
char actorChars[actorTypes] = " @Xo";

void drawTile(int id) {
	printColorBg(tileColors[id][0], tileColors[id][1]);
	printf("%c", tileChars[id]);
}

void drawActor(int id) {
	printColorBg(actorColors[id][0], actorColors[id][1]);
	printf("%c", actorChars[id]);
}
