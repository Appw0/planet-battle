// Cam
// Proof of concept for rendering script. Includes all
// neccessary functions and prerecs to draw a frame.
// Using this for colors https://www.theurbanpenguin.com/4184-2/
#include "project.h"

// Map array
int laserMap[mapMaxHeight][mapMaxWidth];
int laserEffects[laserCount][laserProperty];

void drawTile(int tileID) {
	printColorBg(tiles[tileID].color, tiles[tileID].bgColor);
	printf("%c", tiles[tileID].tile);
}

void drawActorByType(struct actorTypeData actorType) {
	printColorBg(actorType.color, actorType.bgColor);
	printf("%c", actorType.tile);
}

void drawActor(int actorID) {
	drawActorByType(*(actors[actorID].type));
}

void drawLaser(int id) {
	printColorBg(laserEffects[id][laserColor], laserEffects[id][laserBgColor]);
	printf("%c", laserEffects[id][laserChar]);
}

int drawActorsAt(int x, int y) {
	int id = getActorAtXY(x, y);
	if (isValidActorID(id)) {
		drawActor(id);
		return 1;
	}
	return 0;
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
		if (isXYOnMap(x, y)) {
			laserMap[y][x] = laserID;
			x += dX;
			y += dY;
		} else {
			break;
		}
		
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

int nextLaserEffectID() {
	int i;
	for (i = 0; i < laserCount; i++) {
		if (!laserEffects[i][laserNeedsRender]) {
			return i;
		}
	}
	printf($lred "Too many lasers!");
	return 0;
}

void createLaserEffect(char lChar, int lColor, int lBgColor, int x, int y, int direction, int distance) {
	int id = nextLaserEffectID();
	laserEffects[id][laserNeedsRender] = 1;
	laserEffects[id][laserChar] = lChar;
	laserEffects[id][laserColor] = lColor;
	laserEffects[id][laserBgColor] = lBgColor;
	laserEffects[id][laserX] = x;
	laserEffects[id][laserY] = y;
	laserEffects[id][laserDirection] = direction;
	laserEffects[id][laserDistance] = distance;
}

void drawPlayerEquipped() {
  printf("M: %-16s ", getMeleeWeapon(getPlayerID()).displayName);
  printf("R: %-16s ", getRangedWeapon(getPlayerID()).displayName);
  printf("U: %-16s", getUtilItem(getPlayerID()).displayName );
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
    i = printText(sideText, 25, i);
    printf("\n");
    
    // Loops through every "pixel" to determine what to print
    for (y = 0; y < mapHeight; y++) {
        for (x = 0; x < mapWidth; x++) {
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
        i = printText(sideText, 25, i);
        printf("\n");
    }
	
	// for (y = 0; y < mapHeight; y++) {
        // for (x = 0; x < mapWidth; x++) {
			// printf("%d", laserMap[y][x]);
		// }
        // printf("\n");
    // }
    
    // Needs some padding equation here
    printf("Health: %3d%%   Shield: %3d%%     h: Help |", percent(actors[getPlayerID()].health, 10), actors[getPlayerID()].shield);
    printText(sideText, 25, i);
    printf("\n");
    drawPlayerEquipped();
    
    printf("\n");
}

// Draws inventory list, do not clear screen when drawing
void drawInventory(int selected) {

  printf("\nInventory\n");
  
  int i;
  printf("Equipped Items\n");
  
  printf("Melee  : %s", getMeleeWeapon(getPlayerID()).displayName);
  printf(selected == 0 ? " <\n" : "\n");
  
  printf("Ranged : %s", getRangedWeapon(getPlayerID()).displayName);
  printf(selected == 1 ? " <\n" : "\n");
  
  printf("Utility: %s", getUtilItem(getPlayerID()).displayName);
  printf(selected == 2 ? " <\n" : "\n");
  
  printf("\n");

  for (i=3; i<inventorySize; i++) {
    printf("Slot %2d: %s",i-2, items[playerInventory[i]].displayName);
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

void updateSideText(char newText[]) {
  int len;
  if ((strlen(sideText)+strlen(newText)+1)>330) {
    len=strlen(newText)+1;
    int i;
    for (i = 0; i<len; i++) {
      leftShift(sideText,strlen(sideText)+1);
    }
  }
  strcat(sideText, newText);
  strcat(sideText, " ");
  drawScreen();
  
}

int leftShift(char *words, int len)
{
    int i;
    for(i = 1; i < len; i++)
    {
        words[i - 1] = words[i];
    }
    len--;
    return len;
}
