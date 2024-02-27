// Cam
// Proof of concept for rendering script. Includes all
// neccessary functions and prerecs to draw a frame.
// Using this for colors https://www.theurbanpenguin.com/4184-2/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

// Screen Width and Height, this should be a global value passed in when this is
// Turned into a function
#define w 40
#define h 16

// actorCount, can go as high as needed (zero id monsters do nothing)
#define actorCount 16
int array[h][w];
int array2[actorCount][5]={{1,10,5,100,75}};

// Got help from reading: https://viewsourcecode.org/snaptoken/kilo/02.enteringnewTermSettingsMode.html
// Use 'a' and 'd' to schmoove, and q to exit.

struct termios originalTermSettings;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermSettings);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &originalTermSettings);
	atexit(disableRawMode);
	
	struct termios newTermSettings = originalTermSettings;
	// Disables echo and canonical mode (which usually holds input until Enter is pressed)
	newTermSettings.c_lflag &= ~(ECHO | ICANON);
	
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTermSettings);
}

// just generates a simple array for now
void tempGenArray() {
    int x, y;
    for (y=0; y<h; y++) {
        for (x=0; x<w; x++) {
            int val;
            if ( (x==0)|| (x==(w-1)) || (y==0) || (y==(h-1))) {
                val = 1;
            } else if (((x==20)||(x==21))&&!(y==10)) { 
                val = 0;
            } else {
                val = rand()%2+2;
            }
            array[y][x]=val;
        }
    }
    
}

void tempGenActors() {
  int i;
  for (i=1; i<5; i++) {
    array2[i][0]=rand()%2+2;
    array2[i][1]=rand()%(w-2)+1;
    array2[i][2]=rand()%(h-2)+1;
    array2[i][3]=1;
    array2[i][4]=1;
  }
}

// This can be cleaned up and turned into an array and moved into its own
// seperate file for multi function use
int GetTileProperties(int type) {
    switch(type) {
        case 0: // Void Tile
            return 32;
            break;
        case 1: // Wall Tile
            printf("\033[0;33m");
            return 35;
            break;
        case 2: // Floor Tile
            printf("\033[0;32m");
            return 46;
            break;
        case 3: // Misc
            printf("\033[0;36m");
            return 46;
            break;
        default:
            return 32;
    }
}

// This can be cleaned up and turned into an array and moved into its own
// seperate file for multi function use
int GetActorProperties(int type) {
    switch(type) {
        case 0: // Dead
            return 0;
            break;
        case 1: // Player Character
            printf("\033[0;33m");
            return 64;
            break;
        case 2: // Generic Monster
            printf("\033[0;31m");
            return 88;
            break;
        case 3: // Generic Monster 2
            printf("\033[0;31m");
            return 111;
            break;
        default:
            return 0;
    }
}

// Checks if actors need to be rendered first
int CheckActors(int actors[actorCount][5], int x, int y) {
  int i;
  for (i=0; i<actorCount; i++) { 
    if ((actors[i][0]>0) && ((actors[i][1]==x) && (actors[i][2]==y))) {
      return GetActorProperties(actors[i][0]);
      break;
    }
  }
  return 0;

}

// Prints Sidebar Text
int printSideBar(char text[], int i) {
    int c;
    for (c=i; c<strlen(text); c++) {
            printf("%c", text[c]);
            
            if (c==strlen(text)-1) {
                return ++c;
                break;
            }
            
            if (((text[c]==32)&&((c-i)>15))||((c-i)>18)) {
                return ++c;
                break;
            }
        }
}

// Primary Drawing Function
void drawScreen(int map[h][w], int actors[actorCount][5], char text[]) {
    int x, y;
    int i=0;
    
    printf("\e[1;1H\e[2J"); // Clear Screen
    // Top line printing
    // Must be exactly this length to work for current board size
    printf("GAME NAME - Top Text What Not           |");
    i = printSideBar(text, i);
    printf("\n");
    
    // Loops through every "pixel" to determine what to print
    for (y=0; y<h; y++) {
        for (x=0; x<w; x++) {
            int Draw;
            
            // Check to see if an actor needs to be drawn, draws first found in list
            // as such player should always be the first entry
            Draw = CheckActors(actors,x,y);
            if (Draw < 1) {
              // If not then get the draw properties of map tile
              Draw = GetTileProperties(map[y][x]);
            }
            printf("%c",Draw); // Draw Tile Sprite
            printf("\033[0m"); // Reset Color
        }
        printf("|");
        
        // Print Side Bar Text Line
        i = printSideBar(text, i);
        printf("\n");
    }
    
    
                                            // Needs some padding equation here
    printf("Health: %3d%%   Shield: %3d%%             |", actors[0][3] , actors[0][4] );
    printSideBar(text, i);
    
    printf("\n");
}

void actorRandomWalk(int actors[actorCount][5]) {
  int i, move, dir;
  for (i = 1; i<=actorCount; i++) {
  move=rand()%3-1;
  dir=rand()%2+1;
  actors[i][dir]=actors[i][dir]+move;
  
  }
}

int main()
{
    char text[]="test text to write to the iovefugiesochfehs gfjkajfv negaihfcjeakl fevajknf jeiaklcfnyoiaejside. I can wri23894713098 benwfnv9poweir,8239ncyfieowjte all sorts of stuff over here waijdjiajksadjf";
    tempGenArray();
    srand(time(NULL));
    tempGenActors();
    
    printf("\n");
    drawScreen(array, array2, text);
    printf("\n");
    
	char c = '\0';
	
	enableRawMode();
	
   int cx,cy;
	do {
   cx=array2[0][1];
   cy=array2[0][2];
   
   if ((c == 'w')||(c == 'a')||(c == 's')||(c == 'd')) {
		if ((c == 'd') && (array[cy][cx+1]>1)) {
			array2[0][1]=array2[0][1]+1;
		} else if ((c == 'a') && (array[cy][cx-1]>1)) {
			array2[0][1]=array2[0][1]-1;
		}
    if ((c == 'w') && (array[cy-1][cx]>1)) {
			array2[0][2]=array2[0][2]-1;
		} else if ((c == 's') && (array[cy+1][cx]>1)) {
			array2[0][2]=array2[0][2]+1;
		}
   
   actorRandomWalk(array2);
		  printf("\n");
      drawScreen(array, array2, text);
      printf("\n");
    
    
		if (iscntrl(c)) {
			printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
   
   }
	} while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    
    return 0;
}