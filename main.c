#include <stdlib.h>
#include "nc-windows.h"
#include "say.h"

WINDOW *path, *list;
unsigned int cursor;
unsigned int max = 50;
char *text;

int main(int argc, char *argv[]) {
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	refresh();
		
	path = create_window(0, 0, COLS, 3);
	list = create_window(0, 3, COLS, LINES - 4);
	mvaddstr(LINES - 1, 0, "Press F1 for a menu.");
	move(1, 1);
	
	text = malloc(50);
	if (text == NULL) {
		perror("Error starting program: malloc failed.\n");
		return 1;
	}
	
	int ch;
	while(ch = getch()) {
		if (ch == KEY_F(1)) break;
		if (ch == KEY_BACKSPACE) {
			say("backspace", SAY_NOW | SAY_ASYNC);
			text[cursor] = 0;
			if (cursor == 0) {
				move(1, 1);
			}
			else {
				cursor--;
			}
		}
		else {
			text[cursor] = ch;
			cursor++;
			if (cursor == max) {
				text = realloc(text, max + 10);
				max += 10;
			}
		}
		move(1, 1);
		for (int i=0; i<COLS - 4; i++)
			mvaddch(1, 1 + i, ' ');
		mvprintw(1, 1, text);
		wrefresh(path);
	}
	
	free(text);
	destroy_window(path);
	destroy_window(list);
	endwin();
	return 0;
}
