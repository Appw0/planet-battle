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

void printColorBg(int color, int bgColor) {
	printf("\033[%d;%dm", color, bgColor + 10);
}

void printColor(int color) {
	printColorBg(color, black);
}

void resetColor() {
	printf("\033[0m");
}

void clearTerm() {
	printf("\e[1;1H\e[2J"); // Clear Screen
}

int readKey(char keyEscape[], int codeLength) {
	memset(keyEscape, '\0', codeLength);
	return read(STDIN_FILENO, keyEscape, codeLength);
}

int menuHandleInput(int* selected, char keyCode[], int optionNum, int vertical) {
	if (!vertical && (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) ||
		vertical && (keyCode[0] == 's' || strcmp(keyCode, "\e[B") == 0)) {
		*selected = ++*selected > optionNum - 1 ? optionNum - 1 : *selected;
	} else if (!vertical && (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) ||
		vertical && (keyCode[0] == 'w' || strcmp(keyCode, "\e[A") == 0)) {
		*selected = --*selected < 0 ? 0 : *selected;
	} else if (keyCode[0] == 10) {
		return 0;
	}
	return 1;
}

int askWithTextMenu(char text[], char options[][32], char moreInfo[][512], int optionNum, int vertical) {
	int i, selected = 0;
	char keyCode[8];
	
	enableRawMode();
	do {
		clearTerm();
		printf("%s", text);
		// for (i = 0; i < optionNum; i++) {
			// int selectedSpacer = 2*(i == selected);
			// printf("%c %s %c ", selectSpacers[selectedSpacer], options[i], selectSpacers[selectedSpacer + 1]);
		// }
		// printf("%s\n", moreInfo[selected]);
		
		for (i = 0; i < optionNum; i++) {
			int selectedSpacer = 2*(i == selected);
			printf("%c %s %c ", selectSpacers[selectedSpacer], options[i], selectSpacers[selectedSpacer + 1]);
			if (selectedSpacer > 0 && vertical) printf("%s", moreInfo[selected]);
			if (vertical) printf("\n");
		}
		if (!vertical) printf("%s", moreInfo[selected]);
	} while (readKey(keyCode, 8) && menuHandleInput(&selected, keyCode, optionNum, vertical));
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
	} while(readKey(keyCode, 8) && menuHandleInput(&selected, keyCode, optionNum, 0));
}