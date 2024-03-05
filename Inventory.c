#include "project.h"

int item[itemCount][itemProperty]={
	{itemCategoryNone, 0}, {itemCategoryMelee, 2},
    {itemCategoryRanged, 1}, {itemCategoryMelee, 2},
    {itemCategoryMelee,100}};

char itemName[itemCount][itemNameLength] = {
	"None", "Basic Knife", "Basic Laser", "MonsterDebug", "SUPER KNIFE"
};

int playerInventory[inventorySize]={1,0,0,3,2,4};

int getItemDamage(int id) {
	return item[id][itemDamage];
}

int getItemCategory(int id) {
	return item[id][itemCategory];
}

int getMeleeWeapon(int actorID) {
	if (isActorPlayer(actorID)) {
		return playerInventory[slotMelee];
	} else {
		int actorType = actors[actorID][actorTypeID];
		int actorWeapon = actorTypes[actorType][actorTypeWeaponID];
		return getItemCategory(actorWeapon) == itemCategoryMelee ? actorWeapon : itemNone;
	}
}

int getRangedWeapon(int actorID) {
	if (isActorPlayer(actorID)) {
		return playerInventory[slotRanged];
	} else {
		int actorType = actors[actorID][actorTypeID];
		int actorWeapon = actorTypes[actorType][actorTypeWeaponID];
		return getItemCategory(actorWeapon) == itemCategoryRanged ? actorWeapon : itemNone;
	}
}


// This passes back a pointer, make sure its handled properly
char *getInventoryName(int slotNum) {
      return itemName[playerInventory[slotNum]];
}

void manageInventory() {
	int slotType, slotSelected = 0;
	char keyCode[8] = "\0";
	
	do {
		if (keyCode[0] == 'e' && slotSelected > 2) {
			slotType = getItemCategory(playerInventory[slotSelected]);
			if (slotType >= 0) {
        char text[100] = "Equipped "; 
        strcat(text, getInventoryName(slotSelected));
        strcat(text, ".");
        updateSideText(text);
      
				playerInventory[slotType]=playerInventory[slotType]^playerInventory[slotSelected];
				playerInventory[slotSelected]=playerInventory[slotType]^playerInventory[slotSelected];
				playerInventory[slotType]=playerInventory[slotType]^playerInventory[slotSelected];
        
			}
		} else if (keyCode[0] == 'i') {
			return;
		}
		
		drawScreen();
		drawInventory(slotSelected);
	} while (readKey(keyCode, 8) && menuHandleInput(&slotSelected, keyCode, inventorySize, 1));
}
