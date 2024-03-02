#include "project.h"

// 0 is type: 0 is melee, 1 is ranged, 2 is utility, -1 is none
// 1 is damage?
int item[itemCount][itemProperty]={{-1,0},
                                   {0,2},
                                   {1,2},
                                   {0,2},
                                   {0,100}};

char itemName[itemCount][itemNameLength] = { "None", "Basic Knife", "Basic Laser", "MonsterDebug", "SUPER KNIFE"};

// Pos 0 is melee, Pos 1 is ranged, Pos 3 is utility
int playerInventory[inventorySize]={1,0,0,3,2,4};




int getInventoryProperty(int actorID, int slotNum, int type) {
  int actorType=actors[actorID][actorTypeID];
  
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
    int actorWeapon=actors[actorID][actorWeaponID];
    
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
int slotType;

int manageInventory(char c, int slotSelected) {

  if ((c=='w')||(c=='s')) {
    if (c=='w') {
    	slotSelected--;
   	}
    if (c=='s') {
    	slotSelected++;
   	}
  
    if (slotSelected < 0) slotSelected=inventorySize-1;
    if (slotSelected > inventorySize-1) slotSelected=0;
  
  } else if ((c=='e')&&(slotSelected>2)) {
    slotType=getInventoryProperty(0, slotSelected, 1);
    if (slotType >= 0) {
      playerInventory[slotType]=playerInventory[slotType]^playerInventory[slotSelected];
      playerInventory[slotSelected]=playerInventory[slotType]^playerInventory[slotSelected];
      playerInventory[slotType]=playerInventory[slotType]^playerInventory[slotSelected];
    }
  }
  return slotSelected;
}
