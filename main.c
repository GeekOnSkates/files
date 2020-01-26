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
	
	int ch, selection = -1;
	while(ch = getch()) {
		if (ch == KEY_F(1)) break;
		else if (ch == KEY_DOWN) {
			FileList *f = get_updated_files(text);
			if (f == NULL) continue;
			while(f->next != NULL) {
				say(f->details->d_name, 0);
				f = f->next;
			}
		}
		else enter_text(ch, &cursor, text);
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
