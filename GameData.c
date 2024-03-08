#include "project.h"

struct actorData actors[actorMaxCount];
int actorsCreated = 0;
int map[mapMaxHeight][mapMaxWidth];
int mapHeight = mapMaxHeight;
int mapWidth = mapMaxWidth;

struct tileTypeData tiles[tileMaxTypes];
int tilesCreated = 0;

struct itemTypeData items[itemMaxTypes];
int itemsCreated = 0;

struct actorTypeData actorTypes[actorMaxTypes];
int actorTypesCreated = 0;

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

void createActorType(char name[], char displayName[], char tile, int color, int bgColor, int spawnPoints, int health, struct itemTypeData* weapon) {
	if (actorTypesCreated < actorMaxTypes) {
		strncpy(actorTypes[actorTypesCreated].name, name, iniMaxNameLength);
		strncpy(actorTypes[actorTypesCreated].displayName, displayName, actorMaxNameLength);
		actorTypes[actorTypesCreated].tile = tile;
		actorTypes[actorTypesCreated].color = color;
		actorTypes[actorTypesCreated].bgColor = bgColor;
		actorTypes[actorTypesCreated].spawnPoints = spawnPoints;
		actorTypes[actorTypesCreated].health = health;
		actorTypes[actorTypesCreated].weapon = weapon;
		actorTypesCreated++;
	} else {
		printf($lred "Too many actor types!\n");
	}
}