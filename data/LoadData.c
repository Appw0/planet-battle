#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define mapMaxHeight 64
#define mapMaxWidth 64

#define actorMaxCount 32

#define black 30
#define red 31
#define green 32
#define yellow 33
#define blue 34
#define magenta 35
#define cyan 36
#define lgray 37
#define gray 90
#define lred 91
#define lgreen 92
#define lyellow 93
#define lblue 94
#define lmagenta 95
#define lcyan 96
#define white 97

#define iniMaxKeys 256
#define iniMaxKeyLength 256 // sscanf's need to be updated manually!
#define iniMaxValueLength 256 // sscanf's need to be updated manually!
#define iniMaxNameLength 32 // sscanf's need to be updated manually!
#define iniMaxLineLength 256
#define iniMaxListLength 32

#define tileMaxTypes 32

#define itemMaxTypes 32
#define itemMaxNameLength 17

#define itemCategoryNone -1
#define itemCategoryMelee 0
#define itemCategoryRanged 1
#define itemCategoryUtility 2

#define actorMaxTypes 32
#define actorMaxNameLength 32

#define actorMaxRandomPositions 32

struct position {
	int x, y;
};

struct tileTypeData {
	char name[iniMaxNameLength];
	char tile;
	int color;
	int bgColor;
};

struct itemTypeData {
	char name[iniMaxNameLength];
	char displayName[itemMaxNameLength];
	int category;
	int damage;
};

struct actorTypeData {
	char name[iniMaxNameLength];
	char displayName[actorMaxNameLength];
	char tile;
	int color;
	int bgColor;
	int spawnPoints;
	int health;
	struct itemTypeData* weapon;
};

struct iniEntry {
	char name[iniMaxNameLength];
	char keys[iniMaxKeys][iniMaxKeyLength];
	char values[iniMaxKeys][iniMaxValueLength];
	int numKeys;
};

struct tileTypeData tiles[tileMaxTypes];
int tilesCreated = 0;

struct itemTypeData items[itemMaxTypes];
int itemsCreated = 0;

struct actorTypeData actorTypes[actorMaxTypes];
int actorTypesCreated = 0;

int map[mapMaxHeight][mapMaxWidth];
char tempMap[mapMaxHeight][mapMaxWidth];
int mapHeight = mapMaxHeight;
int mapWidth = mapMaxWidth;

struct itemTypeData* getItemPtr(char name[]) {
	int i;
	for (i = 0; i < itemsCreated; i++) {
		if (strcmp(items[i].name, name) == 0) {
			return &items[i];
		}
	}
	printf("ITEM '%s' NOT FOUND!\n", name);
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
	printf("ACTOR TYPE '%s' NOT FOUND!\n", name);
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
	printf("TILE TYPE '%s' NOT FOUND!\n", name);
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
	printf("TILE TYPE '%s' NOT FOUND!\n", (*tileTypePtr).name);
	return 0;
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

void createTileType(char name[], char tile, int color, int bgColor) {
	if (tilesCreated < tileMaxTypes) {
		strncpy(tiles[tilesCreated].name, name, iniMaxNameLength);
		tiles[tilesCreated].tile = tile;
		tiles[tilesCreated].color = color;
		tiles[tilesCreated].bgColor = bgColor;
		tilesCreated++;
	} else {
		printf("Too many tiles!\n");
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
		printf("Too many items!\n");
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
		printf("Too many actor types!\n");
	}
}

void createPlayer(int x, int y) {
	printf("TODO: Create player at %dx and %dy\n", x, y);
}

void createPortal(int x, int y, char level[]) {
	printf("TODO: Create portal at %dx and %dy going to %s\n", x, y, level);
}

void createActorRandomPos(struct actorTypeData* type, struct position pos[], int posCount) {
	printf("TODO: Create actor at any of %d positions\n", posCount);
}

void createActors(struct actorTypeData* types[], int numTypes, struct position pos[], int posCount) {
	printf("TODO: Create random actors from %d types in %d positions\n", numTypes, posCount);
}

void mapTiles(char placeholder, struct tileTypeData* types[], int numTypes) {
	int x, y;
	for (y = 0; y < mapHeight; y++) {
		for (x = 0; x < mapWidth; x++) {
			if (tempMap[y][x] == placeholder) {
				map[y][x] = getTileTypeIndex(types[rand()%numTypes]);
			}
		}
	}
}

int getAndSwapPlaceholderTiles(char oldPlaceholder, char newPlaceholder, struct position pos[], int maxPosCount) {
	int x, y, i = 0;
	for (y = 0; y < mapHeight; y++) {
		for (x = 0; x < mapWidth; x++) {
			if (tempMap[y][x] == oldPlaceholder) {
				tempMap[y][x] = newPlaceholder;
				if (i < maxPosCount) {
					pos[i].x = x;
					pos[i].y = y;
					i++;
				}
			}
		}
	}
	return i;
}

int getAndSwapPlaceholderTile(char oldPlaceholder, char newPlaceholder, int* x, int* y) {
	struct position pos[1];
	int i = getAndSwapPlaceholderTiles(oldPlaceholder, newPlaceholder, pos, 1);
	(*x) = pos[0].x;
	(*y) = pos[0].y;
	return i;
}

void readInt(struct iniEntry data, int index, char key[], int* value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) sscanf(data.values[index], "%d", value);
}

void readChar(struct iniEntry data, int index, char key[], char* value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) (*value) = data.values[index][0];
}

void readStr(struct iniEntry data, int index, char key[], char dest[], int length) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) strncpy(dest, data.values[index], length);
}

void readColor(struct iniEntry data, int index, char key[], int* value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) (*value) = evalColor(data.values[index]);
}

void readItemCategory(struct iniEntry data, int index, char key[], int* value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) (*value) = evalItemCategory(data.values[index]);
}

void readItemType(struct iniEntry data, int index, char key[], struct itemTypeData** value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) (*value) = getItemPtr(data.values[index]);
}

void readActorType(struct iniEntry data, int index, char key[], struct actorTypeData** value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) (*value) = getActorTypePtr(data.values[index]);
}

int readActorTypes(struct iniEntry data, int index, char key[], struct actorTypeData* types[], int number, int* typesFound) {
	int i = 0;
	if (key == NULL || strcmp(data.keys[index], key) == 0) {
		char* actorName = strtok(data.values[index], " ,");
		while(actorName != NULL && i < number) {
			types[i] = getActorTypePtr(actorName);
			actorName = strtok(NULL, " ,");
			i++;
		}
		(*typesFound) = i;
	}
	return i;
}

void readTileType(struct iniEntry data, int index, char key[], struct tileTypeData** value) {
	if (key == NULL || strcmp(data.keys[index], key) == 0) (*value) = getTileTypePtr(data.values[index]);
}

int readTileTypes(struct iniEntry data, int index, char key[], struct tileTypeData* types[], int number, int* typesFound) {
	int i = 0;
	if (key == NULL || strcmp(data.keys[index], key) == 0) {
		char* tileName = strtok(data.values[index], " ,");
		while(tileName != NULL && i < number) {
			types[i] = getTileTypePtr(tileName);
			tileName = strtok(NULL, " ,");
			i++;
		}
		(*typesFound) = i;
	}
	return i;
}

void readTileINI(struct iniEntry data) {
	char tile = ' ';
	int i, color = black, bgColor = black;
	for (i = 0; i < data.numKeys; i++) {
		readChar(data, i, "tile", &tile);
		readColor(data, i, "color", &color);
		readColor(data, i, "bgColor", &bgColor);
	}
	createTileType(data.name, tile, color, bgColor);
}

void readItemINI(struct iniEntry data) {
	char displayName[itemMaxNameLength] = "???";
	int i, category = itemCategoryNone, damage = 0;
	for (i = 0; i < data.numKeys; i++) {
		readStr(data, i, "name", displayName, itemMaxNameLength);
		readItemCategory(data, i, "category", &category);
		readInt(data, i, "damage", &damage);
	}
	createItemType(data.name, displayName, category, damage);
}

void readActorTypeINI(struct iniEntry data) {
	char displayName[actorMaxNameLength] = "???", tile = ' ';
	int i, color = black, bgColor = black, spawnPoints = 0, health = 0;
	struct itemTypeData* weapon = &items[0];
	for (i = 0; i < data.numKeys; i++) {
		readStr(data, i, "name", displayName, actorMaxNameLength);
		readChar(data, i, "tile", &tile);
		readColor(data, i, "color", &color);
		readColor(data, i, "bgColor", &bgColor);
		readInt(data, i, "spawnPoints", &spawnPoints);
		readInt(data, i, "health", &health);
		readItemType(data, i, "weapon", &weapon);
	}
	createActorType(data.name, displayName, tile, color, bgColor, spawnPoints, health, weapon);
}

void readLevelInfo(struct iniEntry data) {
	char levelName[32] = "LEVEL NAME MISSING"; // This should be swapped for a global
	int i, spawnPoints = 0; // So should this
	for (i = 0; i < data.numKeys; i++) {
		readStr(data, i, "name", levelName, 32);
		readInt(data, i, "spawnPoints", &spawnPoints);
	}
	printf("Level '%s' with %d points\n", levelName, spawnPoints);
}

void readLevelMap(struct iniEntry data) {
	int y, x, mapLargestPossibleWidth;
	
	memset(map, 0, sizeof(map));
	memset(tempMap, 0, sizeof(tempMap));
	mapHeight = data.numKeys > mapMaxHeight ? mapMaxHeight : data.numKeys;
	mapWidth = 0;
	mapLargestPossibleWidth = iniMaxValueLength > mapMaxWidth ? mapMaxWidth : iniMaxValueLength;
	
	for (y = 0; y < mapHeight; y++) {
		for (x = 0; x < mapLargestPossibleWidth; x++) {
			if (data.values[y][x] == 0) {
				mapWidth = mapWidth < x ? x : mapWidth;
				break;
			} else {
				tempMap[y][x] = data.values[y][x];
			}
		}
	}
	
	printf("Level is %dx%d\n", mapHeight, mapWidth);
}

void readLevelActor(struct iniEntry data) {
	int i, posCount = 0;
	char placeholder = 'X', replace = '.';
	struct position pos[actorMaxRandomPositions];
	struct actorTypeData* type;
	
	for (i = 0; i < data.numKeys; i++) {
		readActorType(data, i, "actor", &type);
		readChar(data, i, "leave", &replace);
		readChar(data, i, "where", &placeholder);
	}
	
	posCount = getAndSwapPlaceholderTiles(placeholder, replace, pos, actorMaxRandomPositions);
	createActorRandomPos(type, pos, posCount);
}

void readLevelActors(struct iniEntry data) {
	int i, posCount = 0, readTypes = 0;
	char placeholder = 'X', replace = '.';
	struct position pos[actorMaxRandomPositions];
	
	struct actorTypeData* types[iniMaxListLength];
	memset(types, 0, sizeof(types));
	
	for (i = 0; i < data.numKeys; i++) {
		readActorTypes(data, i, "actors", types, iniMaxListLength, &readTypes);
		readChar(data, i, "leave", &replace);
		readChar(data, i, "where", &placeholder);
	}
	
	posCount = getAndSwapPlaceholderTiles(placeholder, replace, pos, actorMaxRandomPositions);
	createActors(types, readTypes, pos, posCount);
}

void readLevelPlayer(struct iniEntry data) {
	char replace = '.';
	int i, x, y;
	for (i = 0; i < data.numKeys; i++) {
		readChar(data, i, "leave", &replace);
	}
	getAndSwapPlaceholderTile('@', replace, &x, &y);
	createPlayer(x, y);
}

void readLevelPortal(struct iniEntry data) {
	char placeholder = 'X', replace = '.', level[iniMaxNameLength];
	int i, x, y;
	for (i = 0; i < data.numKeys; i++) {
		readStr(data, i, "goto", level, iniMaxNameLength);
		readChar(data, i, "leave", &replace);
		readChar(data, i, "where", &placeholder);
	}
	getAndSwapPlaceholderTile(placeholder, replace, &x, &y);
	createPortal(x, y, level);
}

void readLevelTiles(struct iniEntry data) {
	struct tileTypeData* types[iniMaxListLength];
	int i, tilesFound = 0;
	for (i = 0; i < data.numKeys; i++) {
		memset(types, 0, sizeof(types));
		readTileTypes(data, i, NULL, types, iniMaxListLength, &tilesFound);
		mapTiles(data.keys[i][0], types, tilesFound);
	}
}

void readLevelINI(struct iniEntry data) {
	if (strcmp(data.name, "info") == 0) {
		readLevelInfo(data);
	} else if (strcmp(data.name, "map") == 0) {
		readLevelMap(data);
	} else if (strcmp(data.name, "tiles") == 0) {
		readLevelTiles(data);
	} else if (strcmp(data.name, "actor") == 0) {
		readLevelActor(data);
	} else if (strcmp(data.name, "actorRandom") == 0) {
		readLevelActors(data);
	} else if (strcmp(data.name, "portal") == 0) {
		readLevelPortal(data);
	} else if (strcmp(data.name, "player") == 0) {
		readLevelPlayer(data);
	}
}

void readINI(char fileName[], void(*readData)(struct iniEntry)) {
	struct iniEntry data;
	char line[iniMaxLineLength];
	int readFirstValue = 0;
	
	FILE* fileID = fopen(fileName, "r");
	
	if (fileID == NULL) {
		printf("FILE '%s' NOT FOUND!\n", fileName);
		return;
	}
	
	while (!feof(fileID)) {
		if (fgets(line, iniMaxLineLength, fileID) == NULL) break;
		
		switch(line[0]) {
			case '\r':
			case '\n':
			case ';':
				break;
			case '[':
				if (!readFirstValue) {
					readFirstValue = 1;
				} else {
					readData(data);
				}
				data.numKeys = 0;
				memset(data.keys, '\0', sizeof(data.keys));
				memset(data.values, '\0', sizeof(data.keys));
				if (sscanf(line, "[%32[^]\r\n]", data.name) == 0) {
					printf("INVALID INI NAME! FILE COULD NOT BE READ!\n");
					return;
				};
				break;
			default:
				if (data.numKeys < iniMaxKeys) {
					if (strchr(line, '=') == NULL) {
						sscanf(line, "%256[^\r\n]", data.values[data.numKeys]);
					} else {
						sscanf(line, "%256[^ =] = %256[^\r\n]", data.keys[data.numKeys], data.values[data.numKeys]);
					}
					data.numKeys++;
				} else {
					printf("Too many keys!\n");
				}
		}
	}
	
	if (readFirstValue) {
		readData(data);
	} else {
		printf("NO READABLE DATA IN INI FILE!\n");
	}
	
	fclose(fileID);
}


// Name should never be bigger than iniMaxNameLength
void loadLevel(char name[]) {
	char path[iniMaxNameLength + 16] = "\0"; //= "/data/";
	strcat(path, name);
	strcat(path, ".ini");
	readINI(path, readLevelINI);
}

void loadData() {
	readINI("Tiles.ini", readTileINI);
	readINI("Items.ini", readItemINI);
	readINI("Actors.ini", readActorTypeINI);
}

int main() {
	int i, x, y;
	
	loadData();
	
	printf("Read %d tiles, %d actors and %d items\n\n", tilesCreated, actorTypesCreated, itemsCreated);
	
	loadLevel("TestLevel");
	
	for (y = 0; y < mapHeight; y++) {
		for (x = 0; x < mapWidth; x++) {
			printf("%d", map[y][x]);
		}
		printf("\n");
	}
	
	for (i = 0; i < tilesCreated; i++) {
		printf("%s: '%c' c:%d, bc:%d\n", tiles[i].name, tiles[i].tile, tiles[i].color, tiles[i].bgColor);
	}
	
	printf("\n");
	
	for (i = 0; i < itemsCreated; i++) {
		printf("%s: '%s' c:%d, d:%d\n", items[i].name, items[i].displayName, items[i].category, items[i].damage);
	}
	
	printf("\n");
	
	for (i = 0; i < actorTypesCreated; i++) {
		printf("%s: '%s' '%c' c:%d, bc:%d sp:%d h:%d wp:%s\n", actorTypes[i].name, actorTypes[i].displayName, actorTypes[i].tile, actorTypes[i].color, actorTypes[i].bgColor, actorTypes[i].spawnPoints, actorTypes[i].health, actorTypes[i].weapon->name);
	}
	
	return 0;
}