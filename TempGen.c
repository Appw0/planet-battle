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
    array2[i][0]=rand()%2+2;
    array2[i][1]=rand()%(mapw-2)+1;
    array2[i][2]=rand()%(maph-2)+1;
    array2[i][3]=rand()%2+1;
    array2[i][4]=1;
  }
}

void actorRandomWalk(int actors[actorCount][actorProperty]) {
  int i, move, dir;
  for (i = 1; i<=actorCount; i++) {
  move=rand()%3-1;
  dir=rand()%2+1;
  actors[i][dir]=actors[i][dir]+move;
  
  }
}