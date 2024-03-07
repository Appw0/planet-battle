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

int getActorAt(int x, int y) {
	int id;
	for (id = 0; id < actorCount; id++) { 
		if (!isActorDead(id) && actors[id][actorX] == x && actors[id][actorY] == y) {
			return id;
		}
	}
	return -1;
}

void getActorPosition(int actorID, int* x, int* y) {
	(*x) = actors[actorID][actorX];
	(*y) = actors[actorID][actorY];
}

void getAdjacentTile(int direction, int* x, int* y) {
	
}

int isActorPlayer(int actorID) {
	return actors[actorID][actorTypeID] == playerTypeID;
}

int isActorDead(int actorID) {
	return actors[actorID][actorTypeID] == deadTypeID;
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

int directionToXY(int direction, int* x, int* y) {
	(*x) = 0;
	(*y) = 0;
	switch (direction) {
		case north:
			(*y) = -1;
			break;
		case east:
			(*x) = 1;
			break;
		case south:
			(*y) = 1;
			break;
		case west:
			(*x) = -1;
			break;
		default:
			printf("\nINVALID DIRECTION!\n");
	}
}


// Computes the number of tiles a laser goes before hitting a wall, and finds all the actors on the way.
int laserRaycast(int x, int y, int direction, int actorHitIDs[], int maxHits) {
	memset(actorHitIDs, -1, maxHits * sizeof(int));
	int actorsHit = 0, i, dX, dY;
	directionToXY(direction, &dX, &dY);
	for (i = 0; i < 100; i++) {
		if (isTileSolid(x, y)) {
			break;
		} else if (actorsHit < maxHits) {
			int actorID = getActorAt(x, y);
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
		printf("Invalid color '%s'\n", color);
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
		printf("Invalid category '%s'\n", category);
		return itemCategoryNone;
	}
}