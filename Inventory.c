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

int addToInventory(struct itemTypeData* type) {
	char text[64];
	int i, id = type - items; // this is cursed, but is a result of playerInventory not containing pointers :(
	// TODO: make playerInventory contain pointers...
	
	for (i = 3; i < inventorySize; i++) { // This is also bad, we shouldn't randomly start at 3 to skip special slots, they should be separate
		if (playerInventory[i] == 0) { // Again, this is bad and i hate it. Magic numbers bad.
			playerInventory[i] = id;
			snprintf(text, 64, $lyellow "Picked up a %s.\n", type->displayName);
			updateSideText(text);
			return 1;
		}
	}
	return 0; // Your inventory was full.
}

void playerPickupItems() {
	struct position pos = getActorPosition(getPlayerID());
	int droppedItemID = getDroppedItemAtXY(pos.x, pos.y);
	
	while (isValidDroppedItemID(droppedItemID)) {
		if (addToInventory(droppedItems[droppedItemID].type)) {
			droppedItems[droppedItemID].type = getItemPtr("none");
			droppedItemID = getDroppedItemAtXY(pos.x, pos.y);
		} else {
			updateSideText($yellow "Inventory full!\n");
			break;
		}
	}
}


// TODO: This code does not function with new itemCategories, it should be overhauled along with playerInventory
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
