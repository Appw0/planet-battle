// Terminal escapes from https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <math.h>

// Map
#define numDirections 4
#define east 0
#define north 1
#define west 2
#define south 3
#define tileMaxTypes 32
#define mapMaxHeight 64
#define mapMaxWidth 64
#define portalMaxCount 8

// Actors
#define actorMaxCount 32
#define actorMaxTypes 32
#define actorMaxNameLength 32
#define actorMaxRandomPositions 32

// Items
#define itemMaxTypes 32
#define itemMaxNameLength 17
#define itemCategoryNone -1
#define itemCategoryMelee 0
#define itemCategoryRanged 1
#define itemCategoryUtility 2

//Inventory
#define slotMelee 0
#define slotRanged 1
#define slotUtil 2
#define inventorySize 13 //10 slots plus 3 equipped

// Lasers
#define laserCount 16
#define laserProperty 8
#define laserNeedsRender 0
#define laserChar 1
#define laserColor 2
#define laserBgColor 3
#define laserX 4
#define laserY 5
#define laserDirection 6
#define laserDistance 7

// INI Constants
#define iniMaxKeys 256
#define iniMaxKeyLength 256 // sscanf's need to be updated manually!
#define iniMaxValueLength 256 // sscanf's need to be updated manually!
#define iniMaxNameLength 32 // sscanf's need to be updated manually!
#define iniMaxLineLength 256
#define iniMaxListLength 32

// Colors
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

#define $black "\033[30m"
#define $red "\033[31m"
#define $green "\033[32m"
#define $yellow "\033[33m"
#define $blue "\033[34m"
#define $magenta "\033[35m"
#define $cyan "\033[36m"
#define $lgray "\033[37m"
#define $gray "\033[90m"
#define $lred "\033[91m"
#define $lgreen "\033[92m"
#define $lyellow "\033[93m"
#define $lblue "\033[94m"
#define $lmagenta "\033[95m"
#define $lcyan "\033[96m"
#define $white "\033[97m"

// Non-Color Escapes Supported by MobaXTerm
#define reset 0
#define bold 1
#define faint 2
#define underline 4
#define blink 5
#define crossedout 9

struct position {
	int x, y;
};

struct tileTypeData {
	char name[iniMaxNameLength];
	char tile;
	int color;
	int bgColor;
	int blockMove;
	int blockLaser;
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
	int slowness;
	struct itemTypeData* weapon;
};

struct actorData {
	struct actorTypeData* type;
	int x, y;
	int moveCooldown;
	int health;
	int shield;
};

struct portalData {
	char level[iniMaxKeyLength];
	int x, y;
};

extern int laserEffects[laserCount][laserProperty];

extern struct tileTypeData tiles[tileMaxTypes];
extern int tilesCreated;

extern struct itemTypeData items[itemMaxTypes];
extern int itemsCreated;

extern struct actorTypeData actorTypes[actorMaxTypes];
extern int actorTypesCreated;

extern char sideText[330];
#define topTextLength 32
extern char topText[topTextLength];

// Utils.c
struct position posFromXY(int x, int y);

// Inventory.c
struct itemTypeData* getMeleeWeaponPtr(int actorID);
struct itemTypeData getMeleeWeapon(int actorID);
struct itemTypeData* getRangedWeaponPtr(int actorID);
struct itemTypeData getRangedWeapon(int actorID);
struct itemTypeData* getUtilItemPtr(int actorID);
struct itemTypeData getUtilItem(int actorID);
extern int playerInventory[inventorySize];

// Spawner.c
extern int levelSpawnPoints;
void createPlayer(int x, int y) ;
void createPortal(int x, int y, char level[]);
void createActorRandomPos(struct actorTypeData* type, struct position pos[], int posCount, int ignoreWalkable);
void createActors(struct actorTypeData* types[], int numTypes, struct position pos[], int posCount, int ignoreWalkable);

// GameData.c
struct itemTypeData* getItemPtr(char name[]);
struct itemTypeData getItem(char name[]);
struct actorTypeData* getActorTypePtr(char name[]);
struct actorTypeData getActorType(char name[]);
struct tileTypeData* getTileTypePtr(char name[]);
struct tileTypeData getTileType(char name[]);
int getTileTypeIndex(struct tileTypeData* tileTypePtr);
void createActorType(char name[], char displayName[], char tile, int color, int bgColor, int spawnPoints, int health, int slowness, struct itemTypeData* weapon);


// GameData.c Map Data
extern int map[mapMaxHeight][mapMaxWidth];
extern int mapHeight;
extern int mapWidth;

extern struct actorData actors[actorMaxCount];
extern int actorsCreated;

extern struct portalData portals[portalMaxCount];
extern int portalsCreated;

extern struct actorData playerCopy;
extern int playerDied;