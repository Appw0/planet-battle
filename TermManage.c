// Got help from reading: https://viewsourcecode.org/snaptoken/kilo/02.enteringnewTermSettingsMode.html
// q to exit.

#include "project.h"

struct termios originalTermSettings;
char selectSpacers[4] = "  ><";

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermSettings);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &originalTermSettings);
	atexit(disableRawMode);
	
	struct termios newTermSettings = originalTermSettings;
	// Disables echo and canonical mode (which usually holds input until Enter is pressed)
	newTermSettings.c_lflag &= ~(ECHO | ICANON);
	
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTermSettings);
}

void clearTerm() {
	printf("\e[1;1H\e[2J"); // Clear Screen
}

int readKey(char keyEscape[], int codeLength) {
	memset(keyEscape, '\0', codeLength);
	return read(STDIN_FILENO, keyEscape, codeLength);
}

int menuHandleInput(int* selected, char keyCode[], int optionNum) {
	if (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) {
		*selected = ++*selected > optionNum - 1 ? optionNum - 1 : *selected;
	} else if (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) {
		*selected = --*selected < 0 ? 0 : *selected;
	} else if (keyCode[0] == 10) {
		return 0;
	}
	return 1;
}

int askWithTextMenu(char text[], char options[][32], char moreInfo[][512], int optionNum) {
	int i, selected = 0;
	char keyCode[8];
	
	enableRawMode();
	do {
		clearTerm();
		printf("%s", text);
		for (i = 0; i < optionNum; i++) {
			int selectedSpacer = 2*(i == selected);
			printf("%c %s %c ", selectSpacers[selectedSpacer], options[i], selectSpacers[selectedSpacer + 1]);
		}
		printf("%s\n", moreInfo[selected]);
	} while (readKey(keyCode, 8) && menuHandleInput(&selected, keyCode, optionNum));
}

// TODO: Update/make new function to use item/sprite IDs instead of passing an entire array
int askWithSpriteMenu(char text[], char sprites[][5][6], char moreInfo[][512], int optionNum) {
	int x, y, selected = 0;
	char keyCode[8];
	
	enableRawMode();
	do {
		clearTerm();
		printf("%s", text);
		for (y = 0; y < 5; y++) {
			for (x = 0; x < optionNum; x++) {
				int selectedSpacer = 2*(x == selected);
				printf("%c %s %c", selectSpacers[selectedSpacer], sprites[x][y], selectSpacers[selectedSpacer + 1]);
			}
			printf("\n");
		}
		printf("\n%s\n", moreInfo[selected]);
	} while(readKey(keyCode, 8) && menuHandleInput(&selected, keyCode, optionNum));
}