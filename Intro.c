#include "project.h"

void printDramatic(char text[]) {
	int i;
	for (i = 0; i < strlen(text); i++) {
		int millis, doPrint = 1;
		
		switch (text[i]) {
			case '@':
				millis = 2000;
				doPrint = 0;
				break;
			case '!':
			case '.':
			case '?':
			case ',':
			case '\n':
				millis = 200;
				break;
			case ' ':
				millis = 80;
				break;
			default:
				millis = 20;
		}
		
		if (doPrint) printf("%c", text[i]);
		
		sleepMs(millis);
	}
}

void introMovie() {
	int line;
	char introText[] = {
		"\n\n"
		"    A short time ago, in a galaxy not that far away, a rouge planet drifted in the void...\n"
		"    And underneath its black clouds floated an ancient station whose purpose had been lost to time.\n"
		"    But as the millennia have passed, few have ever ventured here, and few have ever bothered,\n"
		"    for none have made it back alive.\n\n"
		"    As you land your leaky starship on its rotting hull, gales of force whistling around,\n"
		"    and plasma cutter buzzing with anticipation, you begin to wonder...\n\n"
		"    'Will I be the next to die?'\n\n"
		"    "
	};
	
	clearTerm();
	setbuf(stdout, NULL);
	
	printDramatic(introText);
	sleepMs(2000);
	
	char opts[][32] = {"Yes", "No"};
	char moreinf[2][512] = {"\0", "\0"};
	askWithTextMenu(introText, opts, moreinf, 2, 0);
	
	setvbuf(stdout, NULL, _IOLBF, 1024);
}