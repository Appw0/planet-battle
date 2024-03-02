#include "project.h"

void moveActor(int actorNum, int dir) {

  int xpos=actors[actorNum][actorX];
  int ypos=actors[actorNum][actorY];
  int dX, dY;
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
		directionToXY(dir, &dX, &dY);
		if (map[ypos + dY][xpos + dX] > Barrier) {
			actors[actorNum][actorX] += dX;
			actors[actorNum][actorY] += dY;
		}
       
      }
    } else {
      actors[actorNum][actorMoveCool]=actors[actorNum][actorMoveCool]-1;
    }
 
}

void move(char c) {

  int dir = -1;
  
  if (c=='w') {
  	dir=north;
 	}
  if (c=='a') {
  	dir=east;
 	}
  if (c=='s') {
  	dir=south;
 	}
  if (c=='d') {
  	dir=west;
 	}
  
  // This is not good. This function should be split into AI and player movement, and not take a char as input.
  if (dir != -1) moveActor(0,dir);
  
  int i;
  for (i=1; i<actorCount; i++) {
    if (actors[i][actorTypeID]>1) {   
      moveActor(i,rand()%4);
      
    }
  }
}
