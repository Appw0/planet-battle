#include "project.h"

struct actorData actors[actorMaxCount];
int actorsCreated = 0;
int map[mapMaxHeight][mapMaxWidth];
int mapHeight = mapMaxHeight;
int mapWidth = mapMaxWidth;
struct portalData portals[portalMaxCount];
int portalsCreated = 0;
struct droppedItemData droppedItems[droppedItemsMaxCount];

struct tileTypeData tiles[tileMaxTypes];
int tilesCreated = 0;

struct itemTypeData items[itemMaxTypes];
int itemsCreated = 0;

struct actorTypeData actorTypes[actorMaxTypes];
int actorTypesCreated = 0;

struct datapadData datapads[datapadMaxCount];
int datapadsCreated = 0;

struct actorData playerCopy;
int playerDied = 0;

void setupDefaultPlayer() {
	playerCopy.type = getActorTypePtr("player");
	playerCopy.health = 10;
}

// Reset the actors, but save a copy of the player for later
void resetActors() {
	int i;
	for (i = 0; i < actorsCreated; i++) {
		if (isActorPlayer(i)) {
			playerCopy = actors[i];
		}
		actors[i].type = getActorTypePtr("dead");
	}
	actorsCreated = 0;
}

void resetPortals() {
	portalsCreated = 0;
}

void resetDroppedItems() {
	struct itemTypeData* noneType = getItemPtr("none");
	int id;
	for (id = 0; id < droppedItemsMaxCount; id++) {
		droppedItems[id].type = noneType;
	}
}

struct datapadData* getDatapadPtr(char name[]) {
	int i;
	for (i = 0; i < datapadsCreated; i++) {
		if (strcmp(datapads[i].name, name) == 0) {
			return &datapads[i];
		}
	}
	printf($lred "DATAPAD '%s' NOT FOUND!\n", name);
	return &datapads[0];
}

struct datapadData getDatapad(char name[]) {
	return *getDatapadPtr(name);
}

struct itemTypeData* getItemPtr(char name[]) {
	int i;
	for (i = 0; i < itemsCreated; i++) {
		if (strcmp(items[i].name, name) == 0) {
			return &items[i];
		}
	}
	printf($lred "ITEM '%s' NOT FOUND!\n", name);
	return &items[0];
}

struct itemTypeData getItem(char name[]) {
	return *getItemPtr(name);
}

struct actorTypeData* getActorTypePtr(char name[]) {
	int i;
	for (i = 0; i < actorTypesCreated; i++) {
		if (strcmp(actorTypes[i].name, name) == 0) {
			return &actorTypes[i];
		}
	}
	printf($lred "ACTOR TYPE '%s' NOT FOUND!\n", name);
	return &actorTypes[0];
}

struct actorTypeData getActorType(char name[]) {
	return *getActorTypePtr(name);
}

struct tileTypeData* getTileTypePtr(char name[]) {
	int i;
	for (i = 0; i < tilesCreated; i++) {
		if (strcmp(tiles[i].name, name) == 0) {
			return &tiles[i];
		}
	}
	printf($lred "TILE TYPE '%s' NOT FOUND!\n", name);
	return &tiles[0];
}

struct tileTypeData getTileType(char name[]) {
	return *getTileTypePtr(name);
}

int getTileTypeIndex(struct tileTypeData* tileTypePtr) {
	int i;
	for (i = 0; i < tilesCreated; i++) {
		if (&tiles[i] == tileTypePtr) {
			return i;
		}
	}
	printf($lred "TILE TYPE '%s' NOT FOUND!\n", (*tileTypePtr).name);
	return 0;
}

void createTileType(char name[], char tile, int color, int bgColor, int blockMove, int blockLaser) {
	if (tilesCreated < tileMaxTypes) {
		strncpy(tiles[tilesCreated].name, name, iniMaxNameLength);
		tiles[tilesCreated].tile = tile;
		tiles[tilesCreated].color = color;
		tiles[tilesCreated].bgColor = bgColor;
		tiles[tilesCreated].blockMove = blockMove;
		tiles[tilesCreated].blockLaser = blockLaser;
		tilesCreated++;
	} else {
		printf($lred "Too many tiles!\n");
	}
}

void createItemType(char name[], char displayName[], int category, int damage) {
	if (itemsCreated < itemMaxTypes) {
		strncpy(items[itemsCreated].name, name, iniMaxNameLength);
		strncpy(items[itemsCreated].displayName, displayName, itemMaxNameLength);
		items[itemsCreated].category = category;
		items[itemsCreated].damage = damage;
		itemsCreated++;
	} else {
		printf($lred "Too many items!\n");
	}
}

void createActorType(char name[], char displayName[], char tile, int color, int bgColor, int spawnPoints, int health, int slowness, struct itemTypeData* weapon) {
	if (actorTypesCreated < actorMaxTypes) {
		strncpy(actorTypes[actorTypesCreated].name, name, iniMaxNameLength);
		strncpy(actorTypes[actorTypesCreated].displayName, displayName, actorMaxNameLength);
		actorTypes[actorTypesCreated].tile = tile;
		actorTypes[actorTypesCreated].color = color;
		actorTypes[actorTypesCreated].bgColor = bgColor;
		actorTypes[actorTypesCreated].spawnPoints = spawnPoints;
		actorTypes[actorTypesCreated].health = health;
		actorTypes[actorTypesCreated].slowness = slowness;
		actorTypes[actorTypesCreated].weapon = weapon;
		actorTypesCreated++;
	} else {
		printf($lred "Too many actor types!\n");
	}
}

void createDatapad(char name[], char text[]) {
	if (datapadsCreated < datapadMaxCount && itemsCreated < itemMaxTypes) {
		strncpy(datapads[datapadsCreated].name, name, iniMaxNameLength);
		strncpy(datapads[datapadsCreated].text, text, datapadTextMaxLength);
		createItemType(name, "Datapad", itemCategoryDatapad, 0);
		datapadsCreated++;
	} else if (datapadsCreated < datapadMaxCount) {
		printf($lred "Can't create any more datapads because there are too many items!\n");
	} else {
		printf($lred "Too many datapads!\n");
	}
}