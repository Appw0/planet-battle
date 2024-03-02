#include "project.h"

int main()
{
    int inventoryMode=0, slot;

    char sideText[]="This is a test level for development while actual maps get created.";
    char topText[]="Test Level"; 
    tempGenArray(map);
    srand(time(NULL));
    tempGenActors(actors);
    
    printf("\n");
    drawScreen(topText, sideText);
    printf("\n");
    
	char c = '\0';
	
	enableRawMode();
	
  do {
     
    if (c=='i') {
      inventoryMode=inventoryMode^1;
      slot=0;
    }
    
    if (inventoryMode==1) {
      
      slot=manageInventory(c, slot);
      drawScreen(topText, sideText);
      drawInventory(slot);
    } else {
      if ((c=='w')||(c=='a')||(c=='s')||(c=='d')||(c=='.')) {
        move(c);
      }
      drawScreen(topText, sideText);
    }
    
    
    
    
    
		//if (iscntrl(c)) {
		//	printf("%d\n", c);
		//} else {
		//	printf("%d ('%c')\n", c, c);
		//}
   
	} while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    
    return 0;
}
