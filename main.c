#include <stdlib.h>
#include <linux/limits.h>
#include "nc-windows.h"
#include "say.h"
#include "ui-stuff.h"

int main(int argc, char *argv[]) {
	char text[PATH_MAX], selectedFile[PATH_MAX];
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
	ui_show_instructions();
	move(1, 1);
	
	int ch, selection = -1;
	while(ch = getch()) {
		if (ch == KEY_F(1)) break;
		if (ch == KEY_UP) {
			char found = 0;
			FileList *f = get_updated_files(text);
			if (f == NULL) {
				say("No files found", SAY_NOW | SAY_ASYNC);
				continue;
			}
			if (selection >= f->ID)
				selection = 0;
			else selection++;
			while(f->next != NULL) {
				if (f->ID == selection) {
					strcpy(selectedFile, f->details->d_name);
					say(f->details->d_name, SAY_NOW | SAY_ASYNC);
					break;
				}
				f = f->next;
			}
		}
		else if (ch == KEY_DOWN) {
			char found = 0;
			FileList *f = get_updated_files(text);
			if (f == NULL) {
				say("No files found", SAY_NOW | SAY_ASYNC);
				continue;
			}
			if (selection <= 0)
				selection = f->ID;
			else selection--;
			while(f->next != NULL) {
				if (f->ID == selection) {
					strcpy(selectedFile, f->details->d_name);
					say(f->details->d_name, SAY_NOW | SAY_ASYNC);
					break;
				}
				f = f->next;
			}
		}
		else {
			selection = -1;
			enter_text(ch, &cursor, text);
		}
		if (!ui_updated(list_width, list_height, text)) {
			perror("The program crashed with the following info:\n");
			break;
		}
		mvprintw(0, 0, "%d  ", selection);
		mvprintw(0, 10, "\"%s\"       ", selectedFile);
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
