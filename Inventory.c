#include "project.h"

// 
int item[itemCount][itemProperty]={{0,0},
                                   {2,0},
                                   {0,2},
                                   {2,0}};

char itemName[itemCount][itemNameLength] = { "None", "Basic Knife", "Basic Laser", "Monster Weapon - D"};

int playerInventory[inventorySize]={1,0,0};

int getInventoryProperty(int actorNum, int slotNum, int type) {
  int actorType=actors[actorNum][actorTypeID];
  
  if (actorType == 1) { //checks if actor the call is for is the player
    switch (type) {
      case 0: // ID number
        return playerInventory[slotNum];
        break;
      case 1: // first property
        return item[playerInventory[slotNum]][0];
        break;
      case 2: // second property
        return item[playerInventory[slotNum]][1];
        break;
      default:
        break;
    }
  } else {
    int actorWeapon=actors[actorNum][actorWeaponID];
    
    switch (type) {
      case 0: // ID number
        return actorWeapon;
        break;
      case 1: // first property
        return item[actorWeapon][0];
        break;
      case 2: // second property
        return item[actorWeapon][1];
        break;
      default:
        break;
    }
  }
}

// This passes back a pointer, make sure its handled properly
char *getInventoryName(int slotNum) {
      return itemName[playerInventory[slotNum]];
}
