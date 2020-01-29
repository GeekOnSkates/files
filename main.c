#include <stdlib.h>
#include <linux/limits.h>
#include "nc-windows.h"
#include "say.h"
#include "ui-stuff.h"

int main(int argc, char *argv[]) {
	if (!speech_enabled()) {
		// TO-DO: Record espeak saying speech was not able to load (lol)
		return WINDOW_EXIT;
	}
	initscr();
	raw();
	keypad(stdscr, TRUE);
	refresh();
	
	char fullPath[PATH_MAX];
	memset(fullPath, 0, PATH_MAX);
	
	char instructionsSpoken = 0;
	char activeWindow = WINDOW_MAIN;
	while(1) {
		if (activeWindow == WINDOW_EXIT) break;
		if (activeWindow == WINDOW_MAIN) {
			if (!instructionsSpoken) {
				activeWindow = mainScreen(1, fullPath);
				instructionsSpoken = 1;
			}
			else activeWindow = mainScreen(0, fullPath);
		}
		else if (activeWindow == WINDOW_CONFIRM_DELETE)
			activeWindow = confirmDelete(fullPath);
		else if (activeWindow == WINDOW_RENAME_FILE)
			activeWindow = renameFile(fullPath);
	}
	
	clear();
	endwin();
	if (argc > 1) printf("%s", fullPath);
	return 0;
}
