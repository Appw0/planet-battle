#include "project.h"
//increments the actor cooldown to ), checks if a melee attack exists, checks if the the tile is walkable.
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
//Calculates the new position, increments the cooldown, then physically updates the actors position, test the direction and ability for validity
void moveActorAndRangeAttack(int actorID, struct position movement, int shootDir, int shouldFire) {
	struct position pos = getActorPosition(actorID), newPos;
	newPos = posAdd(pos, movement);
	
	actors[actorID].moveCooldown -= 1;
	actors[actorID].dangerDir = -1;
	
	if (actors[actorID].moveCooldown <= 0) {
		actors[actorID].moveCooldown = actors[actorID].type->slowness;
		
		if (isTileWalkable(newPos.x, newPos.y)) {
			actors[actorID].x = newPos.x;
			actors[actorID].y = newPos.y;
		}
		
		if (shootDir > -1 && shouldFire) {
			actorShootLaser(actorID, shootDir);
		}
	} else if (actors[actorID].moveCooldown == 1) {
		actors[actorID].dangerDir = shootDir;
	}
}
//Checks that blockMove is not true and that there is no actor at that position
int isTileWalkable(int x, int y) {
	return !tiles[map[y][x]].blockMove && !isValidActorID(getActorAtXY(x, y));
}
//Does the tile have a true block laser value
int tileBlocksLasers(int x, int y) {
	return tiles[map[y][x]].blockLaser;
}
//Pathfind defined line 131
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
//RangedPathfind found on line 155, in its current itterattion if no valid actors are near the NPC makes a 50/50 descision to fire
void doRangedActorAI(int actorID) {
	int nearbyActorID, shootDir, linedUp, moveDir = rangedPathfind(actorID, &shootDir, &linedUp);
	struct position movement = directionToPos(moveDir), pos = getActorPosition(actorID);
	
	nearbyActorID = getActorAt(posAdd(movement, pos));
	
	if (!isValidActorID(nearbyActorID)) {
		// TODO: replace rand() with an AI config thing specific to each actorType
		// This will control how trigger-happy each enemy is
		moveActorAndRangeAttack(actorID, movement, shootDir, rand()%2 == 0 ? 1 : linedUp);
	}
}
//Using the functions above to to determine quantity of AI and types 
void doAI() {
	int id;
	for (id = 0; id < actorsCreated; id++) {
		struct itemTypeData rangedWeapon = getRangedWeapon(id);
		
		if (actorHasAI(id)) {
			if (itemNameIs(rangedWeapon, "none")) {
				doActorAI(id);
			} else {
				doRangedActorAI(id);
			}
		}
	}
}
//Assigning WASD and the arrows to move the player (Uses function found on line 3)
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

int playerCheckPortals() {
	char text[64];
	int i, playerID = getPlayerID();
	for (i = 0; i < portalsCreated; i++) {
		if (portals[i].x == actors[playerID].x && portals[i].y == actors[playerID].y) {
			// TODO: More fanfare! Maybe also ask to confirm...
			loadLevel(portals[i].level);
			snprintf(text, 64, $lmagenta "Moved to %s\n", topText); //snprintf similar to sprintf, but better at preventing overflow
			updateSideText(text); 
		}
	}
}

// Just a straight line walk towards player
int pathfind(int actorID) {  
	int difx, dify, choose = 0, x, y, dir, absDist = 100;
	
	if (!playerDied) {
		difx = actors[getPlayerID()].x - actors[actorID].x;
		dify = actors[getPlayerID()].y - actors[actorID].y;
		absDist = (int)round(sqrt(pow((double)difx, 2) + pow((double)dify, 2)));
		choose = rand()%3;
	}
	
	if (choose == 0 && absDist > 6) {
		return randomDirection();
	} else {
		x = difx > 0 ? east : west; 
		y = dify > 0 ? south : north;
		dir = abs(difx) > abs(dify) ? x : y;
		return dir;
	}
}

int rangedPathfind(int actorID, int* shootDirection, int* shotLinedUp) {
	struct position dif = {0, 0}, pos = getActorPosition(actorID), playerPos = getActorPosition(getPlayerID());
	dif = posSubtract(pos, playerPos);
	(*shotLinedUp) = 1; //is my shot lined up?
	
	if (dif.x != 0 && dif.y != 0) { //are we diagonal?
		(*shotLinedUp) = 0;
		if (abs(dif.x) < abs(dif.y)) {
			(*shootDirection) = dif.y > 0 ? north : south;
			return dif.x > 0 ? west : east;
		} else { 
			(*shootDirection) = dif.x > 0 ? west : east;
			return dif.y > 0 ? north : south;
		}
	} else if (dif.x == 0) { //Are you above or below me
		(*shootDirection) = dif.y > 0 ? north : south;
		return dif.y < 0 ? north : south;
	} else { //Are you to my right or left
		(*shootDirection) = dif.x > 0 ? west : east;
		return dif.x < 0 ? west : east;
	}
}
