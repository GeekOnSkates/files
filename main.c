#include <stdlib.h>
#include "nc-windows.h"
#include "say.h"
#include "file-data.h"

WINDOW *path, *list;
unsigned int cursor, list_width, list_height;
unsigned int max = 50;
char *text;

void updateList() {
	curs_set(0);
	int x, y;
	getyx(stdscr, y, x);
	
	char *spaces = malloc(list_width + 1);
	if (spaces == NULL) exit(123);	// for now
	memset(spaces, ' ', list_width);
	mvprintw(1, 1, spaces);
	for (int i=0; i<list_height; i++)
		mvprintw(i + 4, 1, spaces);
	free(spaces);
	
	FileList *files = get_updated_files(text);
	if (files == NULL) {
		mvprintw(1, 1, text);
		curs_set(1);
		return;
	}
	unsigned char i = 0;
	while(files->next != NULL) {
		if (files->details->d_name[0] == '.'
			&& files->details->d_name[1] == '.') {
				files = files->next;
				continue;
			}
		move(i + 4, 1);
		printw("%s", files->details->d_name);
		files = files->next;
		i++;
	}
    mvprintw(1, 1, text);
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
	mvaddstr(LINES - 1, 0, "Files 1.0     For help press F1.    To exit press F4.");
	move(1, 1);
	
	text = malloc(50);
	if (text == NULL) {
		perror("Error starting program: malloc failed.\n");
		return 1;
	}
	
	int ch;
	int selection = -1;
	while(ch = getch()) {
		if (ch == KEY_F(1)) break;
		if (ch == KEY_BACKSPACE) {
			say("backspace", SAY_NOW | SAY_ASYNC);
			if (text[cursor] == 0) cursor--;
			text[cursor] = 0;
			if (cursor == 0) {
				printw(" ");
				move(1,1);
				continue;
			}
			if (cursor >= 0) cursor--;
			move(1, cursor);
		}
		else if (ch == KEY_DOWN) {
			FileList *f = get_updated_files(text);
			if (f == NULL) continue;
			while(f->next != NULL) {
				say(f->details->d_name, 0);
				f = f->next;
			}
		}
		else {
			say_key(ch, SAY_NOW | SAY_ASYNC);
			text[cursor] = ch;
			cursor++;
			if (cursor == max) {
				text = realloc(text, max + 10);
				max += 10;
			}
		}
		updateList();
		wrefresh(path);
		wrefresh(list);
	}
	
	free(text);
	destroy_window(path);
	destroy_window(list);
	endwin();
	return 0;
}
