#include "project.h"

int levelSpawnPoints = 0;

// void spawnEnemies(int totalPoints, int maxCount) {
  // int xguess, yguess, chooseType, i, valid;
  // int spawnpointTotal=totalPoints;
  
  // for(i=1; i<maxCount+1; i++) {
    // do {
      // xguess=rand()%(mapw)+1;
      // yguess=rand()%(maph)+1;
      // valid = map[yguess][xguess] > Barrier ? 1 : 0;
    // } while(valid != 1);  
    
    // chooseType = rand()%(actorTypeCount-2)+2;
    // spawnpointTotal -= actorTypes[chooseType][actorTypeSpawnPoints];
    
    // actors[i][actorTypeID]=chooseType;
    // actors[i][actorX]=xguess;
    // actors[i][actorY]=yguess;
    // actors[i][actorMoveCool]=actorTypes[chooseType][actorTypeMoveCool];
    // actors[i][actorHealth]=actorTypes[chooseType][actorTypeHealth];
    
    // if (spawnpointTotal<=0) break;
     
  // }
// }

void createActor(struct actorTypeData* type, struct position pos) {
	if (actorsCreated < actorMaxCount) {
		actors[actorsCreated].type = type;
		actors[actorsCreated].x = pos.x;
		actors[actorsCreated].y = pos.y;
		actors[actorsCreated].moveCooldown = 0;
		actors[actorsCreated].health = type->health;
		actors[actorsCreated].shield = 0;
		actorsCreated++;
	} else {
		printf($lred "Too many actors!\n");
	}
}

void createPlayer(int x, int y) {
	struct position pos = posFromXY(x, y);
	createActor(getActorTypePtr("player"), pos);
	actors[actorsCreated - 1].health = 10;
	// TODO: implement a better way of setting the player's health
	// It's a bit more difficult than a monster as this health is not cosntant.
}

void createActorRandomPos(struct actorTypeData* type, struct position pos[], int posCount, int ignoreWalkable) {
	int i, posIndex;
	for (i = 0; i < 500; i++) {
		posIndex = rand()%posCount;
		if (!isValidActorID(getActorAtXY()) && (ignoreWalkable || isTileWalkable(pos[posIndex].x, pos[posIndex].y))) {
			createActor(type, pos[posIndex]);
			return;
		}
	}
	printf("Can't spawn actor %s! Tried to many times!\n", type->name);
}

void createActors(struct actorTypeData* types[], int numTypes, struct position pos[], int posCount, int ignoreWalkable) {
	int i;
	for (i = 0; i < actorMaxCount - actorsCreated; i++) {
		struct actorTypeData* randomType = types[rand()%numTypes];
		createActorRandomPos(randomType, pos, posCount, 1);
		levelSpawnPoints -= randomType->spawnPoints;
		if (levelSpawnPoints < 1) {
			break;
		}
	}
}

void createPortal(int x, int y, char level[]) {
	printf("TODO: Create portal at %dx and %dy going to %s\n", x, y, level);
}