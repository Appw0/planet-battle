#include "project.h"

void moveActor(int actorID, int dir) {

  int xpos=actors[actorID][actorX];
  int ypos=actors[actorID][actorY];
  int attackDone;
  
  // Dec the actor move cooldown
  if (actors[actorID][actorMoveCool] == 0) {
    if (actorID > 0) {
      actors[actorID][actorMoveCool]=rand()%2+1;
    } else {
      actors[actorID][actorMoveCool] = 0;
    }
      attackDone = Attack(dir,xpos,ypos,actorID);
      printf("%d",attackDone);
      if ( attackDone == 0 ) {
        
        // Move in direction
        if ((dir==1) && (map[ypos-1][xpos]>Barrier)) {
      		actors[actorID][actorY]=actors[actorID][actorY]-1;
      	}
        if ((dir==2) && (map[ypos][xpos-1]>Barrier)) {
      		actors[actorID][actorX]=actors[actorID][actorX]-1;
      	}
        if ((dir==3) && (map[ypos+1][xpos]>Barrier)) {
      		actors[actorID][actorY]=actors[actorID][actorY]+1;
      	}
        if ((dir==4) && (map[ypos][xpos+1]>Barrier)) {
      		actors[actorID][actorX]=actors[actorID][actorX]+1;
      	}
       
      }
    } else {
      actors[actorID][actorMoveCool]=actors[actorID][actorMoveCool]-1;
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
      moveActor(i,pathfind(i));
      
    }
  }
}

// Just a straight line walk towards player
int pathfind(int actorID) {  
  int difx, dify, choose, x, y, dir;
  
  difx = actors[0][actorX]-actors[actorID][actorX];
  dify = actors[0][actorY]-actors[actorID][actorY];
  
  choose=rand()%4;
  
  if (choose==0) {
    return rand()%4+1;
  } else {
    x = difx > 0 ? 4 : 2; 
    y = dify > 0 ? 3 : 1;
    dir = abs(difx) > abs(dify) ? x : y;
    return dir;
  }
}







