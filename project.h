#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <math.h>

// Screen Width and Height, this should be a global value passed in when this is
// Turned into a function
#define mapw 40
#define maph 16

// Map Properties
#define Barrier 1
#define tileTypes 4
#define east 0
#define north 1
#define west 2
#define south 3

// Actor Properties Things
#define actorCount 16
#define actorProperty 8
#define actorTypes 4

// Actor Property IDs
#define actorTypeID 0
#define actorX 1
#define actorY 2
#define actorMoveCool 3
#define actorWeaponID 4 // Unused
#define actorHealth 5
#define actorShield 6 // ???

// Monster Properties
#define monsterPropertyCount 5
#define monsterTypes 4

#define spawnPoints 0
#define icon 1
#define health 2
#define moveCool 3
#define weaponID 4

// Items 
#define itemCount 5
#define itemProperty 2
#define itemNameLength 17 //16 Chars with 1 for null
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


char *getInventoryName(int slotNum);

extern int map[maph][mapw];
extern int actors[actorCount][actorProperty];
extern int laserEffects[laserCount][laserProperty];

extern int item[itemCount][itemProperty];
extern char itemName[itemCount][itemNameLength];

extern int playerInventory[inventorySize];

extern int tileColors[tileTypes][2];
extern char tileChars[tileTypes];
extern int actorColors[actorTypes][2];
extern char actorChars[actorTypes];

extern int monsterProperty[monsterTypes][monsterPropertyCount];

extern char sideText[];
extern char topText[]; 
