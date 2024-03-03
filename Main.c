#include "project.h"

char sideText[]="This is a test level for development while actual maps get created.";
char topText[]="Test Level"; 

int main()
{
	int playerHadTurn = 0;
    
    tempGenArray(map);
    srand(time(NULL));
    spawnEnemies(10,4);
    
	char keyCode[8];
	
	enableRawMode();
	computeLaserMap();
	drawScreen(topText, sideText);
	
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
		 playerAimLaser();
   } else if (playerMovement(keyCode)) {
		 playerHadTurn = 1;
	 }
	 
	 if (playerHadTurn) {
		 moveAI();
		 computeLaserMap();
		 setLasersRendered();
		 drawScreen();
	 }
   
	} while (readKey(keyCode, 8));
    
    return 0;
}
