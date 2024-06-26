#include "project.h"

void damageActor(int actorID, int amount) {
	actors[actorID].health -= amount;
	char text[100];
  
  // TODO: Move this to a seperate function for logging damage.
  // And then call it from meleeAttack or actorShootLaser
  // This way we have access to the full context, and can print out
  // the name of what is being hit and what is doing the damage.
  if (isActorPlayer(actorID)) {
    sprintf(text, $red "Got hit for %d.\n", amount);
  } else {
    sprintf(text, $green "Hit enemy for %d.\n", amount);
  }
  updateSideText(text);  
 
	if (actors[actorID].health < 1) {
		if (isActorPlayer(actorID)) {
			playerDied = 1;
		}
		actors[actorID].type = getActorTypePtr("dead");
	}
}

// TODO: dodge chances for laser attacks, should be a separate function called in this one
void damageActors(int actorIDs[], int number, int damageAmount) {
	int i;
	for (i = 0; i < number; i++) {
		if (isValidActorID(actorIDs[i])) damageActor(actorIDs[i], damageAmount);
	}
}

int meleeAttack(int dir, int xpos, int ypos, int actorID) {
	int targetActorID, dX, dY;
	
	directionToXY(dir, &dX, &dY);
	targetActorID = getActorAtXY(xpos + dX, ypos + dY);
	
	struct itemTypeData itemType = getMeleeWeapon(actorID);
	if (!itemNameIs(itemType, "none") && isValidActorID(targetActorID)) {
		damageActor(targetActorID, itemType.damage);
		return 1;
	}
	return 0;
}

// Shoots a laser weapon held by an actor in a direction. This assumes this actor actually has a laser weapon!
void actorShootLaser(int actorID, int direction) {
	struct itemTypeData weapon = getRangedWeapon(actorID);
	int hitIDs[8], dist, dX, dY, x, y;
	
	getActorXY(actorID, &x, &y);
	directionToXY(direction, &dX, &dY);
	
	dist = laserRaycast(x + dX, y + dY, direction, hitIDs, 8);
	
	damageActors(hitIDs, 8, weapon.damage);
	
	// Temporary settings
	// TODO: actually have some sort of array that maps weapons to laser colors and stuff
	createLaserEffect(direction == north || direction == south ? '|' : '-', lred, black, x + dX, y + dY, direction, dist);
}

// Lets the player aim a laser weapon, and returns if they actually shot it.
int playerAimLaser() {
	int shotLaser = 0, playerID = getPlayerID();
	char keyCode[8] = "\0";
	
	if (itemNameIs(getRangedWeapon(playerID), "none")) {
		printColor(lred);
		printf("You don't have a ranged weapon!\n");
		return 0;
	}
	
	computeLaserMap();
	drawScreen();
	printColor(lyellow);
	printf("Press a direction key to fire!\n");
	
	while (!shotLaser && readKey(keyCode, 8)) {
		// TODO: consolidate these funny repeated checks into a keyCodeToDirection function
		if (keyCode[0] == 'w' || strcmp(keyCode, "\e[A") == 0) {
			shotLaser = 1;
			actorShootLaser(playerID, north);
		} else if (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) {
			shotLaser = 1;
			actorShootLaser(playerID, west);
		} else if (keyCode[0] == 's' || strcmp(keyCode, "\e[B") == 0) {
			shotLaser = 1;
			actorShootLaser(playerID, south);
		} else if (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) {
			shotLaser = 1;
			actorShootLaser(playerID, east);
		} else if (keyCode[0] == 'f' || keyCode[0] == '\e') {
			break;
		}
	}
	
	if (shotLaser) {
		computeLaserMap();
		drawScreen();
		sleepMs(200);
	}

	return shotLaser;
}
