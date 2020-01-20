#include "nc-windows.h"
#include "say.h"

WINDOW *path, *list;

int main(int argc, char *argv[]) {
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	refresh();
		
	path = create_window(0, 0, COLS, 3);
	list = create_window(0, 3, COLS, LINES - 4);
	mvaddstr(LINES - 1, 0, "Press F1 for a menu.");
	move(1, 1);
	say("What a geek", 0);
	
	getch();
	
	destroy_window(path);
	destroy_window(list);
	endwin();
	return 0;
}
