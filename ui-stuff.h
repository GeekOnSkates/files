#ifndef _UI_STUFF_H_
#define _UI_STUFF_H_

#include <ncurses.h>
#include "file-data.h"

char ui_cleared(unsigned int list_width, unsigned int list_height) {
	char *spaces = malloc(list_width + 1);
	if (spaces == NULL) return 0;
	memset(spaces, ' ', list_width);
	mvprintw(1, 1, spaces);
	for (int i=0; i<list_height; i++)
		mvprintw(i + 4, 1, spaces);
	free(spaces);
	return 1;
}

void ui_show_instructions(unsigned int list_width) {
	mvprintw(LINES - 1, 0, "For help press F1.");
	int chars_left = list_width - 20;
	if (chars_left >= 22)
		mvprintw(LINES - 1, 22, "To exit press F4.");
}

char ui_updated(unsigned int list_width, unsigned int list_height, const char *text) {
	curs_set(0);
	int x, y;
	getyx(stdscr, y, x);
	if (!ui_cleared(list_width, list_height)) return 1;
	
	FileList *files = get_updated_files(text);
	if (files == NULL) {
		mvprintw(1, 1, text);
		curs_set(1);
		return 1;
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
    return 1;
}

#endif