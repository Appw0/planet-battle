#include "project.h"

char sideText[330]="This is a test level for development while actual maps get created.\n";
char topText[topTextLength];

int main(int argCount, char* args[]) {
    srand(time(NULL));
	
	if (!loadData()) return 1;
	
	// This function is temporary and should be replaced with loading save data
	setupDefaultPlayer();
	
	if (argCount > 1) {
		if (!loadLevel(args[1])) return 1;
	} else {
		if (!loadLevel("TestLevel")) return 1;
		introMovie();
	}
    
	int playerHadTurn = 0;
	char keyCode[8] = "\0";
	
	enableRawMode();
	computeLaserMap();
	drawScreen();
	
  do {
	  
	 playerHadTurn = 0;
	 if (keyCode[0] == 'i') {
		 manageInventory();
		 drawScreen();
   } else if (keyCode[0] == 'h') {
     drawHelp();
	 } else if (keyCode[0] == '.') {
		 playerHadTurn = 1; // pass time
	 } else if (keyCode[0] == 'f') {
		 playerHadTurn = playerAimLaser();
	 } else if (playerMovement(keyCode)) {
		 playerPickupItems();
		 playerCheckPortals();
		 playerHadTurn = 1;
	 }
	 
	 if (playerHadTurn) {
		 doAI();
		 computeLaserMap();
		 setLasersRendered();
		 drawScreen();
	 }
	 
	 if (playerDied) {
		 printf($lred "\nYou ded\n"); // TODO: actually roll back the game to a saved state
		 break;
	 }
   
	} while (readKey(keyCode, 8));
    
    return 0;
}
