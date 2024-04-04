#include "project.h"

struct termios originalTermSettings;
char selectSpacers[4] = "  ><";


// Resets the terminal back to its original settings
void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermSettings);
}

// Configures the terminal to accept keyboard input directly
void enableRawMode() {
	tcgetattr(STDIN_FILENO, &originalTermSettings);
	atexit(disableRawMode);
	
	struct termios newTermSettings = originalTermSettings;
	// Disables echo and canonical mode (which usually holds input until Enter is pressed)
	newTermSettings.c_lflag &= ~(ECHO | ICANON);
	
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTermSettings);
}

// Parses int representations of color and effect into an escape sequence
void printColorBgExtras(int color, int bgColor, int extra) {
	printf("\033[%d;%d;%dm", extra, color, bgColor + 10);
}

void printColorBg(int color, int bgColor) {
	printColorBgExtras(color, bgColor, reset);
}

void printColor(int color) {
	printColorBg(color, black);
}

// Sets the color and effect back to defaults
void resetColor() {
	printf("\033[0m");
}

void clearTerm() {
	printf("\e[1;1H\e[2J"); // Clears the screen and moves the cursor back to 1,1
}

// Reads a certain number of stdin characters into keyEscape
// This is the escape sequence of the pressed key
int readKey(char keyEscape[], int codeLength) {
	memset(keyEscape, '\0', codeLength);
	return read(STDIN_FILENO, keyEscape, codeLength);
}

// Handles the keyboard logic of moving the cursor around a menu,
// Represented by a pointer to the currently selected index, and the total number of options
// It is also configured with whether the input should be vertical, and a keyCode to exit the menu
// Returns whether the menu is still being interacted with
int menuHandleInput(int* selected, char keyCode[], int optionNum, int vertical, char exitCode) {
	if (!vertical && (keyCode[0] == 'd' || strcmp(keyCode, "\e[C") == 0) ||
		vertical && (keyCode[0] == 's' || strcmp(keyCode, "\e[B") == 0)) {
		*selected = ++*selected > optionNum - 1 ? optionNum - 1 : *selected;
	} else if (!vertical && (keyCode[0] == 'a' || strcmp(keyCode, "\e[D") == 0) ||
		vertical && (keyCode[0] == 'w' || strcmp(keyCode, "\e[A") == 0)) {
		*selected = --*selected < 0 ? 0 : *selected;
	} else if (keyCode[0] == exitCode) {
		return 0;
	}
	return 1;
}

// Takes over the screen and replaces it with a menu defined by a couple variables
// text: the text to print before the menu
// options: a matrix with the text of each option
// moreInfo: a matrix with more text that will display when an option is hovered over
// optionNum: the number of options
// vertical: whether the menu should be vertical or not
int askWithTextMenu(char text[], char options[][32], char moreInfo[][512], int optionNum, int vertical) {
	int i, selected = 0;
	char keyCode[8] = "\0";
	
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
	} while (readKey(keyCode, 8) && menuHandleInput(&selected, keyCode, optionNum, vertical, '\n'));
}

// TODO: Update/make new function to use item/sprite IDs instead of passing an entire array
// An unused function that can print a menu where the user can select between a number of cool ascii images
int askWithSpriteMenu(char text[], char sprites[][5][6], char moreInfo[][512], int optionNum) {
	int x, y, selected = 0;
	char keyCode[8] = "\0";
	
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
	} while(readKey(keyCode, 8) && menuHandleInput(&selected, keyCode, optionNum, 0, '\n'));
}

// Prints out text dramatically, by pausing between each letter, word, line, and sentence
void printDramatic(char text[]) {
	setbuf(stdout, NULL);
	
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
	
	setvbuf(stdout, NULL, _IOLBF, 1024);
}