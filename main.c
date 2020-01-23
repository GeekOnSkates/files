#include <stdlib.h>
#include "nc-windows.h"
#include "say.h"
#include "file-data.h"

WINDOW *path, *list;
unsigned int cursor, list_width, list_height;
unsigned int max = 50;
char *text;

void look4files() {
	curs_set(0);
	int x, y;
	getyx(stdscr, y, x);
	
	move(5, 1);
	for (int i=0; i<list_height; i++)
		for (int j=0; j<list_width; j++)
			mvaddch(i + 4, j + 3, ' ');
	
	FileList *files = get_updated_files(text, files);
	if (files == NULL) return;
	unsigned char i = 0;
	while(files->next != NULL) {
		move(i + 4, 1);
		printw("%s", files->details->d_name);
		i++;
		if (i == list_height) break;
		files = files->next;
	}
    move(y, x);
    curs_set(1);
}

int main(int argc, char *argv[]) {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	refresh();
	if (!speech_enabled()) {
		// TO-DO: Record espeak saying speech was not able to load (lol)
		return 1;
	}
	list_width = COLS - 4;
	list_height = LINES - 6;
	path = create_window(0, 0, COLS, 3);
	list = create_window(0, 3, COLS, list_height + 2); // +2 for borders
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
			if (cursor == 0) continue;
			if (cursor >= 0) cursor--;
			move(1, cursor);
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
		for (int i=0; i<list_width; i++)
			mvaddch(1, 1 + i, ' ');
		mvprintw(1, 1, text);
		look4files();
		wrefresh(path);
		wrefresh(list);
	}
	
	free(text);
	destroy_window(path);
	destroy_window(list);
	endwin();
	return 0;
}
