#include "project.h"

struct timespec requested, remaining;


// Sleeps for a certain number of milliseconds by using nanosleep
void sleepMs(int millis) {
	double fractional, integral;
	fractional = modf(millis/1000.0d, &integral);
    requested.tv_sec = integral;
    requested.tv_nsec = fractional * 1000000000;
    
    nanosleep(&requested, &remaining);
}

// A human readable shorthand for testing an item's name
int itemNameIs(struct itemTypeData type, char name[]) {
	return strcmp(type.name, name) == 0;
}

// Returns the droppedItemID of an item at the specified coordinates, or -1 if nothing is found
int getDroppedItemAtXY(int x, int y) {
	int id;
	for (id = 0; id < droppedItemsMaxCount; id++) {
		if (!itemNameIs(*droppedItems[id].type, "none") && droppedItems[id].x == x && droppedItems[id].y == y) {
			return id;
		}
	}
	return -1;
}

// Returns the next free droppedItemID, or -1 if there are too many 
int getNextDroppedItemID() {
	int id;
	for (id = 0; id < droppedItemsMaxCount; id++) {
		if (itemNameIs(*droppedItems[id].type, "none")) {
			return id;
		}
	}
	printf($lred "Too many dropped items!");
	return -1;
}

// Returns the actorID of an actor at the specified coordinates, or -1 if none are there
// Also has a flag for finding dead actors too
int getAnyActorAtXY(int x, int y, int deadAllowed) {
	int id;
	for (id = 0; id < actorsCreated; id++) { 
		if ((deadAllowed | !isActorDead(id)) && actors[id].x == x && actors[id].y == y) {
			return id;
		}
	}
	return -1;
}

// Shorthand if you're only searching for living actors
int getActorAtXY(int x, int y) {
	return getAnyActorAtXY(x, y, 0);
}

// A variant of the above for position structs
int getActorAt(struct position pos) {
	return getActorAtXY(pos.x, pos.y);
}

// The opposite of getActorAtXY, copies the actors x and y into the passed locations
void getActorXY(int actorID, int* x, int* y) {
	(*x) = actors[actorID].x;
	(*y) = actors[actorID].y;
}

// A variant of the above for positions structs
struct position getActorPosition(int actorID) {
	struct position pos;
	getActorXY(actorID, &pos.x, &pos.y);
	return pos;
}

// Returns the player actors actorID. If doesn't find the player, something is very wrong
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

// Human readable shorthand for figuring out of this actor is the player
int isActorPlayer(int actorID) {
	return actors[actorID].type == getActorTypePtr("player");
}

// Human readable shorthand for determining if this actor is dead
int isActorDead(int actorID) {
	return actors[actorID].type == getActorTypePtr("dead");
}

// Returns if this actor is a living NPC
int actorHasAI(int actorID) {
	return !isActorPlayer(actorID) && !isActorDead(actorID);
}

// Useful for checking the output of a function which can return an invalid actor ID to indicate an error
int isValidActorID(int actorID) {
	return actorID > -1 && actorID < actorsCreated;
}

// Similar to the above, but for droppedItemIDs
int isValidDroppedItemID(int id) {
	return id > -1 && id < droppedItemsMaxCount;
}

// Percents are hard
int percent(int num, int outOf) {
  return roundf(((float)num/(float)outOf)*(float)100);
}

// Adds to position structs
struct position posAdd(struct position pos1, struct position pos2) {
	struct position sum = {0, 0};
	sum.x = pos1.x + pos2.x;
	sum.y = pos1.y + pos2.y;
	return sum;
}

// Subtracts two positions structs, as pos1 - pos2
struct position posSubtract(struct position pos1, struct position pos2) {
	pos2.x = -pos2.x;
	pos2.y = -pos2.y;
	return posAdd(pos1, pos2);
}

// Finds the distance between two position structs
float posDistance(struct position pos1, struct position pos2) {
	struct position dif = posSubtract(pos1, pos2);
	return sqrt(pow(dif.x, 2) + pow(dif.y, 2));
}

// Turns a direction int into a position struct representing it as a relative position of unit length
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

// Returns an arrow-like character that points in the specified direction
char directionToChar(int direction) {
	switch (direction) {
		case north:
			return '^';
		case east:
			return '>';
		case south:
			return 'v';
		case west:
			return '<';
		default:
			return '?';
	}
}

// RNG!
int randomDirection() {
	return rand()%numDirections;
}

// Turns a direction int into separate x and y variables
int directionToXY(int direction, int* x, int* y) {
	struct position pos = directionToPos(direction);
	(*x) = pos.x;
	(*y) = pos.y;
}

// Checks to see if things are getting out of hand
int isXYOnMap(int x, int y) {
	return x > 0 && x < mapWidth && y > 0 && y < mapHeight;
}

// A variant of above for position structs
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

// A big amalgam that turns a color string into a color int
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

// A horrible function that turns a category string into a category int
int evalItemCategory(char category[]) {
	if (strcmp(category, "none") == 0) {
		return itemCategoryNone;
	} else if (strcmp(category, "melee") == 0) {
		return itemCategoryMelee;
	} else if (strcmp(category, "ranged") == 0) {
		return itemCategoryRanged;
	} else if (strcmp(category, "utility") == 0) {
		return itemCategoryUtility;
	} else if (strcmp(category, "singleuse") == 0) {
		return itemCategorySingleUse;
	} else {
		printf($lred "Invalid category '%s'\n", category);
		return itemCategoryNone;
	}
}