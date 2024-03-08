#include "project.h"

int playerInventory[inventorySize]={1,0,0,3,2,4};

struct itemTypeData* getMeleeWeaponPtr(int actorID) {
	if (isActorPlayer(actorID)) {
		return &items[playerInventory[slotMelee]];
	} else {
		struct itemTypeData* itemType = actors[actorID].type->weapon;
		return itemType->category == itemCategoryMelee ? itemType : getItemPtr("none");
	}
}

struct itemTypeData getMeleeWeapon(int actorID) {
	return *(getMeleeWeaponPtr(actorID));
}

struct itemTypeData* getRangedWeaponPtr(int actorID) {
	if (isActorPlayer(actorID)) {
		return &items[playerInventory[slotRanged]];
	} else {
		struct itemTypeData* itemType = actors[actorID].type->weapon;
		return itemType->category == itemCategoryRanged ? itemType : getItemPtr("none");
	}
}

struct itemTypeData getRangedWeapon(int actorID) {
	return *(getRangedWeaponPtr(actorID));
}

struct itemTypeData* getUtilItemPtr(int actorID) {
	return &items[playerInventory[slotUtil]];
}

struct itemTypeData getUtilItem(int actorID) {
	return *(getUtilItemPtr(actorID));
}

void manageInventory() {
	int slotType, slotSelected = 0;
	char keyCode[8] = "\0";
	
	do {
		if (keyCode[0] == 'e' && slotSelected > 2) {
			slotType = items[playerInventory[slotSelected]].category;
			if (slotType >= 0) {
        char text[100] = "Equipped "; 
        strcat(text, items[playerInventory[slotSelected]].displayName);
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
