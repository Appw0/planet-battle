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
      if ( attackDone == 0 ) {
        
        // Move in direction
		directionToXY(dir, &dX, &dY);
		if (!isTileSolid(xpos + dX, ypos + dY)) {
			actors[actorNum][actorX] += dX;
			actors[actorNum][actorY] += dY;
		}
       
      }
    } else {
      actors[actorNum][actorMoveCool]=actors[actorNum][actorMoveCool]-1;
    }
 
}

int isTileSolid(int x, int y) {
	return map[y][x] <= Barrier;
}

void moveAI() {
  int i;
  for (i=1; i<actorCount; i++) {
    if (actors[i][actorTypeID]>1) {   
      moveActor(i,pathfind(i));
      
    }
  }
}

int playerMovement(char keyCode[8]) {
	if (keyCode[0] == 'w' || strcmp(keyCode, "\e[A") == 0) {
		moveActor(0, north);
		return 1;
	} else if (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) {
		moveActor(0, west);
		return 1;
	} else if (keyCode[0] == 's' || strcmp(keyCode, "\e[B") == 0) {
		moveActor(0, south);
		return 1;
	} else if (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) {
		moveActor(0, east);
		return 1;
	}
	return 0;
}

// Just a straight line walk towards player
int pathfind(int actorID) {  
  int difx, dify, choose, x, y, dir, absDist;
  
  difx = actors[0][actorX]-actors[actorID][actorX];
  dify = actors[0][actorY]-actors[actorID][actorY];
  
  absDist=(int)round(sqrt(pow((double)difx,2)+pow((double)dify,2)));
  choose=rand()%3;
  
  if ((choose==0)&&(absDist>6)) {
    return rand()%4+1;
  } else {
    x = difx > 0 ? east : west; 
    y = dify > 0 ? south : north;
    dir = abs(difx) > abs(dify) ? x : y;
    return dir;
  }
}
