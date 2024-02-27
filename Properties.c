#include "project.h"

// actorCount, can go as high as needed (zero id monsters do nothing)
int actors[actorCount][actorProperty]={{1,10,5,0,100,75}};

// This can be cleaned up and turned into an array and moved into its own
// seperate file for multi function use
int GetTileProperties(int type) {
    switch(type) {
        case 0: // Void Tile
            return 32;
            break;
        case 1: // Wall Tile
            printf("\033[0;33m");
            return 35;
            break;
        case 2: // Floor Tile
            printf("\033[0;32m");
            return 46;
            break;
        case 3: // Misc
            printf("\033[0;36m");
            return 46;
            break;
        default:
            return 32;
    }
}

// This can be cleaned up and turned into an array and moved into its own
// seperate file for multi function use
int GetActorProperties(int type) {
    switch(type) {
        case 0: // Dead
            return 0;
            break;
        case 1: // Player Character
            printf("\033[0;33m");
            return 64;
            break;
        case 2: // Generic Monster
            printf("\033[0;31m");
            return 88;
            break;
        case 3: // Generic Monster 2
            printf("\033[0;31m");
            return 111;
            break;
        default:
            return 0;
    }
}