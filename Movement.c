#include "project.h"

void moveActor(int actorNum, int dir) {

  int xpos=actors[actorNum][actorX];
  int ypos=actors[actorNum][actorY];
  int attackDone;
  
  // Dec the actor move cooldown
  if (actors[actorNum][actorMoveCool] == 0) {
    if (actorNum > 0) {
      actors[actorNum][actorMoveCool]=rand()%2+1;
    } else {
      actors[actorNum][actorMoveCool] = 0;
    }
      attackDone = Attack(dir,xpos,ypos,actorNum);
      printf("%d",attackDone);
      if ( attackDone == 0 ) {
        
        // Move in direction
        if ((dir==1) && (map[ypos-1][xpos]>Barrier)) {
      		actors[actorNum][actorY]=actors[actorNum][actorY]-1;
      	}
        if ((dir==2) && (map[ypos][xpos-1]>Barrier)) {
      		actors[actorNum][actorX]=actors[actorNum][actorX]-1;
      	}
        if ((dir==3) && (map[ypos+1][xpos]>Barrier)) {
      		actors[actorNum][actorY]=actors[actorNum][actorY]+1;
      	}
        if ((dir==4) && (map[ypos][xpos+1]>Barrier)) {
      		actors[actorNum][actorX]=actors[actorNum][actorX]+1;
      	}
       
      }
    } else {
      actors[actorNum][actorMoveCool]=actors[actorNum][actorMoveCool]-1;
    }
 
}

void move(char c) {

  int dir;
  
  if (c=='w') {
  	dir=1;
 	}
  if (c=='a') {
  	dir=2;
 	}
  if (c=='s') {
  	dir=3;
 	}
  if (c=='d') {
  	dir=4;
 	}
  
  moveActor(0,dir);
  
  int i;
  for (i=1; i<actorCount; i++) {
    if (actors[i][actorTypeID]>1) {   
      moveActor(i,rand()%4+1);
      
    }
  }
}
