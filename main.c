#include <stdlib.h>
#include <linux/limits.h>
#include "nc-windows.h"
#include "say.h"
#include "ui-stuff.h"

int main(int argc, char *argv[]) {
	char text[PATH_MAX];
	memset(text, 0, PATH_MAX);
	initscr();
	unsigned int cursor = 0, list_width = COLS - 4, list_height = LINES - 6;
	raw();
	keypad(stdscr, TRUE);
	refresh();
	if (!speech_enabled()) {
		// TO-DO: Record espeak saying speech was not able to load (lol)
		return 1;
	}
	WINDOW *path = create_window(0, 0, COLS, 3);
	WINDOW *list = create_window(0, 3, COLS, list_height + 2); // +2 for borders
	mvaddstr(LINES - 1, 0, "Files 1.0     For help press F1.    To exit press F4.");
	move(1, 1);
	
	int ch;
	int selection = -1;
	while(ch = getch()) {
		if (ch == KEY_F(1)) break;
		if (ch == KEY_BACKSPACE) {
			say("backspace", SAY_NOW | SAY_ASYNC);
			if (cursor > 0) cursor--;
			int len = strlen(text);
			for (int i=cursor; i<len; i++)
				text[i] = text[i + 1];
			if (cursor == 0) {
				move(1, 1);
			}
			else move(1, cursor);
		}
		else if (ch == KEY_DC) {
			say("delete", SAY_NOW | SAY_ASYNC);
			int len = strlen(text);
			for (int i=cursor; i<len; i++)
				text[i] = text[i + 1];
		}
		else if (ch == KEY_LEFT) {
			if (cursor > 0) cursor--;
			if (text[cursor] == 0)
				say("blank", SAY_NOW | SAY_ASYNC);
			else say_key(text[cursor], SAY_NOW | SAY_ASYNC);
		}
		else if (ch == KEY_RIGHT) {
			if ((cursor != 0 || text[cursor] != 0) && cursor <= strlen(text) - 1)
				cursor++;
			if (text[cursor] == 0)
				say("blank", SAY_NOW | SAY_ASYNC);
			else say_key(text[cursor], SAY_NOW | SAY_ASYNC);
		}
		else if (ch == KEY_HOME) {
			cursor = 0;
			say_key(text[cursor], SAY_NOW | SAY_ASYNC);
		}
		else if (ch == KEY_END) {
			cursor = strlen(text);
			say_key(text[cursor], SAY_NOW | SAY_ASYNC);
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
		}
		if (!ui_updated(list_width, list_height, text)) {
			perror("The program crashed with the following info:\n");
			break;
		}
		move(1, cursor + 1);
		wrefresh(path);
		wrefresh(list);
	}
	destroy_window(path);
	destroy_window(list);
	clear();
	endwin();
	return 0;
}
