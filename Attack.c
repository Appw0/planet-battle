#include "project.h"

// this will be the most barebones of code

void damageActor(int actorID, int amount) {
	actors[actorID][actorHealth] -= amount;
	if (actors[actorID][actorHealth] < 1) {
		actors[actorID][actorTypeID] = 0;
	}
}

int meleeAttack(int dir, int xpos, int ypos, int actorID) {
	int targetActorID, dX, dY;
	
	directionToXY(dir, &dX, &dY);
	targetActorID = getActorAt(xpos + dX, ypos + dY);
	
	int itemID = getMeleeWeapon(actorID);
	if (itemID != itemNone && targetActorID != -1) {
		damageActor(targetActorID, getItemDamage(itemID));
		return 1;
	}
	return 0;
}

void playerAimLaser() {
	char keyCode[8];
	
	do {
		if (keyCode[0] == 'w' || strcmp(keyCode, "\e[A") == 0) {
		} else if (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) {
			
		} else if (keyCode[0] == 's' || strcmp(keyCode, "\e[B") == 0) {
			
		} else if (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) {
			
		}
	} while (readKey(keyCode, 8));
}

void actorShootLaser(int actorID) {
	//TODO: yeah. the whole thing
}