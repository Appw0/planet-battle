#include "project.h"

char sideText[1000]="\0";
char topText[topTextLength];

int main(int argCount, char* args[]) {
	srand(time(NULL));
	enableRawMode();
	
	 // If the gamedata fails to load, exit the program with an error code of 1
	if (!loadData()) return 1;
	
	setupDefaultPlayer();
	
	if (argCount > 1) {
		// If an argument has been passed in, interpret it as a level name to load
		if (!loadLevel(args[1])) return 1;
	} else {
		// Otherwise load Level1 and play the intro movie
		if (!loadLevel("Level1")) return 1;
		introMovie();
	}
	
	int playerHadTurn = 0;
	char keyCode[8] = "\0";
	
	// Initialize the laserMap and draw the screen to clear away the intro movie
	computeLaserMap();
	drawScreen();
	
	do {
		playerHadTurn = 0; // Will only be 1 if the input was recognized and should advance time
		
		if (keyCode[0] == 'i') {
			manageInventory(); // Load the inventory menu, this blocks until its closed
			drawScreen(); // Clear it immediately after its closed so things feel responsive
			// Please note that time is NOT advanced here
		} else if (keyCode[0] == 'h') {
			drawHelp(); // Help menu
		} else if (keyCode[0] == '.') {
			playerHadTurn = 1; // Wait around and do nothing by advancing time
		} else if (keyCode[0] == 'f') {
			playerHadTurn = playerAimLaser(); // Laser menu! Blocks just like manageInventory()
		} else if (playerMovement(keyCode)) {
			playerPickupItems(); // The player just moved, so lets vacuum up everthing they walked into
			playerCheckPortals(); // And make sure they don't have anywhere to be
			playerHadTurn = 1;
		}
		
		if (playerHadTurn) {
			doAI();
			computeLaserMap();
			setLasersRendered(); // Tells the laserMap that we want all the lasers to dissapate next turn
			drawScreen();
		}
		
		if (playerDied) {
			printDramatic($lred "\n\n     You are dead.");
			sleepMs(1500);
			printDramatic($white "\n\n     It's " $lblue "cold" $white " and " $lmagenta "clammy" $white " here.\n\n");
			sleepMs(1500);
			break;
		}
		
	} while (readKey(keyCode, 8)); // read in 8 characters of the next keyCode pressed
	
	return 0;
}
