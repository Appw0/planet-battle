// Got help from reading: https://viewsourcecode.org/snaptoken/kilo/02.enteringnewTermSettingsMode.html
// Use 'a' and 'd' to schmoove, and q to exit.
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

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
	int x, y, lilGuyX = 0, lilGuyY = 0;
	char c = '\0';
	
	enableRawMode();
	
	do {
		if (c == 'd') {
			lilGuyX++;
		} else if (c == 'a') {
			lilGuyX = lilGuyX > 0 ? --lilGuyX: lilGuyX;
		} else if (c == 's') {
			lilGuyY++;
		} else if (c == 'w') {
			lilGuyY = lilGuyY > 0 ? --lilGuyY: lilGuyY;
		}
		printf("\x1b[2J"); // Clear screen
		printf("\x1b[H");
		
		for (y = 0; y < lilGuyY; y++) {
			printf("\n");
		}
		for (x = 0; x < lilGuyX; x++) {
			printf(" ");
		}
		printf("@\n");
		
		if (iscntrl(c)) {
			printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
	} while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return 0;
}