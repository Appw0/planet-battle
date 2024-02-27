// Got help from reading: https://viewsourcecode.org/snaptoken/kilo/02.enteringnewTermSettingsMode.html
// Use 'a' and 'd' to schmoove, and q to exit.
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct termios originalTermSettings;

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

int main() {
	int lineLength, i, x, y, selected = 0;
	char c = '\0';
	char itemSprites[][5][6] = {
		{
			"     ",
			" \\ / ",
			" |O| ",
			" / \\ ",
			"     "
		},
		{
			"     ",
			" [J=/",
			" [%&]",
			" [&%]",
			" \\--/"
		},
		{
			"     ",
			" >==R",
			"   []",
			"     ",
			"     "
		}
	};
	
	char itemNames[][32] = {
		"Fusion Mine",
		"Plasma Clip",
		"T77 Point-pistol"
	};
	char itemDescriptions[][512] = {
		"An old, fusion-based anti-personnel mine. Extremely outdated and questionably operable. Who knows what model it is. These things were so common once that entire an field of engineering sprouted up to find these specific models deep beneath layers of soil and sand.",
		"Standard issue plasma clip. Filled with enough deuterium to ignite a stellerator worthy of a starhopper. But you're not going to use it for that are you?",
		"The most basic of point-pistols. Balanced well with little recoil. It can blast pinholes straight through solid, 3 inch steel. Unfortunately, it's so ubiquitous that just about any idiot's personal shield can deflect it with ease."
	};
	char keyEscape[8];
	char spacers[4] = "  <>";
	char descriptionLine[64];
	
	enableRawMode();
	
	do {
		if (c == 27) {
			memset(keyEscape, '\0', 8);
			read(STDIN_FILENO, &keyEscape, 8);
			printf(":: %s\n", keyEscape);
			
			if (strcmp(keyEscape, "[C") == 0) {
				selected = ++selected > 2 ? 2 : selected;
			} else if (strcmp(keyEscape, "[D") == 0) {
				selected = --selected < 0 ? 0 : selected;
			}
		} else if (iscntrl(c)) {
			printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
		
		printf("\x1b[2J"); // Clear screen
		printf("\x1b[H");
		
		printf("\n\n   ▀█▀ █▄█ ██▀   ▄▀▀ █▄█ ▄▀▄ █▀▄ █▀▄ ██▀\n    █  █ █ █▄▄   ▄██ █ █ ▀▄▀ █▀  █▀  █▄▄\n\n");
		
		for (y = 0; y < 5; y++) {
			printf("     ");
			for (x = 0; x < 3; x++) {
				int selectedSpacer = 2*(x == selected);
				printf("%c %s %c  ", spacers[selectedSpacer], itemSprites[x][y], spacers[selectedSpacer + 1]);
			}
			printf("\n");
		}
		
		printf("\n           -- %s --\n", itemNames[selected]);
		
		lineLength = 0;
		for (i = 0; i < strlen(itemDescriptions[selected]); i++) {
			lineLength++;
			printf("%c", itemDescriptions[selected][i]);
			if (lineLength > 50 && itemDescriptions[selected][i] == ' ') {
				printf("\n");
				lineLength = 0;
			}
		}
		printf("\n");
	} while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return 0;
}