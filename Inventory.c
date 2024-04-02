#include "project.h"

struct itemTypeData* playerInventory[inventorySize];
struct itemTypeData* playerEquipped[equipmentSlots];
char equipmentSlotNames[equipmentSlots][8] = {"Melee", "Ranged", "Utility"};

struct itemTypeData* getInventoryItemPtr(int inventorySlot) {
	return playerInventory[inventorySlot];
}

struct itemTypeData getInventoryItem(int inventorySlot) {
	return *(getInventoryItemPtr(inventorySlot));
}

struct itemTypeData* getEquippedItemPtr(int equipmentSlot) {
	return playerEquipped[equipmentSlot];
}

struct itemTypeData getEquippedItem(int equipmentSlot) {
	return *(getEquippedItemPtr(equipmentSlot));
}

struct itemTypeData* getMeleeWeaponPtr(int actorID) {
	if (isActorPlayer(actorID)) {
		return playerEquipped[slotMelee];
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
		return playerEquipped[slotRanged];
	} else {
		struct itemTypeData* itemType = actors[actorID].type->weapon;
		return itemType->category == itemCategoryRanged ? itemType : getItemPtr("none");
	}
}

struct itemTypeData getRangedWeapon(int actorID) {
	return *(getRangedWeaponPtr(actorID));
}

struct itemTypeData* getUtilItemPtr(int actorID) {
	return playerEquipped[slotUtil];
}

struct itemTypeData getUtilItem(int actorID) {
	return *(getUtilItemPtr(actorID));
}

void clearInventory() {
	struct itemTypeData* noneItem = getItemPtr("none");
	int i;
	
	for (i = 0; i < inventorySize; i++) {
		playerInventory[i] = noneItem;
	}
	for (i = 0; i < equipmentSlots; i++) {
		playerEquipped[i] = noneItem;
	}
}

int addToInventory(struct itemTypeData* type) {
	int i;
	
	for (i = 0; i < inventorySize; i++) {
		if (itemNameIs(*playerInventory[i], "none")) {
			playerInventory[i] = type;
			return 1;
		}
	}
	return 0; // Your inventory was full.
}

void playerPickupItems() {
	struct position pos = getActorPosition(getPlayerID());
	int droppedItemID = getDroppedItemAtXY(pos.x, pos.y);
	char text[64];
	
	while (isValidDroppedItemID(droppedItemID)) {
		if (addToInventory(droppedItems[droppedItemID].type)) {
			snprintf(text, 64, $lyellow "Picked up a %s.\n", droppedItems[droppedItemID].type->displayName);
			updateSideText(text);
			
			droppedItems[droppedItemID].type = getItemPtr("none");
			droppedItemID = getDroppedItemAtXY(pos.x, pos.y);
		} else {
			updateSideText($yellow "Inventory full!\n");
			break;
		}
	}
}

void manageInventory() {
	int slotSelected = 0;
	char keyCode[8] = "\0";
	char text[100];
	
	do {
		struct itemTypeData* selectedItem = slotSelected < equipmentSlots ? playerEquipped[slotSelected] : playerInventory[slotSelected - equipmentSlots];
		
		if ((keyCode[0] == '\n' || keyCode[0] == 'e') && slotSelected >= equipmentSlots) {
			switch (selectedItem->category) {
				case itemCategoryDatapad:
					puts($lcyan "\nThis datapad reads:");
					puts(getDatapadPtr(selectedItem->name)->text);
					continue; // Skip clearing the screen
				case itemCategoryMelee:
				case itemCategoryRanged:
				case itemCategoryUtility:
					snprintf(text, 100, $lyellow "Equipped %s.\n", selectedItem->displayName);
					updateSideText(text);
					
					// swap things
					// This code assumes that the category maps directlty to a slot type
					playerInventory[slotSelected - equipmentSlots] = playerEquipped[selectedItem->category];
					playerEquipped[selectedItem->category] = selectedItem;
					break;
				default:
					break;
			}
		} else if (strcmp(keyCode, "\e") == 0) {
			return;
		}
		
		drawScreen();
		drawInventory(slotSelected);
	} while (readKey(keyCode, 8) && menuHandleInput(&slotSelected, keyCode, inventorySize + equipmentSlots, 1, 'i'));
}