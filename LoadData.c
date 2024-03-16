#include "project.h"

struct iniEntry {
	char name[iniMaxNameLength];
	char keys[iniMaxKeys][iniMaxKeyLength];
	char values[iniMaxKeys][iniMaxValueLength];
	int numKeys;
};

char tempMap[mapMaxHeight][mapMaxWidth];

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

int readItemTypes(struct iniEntry data, int index, char key[], struct itemTypeData* types[], int number, int* typesFound) {
	int i = 0;
	if (key == NULL || strcmp(data.keys[index], key) == 0) {
		char* itemName = strtok(data.values[index], " ,");
		while(itemName != NULL && i < number) {
			types[i] = getItemPtr(itemName);
			itemName = strtok(NULL, " ,");
			i++;
		}
		(*typesFound) = i;
	}
	return i;
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
	int i, color = black, bgColor = black, blockLaser = 0, blockMove = 0;
	for (i = 0; i < data.numKeys; i++) {
		readChar(data, i, "tile", &tile);
		readColor(data, i, "color", &color);
		readColor(data, i, "bgColor", &bgColor);
		readInt(data, i, "blockMove", &blockMove);
		readInt(data, i, "blockLaser", &blockLaser);
	}
	createTileType(data.name, tile, color, bgColor, blockMove, blockLaser);
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
	int i, color = black, bgColor = black, spawnPoints = 0, health = 0, slowness = 0;
	struct itemTypeData* weapon = &items[0];
	for (i = 0; i < data.numKeys; i++) {
		readStr(data, i, "name", displayName, actorMaxNameLength);
		readChar(data, i, "tile", &tile);
		readColor(data, i, "color", &color);
		readColor(data, i, "bgColor", &bgColor);
		readInt(data, i, "spawnPoints", &spawnPoints);
		readInt(data, i, "health", &health);
		readInt(data, i, "slowness", &slowness);
		readItemType(data, i, "weapon", &weapon);
	}
	createActorType(data.name, displayName, tile, color, bgColor, spawnPoints, health, slowness, weapon);
}

void readLevelInfo(struct iniEntry data) {
	int i;
	for (i = 0; i < data.numKeys; i++) {
		readStr(data, i, "name", topText, topTextLength);
		readInt(data, i, "spawnPoints", &levelSpawnPoints);
	}
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
	createActorRandomPos(type, pos, posCount, 1);
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
	createActors(types, readTypes, pos, posCount, 1);
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

// TODO: levelItems should not load if the room has been cleared before
void readLevelItem(struct iniEntry data) {
	int i, posCount = 0;
	char placeholder = 'X', replace = '.';
	struct position pos[droppedItemMaxRandomPositions];
	struct itemTypeData* type = &items[0];
	
	for (i = 0; i < data.numKeys; i++) {
		readItemType(data, i, "item", &type);
		readChar(data, i, "leave", &replace);
		readChar(data, i, "where", &placeholder);
	}
	
	posCount = getAndSwapPlaceholderTiles(placeholder, replace, pos, droppedItemMaxRandomPositions);
	createDroppedItemRandomPos(type, pos, posCount);
}

void readLevelItems(struct iniEntry data) {
	int i, posCount = 0, readTypes = 0, minCount = 1, maxCount = 1;
	char placeholder = 'X', replace = '.';
	struct position pos[droppedItemMaxRandomPositions];
	
	struct itemTypeData* types[iniMaxListLength];
	memset(types, 0, sizeof(types));
	
	for (i = 0; i < data.numKeys; i++) {
		readItemTypes(data, i, "items", types, iniMaxListLength, &readTypes);
		readChar(data, i, "leave", &replace);
		readChar(data, i, "where", &placeholder);
		readInt(data, i, "countMin", &minCount);
		readInt(data, i, "countMax", &maxCount);
	}
	
	posCount = getAndSwapPlaceholderTiles(placeholder, replace, pos, droppedItemMaxRandomPositions);
	createDroppedItems(types, readTypes, pos, posCount, maxCount, minCount);
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
	} else if (strcmp(data.name, "item") == 0) {
		readLevelItem(data);
	} else if (strcmp(data.name, "itemRandom") == 0) {
		readLevelItems(data);
	}
}

void readDatapadINI(struct iniEntry data) {
	char text[datapadTextMaxLength] = "\0";
	int i;
	for (i = 0; i < data.numKeys; i++) {
		if (data.values[i][0] == '`') {
			data.values[i][0] = '\n';
		} else if (i < data.numKeys && strlen(data.values[i]) < iniMaxValueLength - 1) {
			strcat(text, "\n");
		}
		
		strncat(text, data.values[i], datapadTextMaxLength - strlen(text) - 1);
	}
	createDatapad(data.name, text);
}

int readINI(char fileName[], void(*readData)(struct iniEntry)) {
	struct iniEntry data;
	char line[iniMaxLineLength];
	int readFirstValue = 0;
	
	FILE* fileID = fopen(fileName, "r");
	
	if (fileID == NULL) {
		printf($lred "FILE '%s' NOT FOUND!\n", fileName);
		return 0;
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
					printf($lred "INVALID INI NAME! FILE COULD NOT BE READ!\n");
					return 0;
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
					printf($lred "Too many keys!\n");
				}
		}
	}
	
	if (readFirstValue) {
		readData(data);
	} else {
		printf($lred "NO READABLE DATA IN INI FILE!\n");
		return 0;
	}
	
	fclose(fileID);
	return 1;
}


// Name should never be bigger than iniMaxNameLength
int loadLevel(char name[]) {
	char path[iniMaxNameLength + 16] = "./data/";
	if (strlen(name) > iniMaxNameLength) {
		printf($lred "Level name '%s' is too long!\n", name);
		return 0;
	}
	strcat(path, name);
	strcat(path, ".ini");
	
	resetDroppedItems();
	resetPortals();
	resetActors();
	return readINI(path, readLevelINI);
}

int loadData() {
	int success = 1;
	success &= readINI("./data/Tiles.ini", readTileINI);
	success &= readINI("./data/Items.ini", readItemINI);
	success &= readINI("./data/Actors.ini", readActorTypeINI);
	success &= readINI("./data/Datapads.ini", readDatapadINI);
	return success;
}