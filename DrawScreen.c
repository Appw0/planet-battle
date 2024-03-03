// Cam
// Proof of concept for rendering script. Includes all
// neccessary functions and prerecs to draw a frame.
// Using this for colors https://www.theurbanpenguin.com/4184-2/
#include "project.h"

// Map array
int map[maph][mapw];
int laserMap[maph][mapw];
int laserEffects[laserCount][laserProperty];

void drawTile(int id) {
	printColorBg(tileColors[id][0], tileColors[id][1]);
	printf("%c", tileChars[id]);
}

void drawActor(int id) {
	printColorBg(actorColors[id][0], actorColors[id][1]);
	printf("%c", actorChars[id]);
}

void drawLaser(int id) {
	printColorBg(laserEffects[id][laserColor], laserEffects[id][laserBgColor]);
	printf("%c", laserEffects[id][laserChar]);
}

int drawActorsAt(int x, int y) {;
  int val = getActorsAt(x,y,1);
  if (val > 0) drawActor(val);
  return val;
}

int drawLasersAt(int x, int y) {
	if (laserMap[y][x] > -1){
		drawLaser(laserMap[y][x]);
		return 1;
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

void drawLaserToLaserMap(int laserID) {
	int i, dX, dY;
	int x = laserEffects[laserID][laserX];
	int y = laserEffects[laserID][laserY];
	directionToXY(laserEffects[laserID][laserDirection], &dX, &dY);
	for (i = 0; i < laserEffects[laserID][laserDistance]; i++) {
		laserMap[y][x] = laserID;
		x += dX;
		y += dY;
	}
}

void computeLaserMap() {
	int i;
	memset(laserMap, -1, sizeof(laserMap));
	for (i = 0; i < laserCount; i++) {
		if (laserEffects[i][laserNeedsRender]) {
			drawLaserToLaserMap(i);
		}
	}
}

void setLasersRendered() {
	int i;
	for (i = 0; i < laserCount; i++) {
		laserEffects[i][laserNeedsRender] = 0;
	}
}

void drawPlayerEquipped() {
  printf("M: %-16s R: %-16s U: %-16s",getInventoryName(0),getInventoryName(1),getInventoryName(2));
}

// Primary Drawing Function
void drawScreen() {
    int x, y;
    int i=0;
    
    clearTerm();
    // Top line printing
    // Must be exactly this length to work for current board size
    
    
    printf("PlanetBattle - ");
    printText(topText, 17, 0);
    printf(" |");
    i = printText(sideText, 15, i);
    printf("\n");
    
    // Loops through every "pixel" to determine what to print
    for (y=0; y<maph; y++) {
        for (x=0; x<mapw; x++) {
			// Attempt to draw actors at this tile
			// If none are drawn here, draw the map tile instead.
			if (drawActorsAt(x, y) == 0) {
				if (drawLasersAt(x, y) == 0) {
					drawTile(map[y][x]);
				}
			}
        }
		resetColor();
        printf("|");
        
        // Print Side Bar Text Line
        i = printText(sideText, 15, i);
        printf("\n");
    }
    
    // Needs some padding equation here
    printf("Health: %3d%%   Shield: %3d%%     h: Help |", percent(actors[0][actorHealth],10) , actors[0][4] );
    printText(sideText, 15, i);
    printf("\n");
    drawPlayerEquipped();
    
    printf("\n");
}

// Draws inventory list, do not clear screen when drawing
void drawInventory(int selected) {

  printf("\nInventory\n");
  
  int i;
  printf("Equipped Items\n");
  
  printf("Melee  : %s",getInventoryName(0));
  printf(selected == 0 ? " <\n" : "\n");
  
  printf("Ranged : %s",getInventoryName(1));
  printf(selected == 1 ? " <\n" : "\n");
  
  printf("Utility: %s",getInventoryName(2));
  printf(selected == 2 ? " <\n" : "\n");
  
  printf("\n");

  for (i=3; i<inventorySize; i++) {
    printf("Slot %2d: %s",i-2,getInventoryName(i));
    printf(selected == i ? " <\n" : "\n");
    
  }
}

// Draws keybind list
void drawHelp() {

  printf("\nHelp\n");
  
  printf("Movement:\n");
  printf("W A S D or arrow keys to move\n");
  printf("Ctrl C to quit\n");
}
