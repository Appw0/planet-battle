#include "project.h"

int main()
{


    char sideText[]="test text to write to the iovefugiesochfehs gfjkajfv negaihfcjeakl fevajknf jeiaklcfnyoiaejside. I can wri23894713098 benwfnv9poweir,8239ncyfieowjte all sorts of stuff over here waijdjiajksadjf";
    char topText[]="Level Name"; 
    tempGenArray(map);
    srand(time(NULL));
    tempGenActors(actors);
    
    printf("\n");
    drawScreen(topText, sideText);
    printf("\n");
    
	char c = '\0';
	
	enableRawMode();
	
  do {
    move(c);
     
		printf("\n");
    drawScreen(topText, sideText);
    printf("\n");
    
    
		//if (iscntrl(c)) {
		//	printf("%d\n", c);
		//} else {
		//	printf("%d ('%c')\n", c, c);
		//}
   
	} while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    
    return 0;
}