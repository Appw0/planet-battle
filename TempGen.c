#include "project.h"

void tempGenArray(int array[maph][mapw]) {
    int x, y;
    for (y=0; y<maph; y++) {
        for (x=0; x<mapw; x++) {
            int val;
            if ( (x==0)|| (x==(mapw-1)) || (y==0) || (y==(maph-1))) {
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

void tempGenActors(int array2[actorCount][actorProperty]) {
  int i;
  for (i=1; i<5; i++) {
    array2[i][0]=rand()%2+2;        // Type
    array2[i][1]=rand()%(mapw-2)+1; // Pos X
    array2[i][2]=rand()%(maph-2)+1; // Pos Y
    array2[i][3]=rand()%2+1;        // Move Points
    array2[i][4]=1;                 // Weapon ID, set zero for player
    array2[i][5]=4;                 // Health (temp)
  }
}
