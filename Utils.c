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

int isActorPlayer(int actorID) {
	return actors[actorID][actorTypeID] == 1;
}

int isActorDead(int actorID) {
	return actors[actorID][actorTypeID] == 0;
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


// Computes the number of tiles a laser goes before hitting a wall.
int laserRaycast(int x, int y, int direction) {
	int i, dX, dY;
	directionToXY(direction, &dX, &dY);
	for (i = 0; i < 100; i++) {
		if (isTileSolid(x, y)) break;
		x += dX;
		y += dY;
	}
	return i;
}

int laserRaycastByID(int laserID) {
	return laserRaycast(laserEffects[laserID][laserX],
						laserEffects[laserID][laserY],
						laserEffects[laserID][laserDirection]
	);
}