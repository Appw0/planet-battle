// Cam
// Proof of concept for rendering script. Includes all
// neccessary functions and prerecs to draw a frame.
// Using this for colors https://www.theurbanpenguin.com/4184-2/
#include "project.h"

// Map array
int map[maph][mapw];

// Checks if actors need to be rendered first
int drawActorsAt(int x, int y) {
  int i;
  for (i=0; i<actorCount; i++) { 
    if ((actors[i][actorID]>0) && ((actors[i][actorX]==x) && (actors[i][actorY]==y))) {
      drawActor(actors[i][actorID]);
      return 1;
    }
  }
  return 0;
}

// Prints text with padding and wrapping (if put in a loop)
int printText(char text[], int width, int start) {
    int overflow = 5;
    int c,i;
    for (c=start; c<strlen(text); c++) {
            printf("%c", text[c]);
            
            if (c==strlen(text)-1) {
                for (i=(c-start); i<=width+overflow; i++) {
                    printf(" ");
                }
                return ++c;
                break;
            }
            
            if (((text[c]==32)&&((c-start)>width))||((c-start)>(width+overflow))) {
                for (i=(c-start); i<=width+overflow; i++) {
                    printf(" ");
                }
                return ++c;
                break;
            }
        }
}

// Primary Drawing Function
void drawScreen(char topText[] , char sideText[]) {
    int x, y;
    int i=0;
    
    clearTerm();
    // Top line printing
    // Must be exactly this length to work for current board size
    
    
    printf("GAME NAME - ");
    printText(topText, 20, 0);
    printf(" |");
    i = printText(sideText, 15, i);
    printf("\n");
    
    // Loops through every "pixel" to determine what to print
    for (y=0; y<maph; y++) {
        for (x=0; x<mapw; x++) {
			// Attempt to draw actors at this tile
			// If none are drawn here, draw the map tile instead.
			if (drawActorsAt(x, y) == 0) drawTile(map[y][x]);
        }
		resetColor();
        printf("|");
        
        // Print Side Bar Text Line
        i = printText(sideText, 15, i);
        printf("\n");
    }
    
    // Needs some padding equation here
    printf("Health: %3d%%   Shield: %3d%%             |", actors[0][3] , actors[0][4] );
    printText(sideText, 15, i);
    
    printf("\n");
}
