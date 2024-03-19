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
	if (actorsCreated < actorMaxCount) {
		actors[actorsCreated] = playerCopy;
		actors[actorsCreated].x = x;
		actors[actorsCreated].y = y;
		actorsCreated++;
	} else {
		// This should never happen, but if it somehow does the game will yell at you instead of immediately segfaulting
		printf($lred "Too many actors! Can't create the player!\n");
	}
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

void createActors(struct actorTypeData* types[], int numTypes, struct position pos[], int posCount, int ignoreWalkable, int minCount, int maxCount, int spawnPoints) {
	int i, createNum, createMax = actorMaxCount - actorsCreated;
	int useGlobalPoints = spawnPoints <= -1;
	
	if (minCount > -1 && (minCount == maxCount || maxCount <= -1)) {
		createNum = minCount;
	} else if (maxCount > -1 && (minCount == maxCount || minCount <= -1)) {
		createNum = maxCount;
	} else if (minCount > maxCount) {
		printf($lred "minCount is larger than maxCount!\n");
		return;
	} else if (minCount > -1 && maxCount > -1) {
		createNum = rand()%(maxCount - minCount + 1) + minCount;
	}
	
	createNum = createNum > createMax ? createMax : createNum;
	
	for (i = 0; i < createNum; i++) {
		if (useGlobalPoints && levelSpawnPoints < 1 || !useGlobalPoints && spawnPoints < 1)
			break;
		
		struct actorTypeData* randomType = types[rand()%numTypes];
		createActorRandomPos(randomType, pos, posCount, ignoreWalkable);
		
		if (useGlobalPoints) {
			levelSpawnPoints -= randomType->spawnPoints;
		} else {
			spawnPoints -= randomType->spawnPoints;
		}
	}
}

void createPortal(int x, int y, char level[]) {
	if (portalsCreated < portalMaxCount) {
		strncpy(portals[portalsCreated].level, level, iniMaxNameLength);
		portals[portalsCreated].x = x;
		portals[portalsCreated].y = y;
		portalsCreated++;
	} else {
		printf($lred "Too many portals!\n");
	}
}

void createDroppedItem(struct itemTypeData* type, struct position pos) {
	int id = getNextDroppedItemID();
	if (isValidDroppedItemID(id)) {
		droppedItems[id].type = type;
		droppedItems[id].x = pos.x;
		droppedItems[id].y = pos.y;
	} else {
		printf($lred "Too many dropped items!\n");
	}
}

void createDroppedItemRandomPos(struct itemTypeData* type, struct position pos[], int posCount) {
	createDroppedItem(type, pos[rand()%posCount]);
}

void createDroppedItems(struct itemTypeData* types[], int numTypes, struct position pos[], int posCount, int maxCount, int minCount) {
	int i, amount;
	
	if (maxCount == minCount) {
		amount = maxCount;
	} else {
		amount = rand()%(maxCount - minCount + 1) + minCount;
	}
	
	for (i = 0; i < amount; i++) {
		createDroppedItemRandomPos(types[rand()%numTypes], pos, posCount);
	}
}