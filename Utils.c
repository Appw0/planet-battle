#include "project.h"

struct timespec requested, remaining;

void sleepMs(int millis) {
	double fractional, integral;
	fractional = modf(millis/1000.0d, &integral);
    requested.tv_sec = integral;
    requested.tv_nsec = fractional * 1000000000;
	//printf("\n%ld:%ld-", requested.tv_sec, requested.tv_nsec);
    
    nanosleep(&requested, &remaining);
}

int itemNameIs(struct itemTypeData type, char name[]) {
	return strcmp(type.name, name) == 0;
}

int getAnyActorAtXY(int x, int y, int deadAllowed) {
	int id;
	for (id = 0; id < actorsCreated; id++) { 
		if ((deadAllowed | !isActorDead(id)) && actors[id].x == x && actors[id].y == y) {
			return id;
		}
	}
	return -1;
}

int getActorAtXY(int x, int y) {
	return getAnyActorAtXY(x, y, 0);
}

int getActorAt(struct position pos) {
	return getActorAtXY(pos.x, pos.y);
}

void getActorXY(int actorID, int* x, int* y) {
	(*x) = actors[actorID].x;
	(*y) = actors[actorID].y;
}

struct position getActorPosition(int actorID) {
	struct position pos;
	getActorXY(actorID, &pos.x, &pos.y);
	return pos;
}

int getPlayerID() {
	int i;
	for (i = 0; i < actorsCreated; i++) {
		if (isActorPlayer(i)) {
			return i;
		}
	}
	printf($lred "PLAYER ACTOR DOES NOT EXIST!!!\n");
	return 0;
}

int isActorPlayer(int actorID) {
	return actors[actorID].type == getActorTypePtr("player");
}

int isActorDead(int actorID) {
	return actors[actorID].type == getActorTypePtr("dead");
}

int actorHasAI(int actorID) {
	return !isActorPlayer(actorID) && !isActorDead(actorID);
}

int isValidActorID(int actorID) {
	return actorID > -1;
}

int percent(int num, int outOf) {
  return roundf(((float)num/(float)outOf)*(float)100);
}

struct position directionToPos(int direction) {
	struct position pos;
	pos.x = 0;
	pos.y = 0;
	switch (direction) {
		case north:
			pos.y = -1;
			break;
		case east:
			pos.x = 1;
			break;
		case south:
			pos.y = 1;
			break;
		case west:
			pos.x = -1;
			break;
		default:
			printf($lred "INVALID DIRECTION!\n");
	}
	return pos;
}

int randomDirection() {
	return rand()%numDirections;
}

int directionToXY(int direction, int* x, int* y) {
	struct position pos = directionToPos(direction);
	(*x) = pos.x;
	(*y) = pos.y;
}

int isXYOnMap(int x, int y) {
	return x > 0 && x < mapWidth && y > 0 && y < mapHeight;
}

int isPosOnMap(struct position pos) {
	return isXYOnMap(pos.x, pos.y);
}

// Computes the number of tiles a laser goes before hitting a wall, and finds all the actors on the way.
int laserRaycast(int x, int y, int direction, int actorHitIDs[], int maxHits) {
	memset(actorHitIDs, -1, maxHits * sizeof(int));
	int actorsHit = 0, i, dX, dY;
	directionToXY(direction, &dX, &dY);
	for (i = 0; i < 100; i++) {
		if (!isXYOnMap(x, y) || tileBlocksLasers(x, y)) {
			break;
		} else if (actorsHit < maxHits) {
			int actorID = getActorAtXY(x, y);
			if (isValidActorID(actorID)) {
				actorHitIDs[actorsHit] = actorID;
				actorsHit++;
			}
		}
		x += dX;
		y += dY;
	}
	return i;
}

int evalColor(char color[]) {
	if (strcmp(color, "black") == 0) {
		return black;
	} else if (strcmp(color, "red") == 0) {
		return red;
	} else if (strcmp(color, "green") == 0) {
		return green;
	} else if (strcmp(color, "yellow") == 0) {
		return yellow;
	} else if (strcmp(color, "blue") == 0) {
		return blue;
	} else if (strcmp(color, "magenta") == 0) {
		return magenta;
	} else if (strcmp(color, "cyan") == 0) {
		return cyan;
	} else if (strcmp(color, "lgray") == 0) {
		return lgray;
	} else if (strcmp(color, "gray") == 0) {
		return gray;
	} else if (strcmp(color, "lred") == 0) {
		return lred;
	} else if (strcmp(color, "lgreen") == 0) {
		return lgreen;
	} else if (strcmp(color, "lyellow") == 0) {
		return lyellow;
	} else if (strcmp(color, "lblue") == 0) {
		return lblue;
	} else if (strcmp(color, "lmagenta") == 0) {
		return lmagenta;
	} else if (strcmp(color, "lcyan") == 0) {
		return lcyan;
	} else if (strcmp(color, "white") == 0) {
		return white;
	} else {
		printf($lred "Invalid color '%s'\n", color);
		return black;
	}
}

int evalItemCategory(char category[]) {
	if (strcmp(category, "none") == 0) {
		return itemCategoryNone;
	} else if (strcmp(category, "melee") == 0) {
		return itemCategoryMelee;
	} else if (strcmp(category, "ranged") == 0) {
		return itemCategoryRanged;
	} else if (strcmp(category, "utility") == 0) {
		return itemCategoryUtility;
	} else {
		printf($lred "Invalid category '%s'\n", category);
		return itemCategoryNone;
	}
}