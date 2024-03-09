#include "project.h"

int levelSpawnPoints = 0;

void createActor(struct actorTypeData* type, struct position pos) {
	if (actorsCreated < actorMaxCount) {
		actors[actorsCreated].type = type;
		actors[actorsCreated].x = pos.x;
		actors[actorsCreated].y = pos.y;
		actors[actorsCreated].moveCooldown = 0;
		actors[actorsCreated].health = type->health;
		actors[actorsCreated].moveCooldown = rand()%(type->slowness + 1); // Adds a little spice!
		actors[actorsCreated].shield = 0;
		actorsCreated++;
	} else {
		printf($lred "Too many actors!\n");
	}
}

void createPlayer(int x, int y) {
	struct position pos = {x, y};
	createActor(getActorTypePtr("player"), pos);
	actors[actorsCreated - 1].health = 10;
	// TODO: implement a better way of setting the player's health
	// It's a bit more difficult than a monster as this health is not cosntant.
}

void createActorRandomPos(struct actorTypeData* type, struct position pos[], int posCount, int ignoreWalkable) {
	int i, posIndex;
	for (i = 0; i < 500; i++) {
		posIndex = rand()%posCount;
		if (!isValidActorID(getActorAt(pos[posIndex])) && (ignoreWalkable || isTileWalkable(pos[posIndex].x, pos[posIndex].y))) {
			createActor(type, pos[posIndex]);
			return;
		}
	}
	printf($lred "Can't spawn actor %s! Tried to many times!\n", type->name);
}

void createActors(struct actorTypeData* types[], int numTypes, struct position pos[], int posCount, int ignoreWalkable) {
	int i;
	for (i = 0; i < actorMaxCount - actorsCreated; i++) {
		struct actorTypeData* randomType = types[rand()%numTypes];
		createActorRandomPos(randomType, pos, posCount, ignoreWalkable);
		levelSpawnPoints -= randomType->spawnPoints;
		if (levelSpawnPoints < 1) {
			break;
		}
	}
}

void createPortal(int x, int y, char level[]) {
	printf("TODO: Create portal at %dx and %dy going to %s\n", x, y, level);
}