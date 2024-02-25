// Cam
// Proof of concept for rendering script. Includes all
// neccessary functions and prerecs to draw a frame.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Screen Width and Height, this should be a global value passed in when this is
// Turned into a function
#define w 40
#define h 15

// actorCount, can go as high as needed (zero id monsters do nothing)
#define actorCount 4
int array[h][w];

// just generates a simple array for now
void tempGenArray() {
    int x, y;
    for (y=0; y<h; y++) {
        for (x=0; x<w; x++) {
            int val;
            if (((x==0)||(x==39))||((y==0)||(y==14))) {
                val = 1;
            } else {
                val = rand()%2+2;
            }
            array[y][x]=val;
        }
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
    
    
    printf("Health: %d   Shield: %d               |",100,100);
    printSideBar(text, i);
    
    printf("\n");
}


int main()
{
    char text[]="test text to write to the side. I can write all sorts of stuff over here waijdjiajksadjf";
    int actors[actorCount][5]={{1,10,5,1,1}
                              ,{2,16,11,1,1}
                              ,{3,12,9,1,1}};
    tempGenArray();
    
    printf("\n");
    drawScreen(array, actors, text);
    printf("\n");
    
    return 0;
}
