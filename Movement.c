#include "project.h"

void moveActorAndAttack(int actorID, int dir) {

  int xpos = actors[actorID].x;
  int ypos = actors[actorID].y;
  int dX, dY;
  
  actors[actorID].moveCooldown -= 1;
  if (actors[actorID].moveCooldown <= 0) {
    if (!isActorPlayer(actorID)) {
      actors[actorID].moveCooldown = actors[actorID].type->slowness;
    } else {
      actors[actorID].moveCooldown = 0;
    }
      if (!meleeAttack(dir, xpos, ypos, actorID)) {
        
        // Move in direction
		directionToXY(dir, &dX, &dY);
		
		if (isTileWalkable(xpos + dX, ypos + dY)) {
			actors[actorID].x += dX;
			actors[actorID].y += dY;
		}
       
      }
    }
 
}

int isTileWalkable(int x, int y) {
	return !tiles[map[y][x]].blockMove;
}

int tileBlocksLasers(int x, int y) {
	return tiles[map[y][x]].blockLaser;
}

void doActorAI(int actorID) {
	int dir, x, y, dX, dY, nearbyActorID;
	dir = pathfind(actorID); // Where we want to go to get to the player
	directionToXY(dir, &dX, &dY);
	getActorXY(actorID, &x, &y);
	
	nearbyActorID = getActorAtXY(x + dX, y + dY);
	// If there isn't anything there, or there is and it's not another NPC, then move and attack!
	if (!isValidActorID(nearbyActorID) || isValidActorID(nearbyActorID) && !actorHasAI(nearbyActorID)) {
		moveActorAndAttack(actorID, dir);
	}
}

void doAI() {
	int i;
	for (i = 0; i < actorsCreated; i++) {
		if (actorHasAI(i)) {
			doActorAI(i);
		}
	}
}

int playerMovement(char keyCode[8]) {
	int playerID = getPlayerID();
	if (keyCode[0] == 'w' || strcmp(keyCode, "\e[A") == 0) {
		moveActorAndAttack(playerID, north);
		return 1;
	} else if (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) {
		moveActorAndAttack(playerID, west);
		return 1;
	} else if (keyCode[0] == 's' || strcmp(keyCode, "\e[B") == 0) {
		moveActorAndAttack(playerID, south);
		return 1;
	} else if (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) {
		moveActorAndAttack(playerID, east);
		return 1;
	}
	return 0;
}

// Just a straight line walk towards player
int pathfind(int actorID) {  
  int difx, dify, choose, x, y, dir, absDist;
  
  difx = actors[getPlayerID()].x - actors[actorID].x;
  dify = actors[getPlayerID()].y - actors[actorID].y;
  
  absDist=(int)round(sqrt(pow((double)difx,2)+pow((double)dify,2)));
  choose=rand()%3;
  
  if ((choose==0)&&(absDist>6)) {
    return randomDirection();
  } else {
    x = difx > 0 ? east : west; 
    y = dify > 0 ? south : north;
    dir = abs(difx) > abs(dify) ? x : y;
    return dir;
  }
}
