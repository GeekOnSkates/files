#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "nc-windows.h"
#include "say.h"
#include "ui-stuff.h"

int main(int argc, const char **argv) {
	if (!speech_enabled()) {
		// TO-DO: Record espeak saying speech was not able to load (lol)
		return WINDOW_EXIT;
	}
	initscr();
	raw();
	keypad(stdscr, TRUE);
	refresh();
	clear();
	char path[PATH_MAX];
	strcpy(path, argv[2]);
	printf("%s\n", path);
	renameFile(path);
	endwin();
	return 0;
}
