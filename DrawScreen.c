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

int drawDeadActorsAt(int x, int y) {
	int id = getAnyActorAtXY(x, y, 1);
	if (isValidActorID(id) && isActorDead(id)) {
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

int drawItemAt(int x, int y) {
	int id = getDroppedItemAtXY(x, y);
	if (isValidDroppedItemID(id)) {
		// TODO: make itemCategory store this info instead of it being hard coded
		switch(droppedItems[id].type->category) {
			case itemCategoryMelee:
				printColorBg(lyellow, black);
				printf("!");
				break;
			case itemCategoryRanged:
				printColorBg(lyellow, black);
				printf("⌐");
				break;
			case itemCategoryUtility:
				printColorBg(lyellow, black);
				printf("☼");
				break;
			case itemCategoryDatapad:
				printColorBg(lyellow, black);
				printf("♦");
				break;
			default:
				printColorBg(lyellow, black);
				printf("¡");
		}
		return 1;
	}
	return 0;
}

int getWordLength(char text[], int offset) {
  int max=strlen(text);
  int i=0;
  
  for(i=0; i<(max-offset); i++) {
    if ((text[i+offset] == ' ')||(text[i+offset] == '\n')||(text[i+offset] == '.')) {
        return i;
    }
  }
}


// Prints text with padding and wrapping (if put in a loop)
int printText(char text[], int width, int start) {
    int i;
    int max = strlen(text);
    int wordLength;
    
    
    if (start>max) {
         for (i=0; i<width; i++) {
             printf(" ");
         }
    } else {
        for (i=0; i<width; i++) {
            wordLength=getWordLength(text, start+i);
            
            if ((text[start+i]=='\n')&&(i<width)) {
                return i+start+1;
            }
            
            if ((text[start+i]==' ')&&(i>width-3)) {
                return i+start+1;
            }
            
            if (((wordLength)>width-i)&&(wordLength<width)) {
                return i+start;
            }
            
            if (!((i==0)&&((text[start+i]=='\n')||(text[start+i]==' ')))) {
              if (i+start < max) {
                  printf("%c",text[start+i], wordLength,i+start);
              } else {
                  printf(" ");
              }
            } else {
              i++;
            }
        }
    }
    return i+start;
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
	printf("M: %-16s ", getMeleeWeaponPtr(getPlayerID())->displayName);
	printf("R: %-16s ", getRangedWeaponPtr(getPlayerID())->displayName);
	printf("U: %-16s", getUtilItemPtr(getPlayerID())->displayName );
}

// Primary Drawing Function
void drawScreen() {
    int x, y;
    int i=0;
    
	resetColor();
    clearTerm();
	
    // Top line printing
    // Must be exactly this length to work for current board size
    
    
    printf("PlanetBattle - ");
    int screenWidth;
    screenWidth = mapWidth < 40 ? 40 : mapWidth;
    printText(topText, screenWidth-15, 0);
    printf("│");
    i = printText(sideText, 25, i);
    printf("\n");
    
    for(x=0;x<screenWidth;x++) {
      printf("─");
    }
    printf("┤");
    i = printText(sideText, 25, i);
    printf("\n");
    
    // Loops through every "pixel" to determine what to print
    for (y = 0; y < mapHeight; y++) {
        for (x = 0; x < mapWidth; x++) {
			// Attempt to draw actors at this tile
			// If none are drawn here, draw the map tile instead.
			if (drawActorsAt(x, y) == 0) {
				if (drawLasersAt(x, y) == 0) {
					if (drawDeadActorsAt(x, y) == 0) {
						if (drawItemAt(x, y) == 0) {
							drawTile(map[y][x]);
						}
					}
				}
			}
        }
       if (mapWidth<40) {
         for (x=mapWidth; x<40; x++) {
           printf(" ");
         }
       }
        
		resetColor();
        printf("│");
        
        // Print Side Bar Text Line
        i = printText(sideText, 25, i);
        printf("\n");
    }
    
    for(x=0;x<screenWidth;x++) {
      printf("─");
    }
    printf("┴");
    for(x=0;x<25;x++) {
      printf("─");
    }
    printf("\n");
    
	if (!playerDied) {
		// Needs some padding equation here
		printf("Health: %3d%%   Shield: %3d%%     h: Help", percent(actors[getPlayerID()].health, 10), actors[getPlayerID()].shield);
		printf("\n");
		drawPlayerEquipped();
	}
    
    printf("\n");
}

// Draws inventory list, do not clear screen when drawing
void drawInventory(int selected) {

  printf("\nInventory\n");
  
  int i;
  printf("Equipped Items\n");
  
  printf("Melee  : %s", getMeleeWeaponPtr(getPlayerID())->displayName);
  printf(selected == 0 ? " <\n" : "\n");
  
  printf("Ranged : %s", getRangedWeaponPtr(getPlayerID())->displayName);
  printf(selected == 1 ? " <\n" : "\n");
  
  printf("Utility: %s", getUtilItemPtr(getPlayerID())->displayName);
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
  if ((strlen(sideText)+strlen(newText)+1)>((mapHeight-2)*25)) {
    len=strlen(newText)+1;
    int i;
    for (i = 0; i<len; i++) {
      leftShift(sideText,strlen(sideText)+1);
    }
  }
  strcat(sideText, newText);
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
