#ifndef _UI_STUFF_H_
#define _UI_STUFF_H_

#include <ncurses.h>
#include <errno.h>
#include "file-data.h"
#include "say.h"

#define WINDOW_EXIT 0
#define WINDOW_MAIN 1
#define WINDOW_CONFIRM_DELETE 2

char ui_cleared(unsigned int list_width, unsigned int list_height);
void ui_show_instructions(char speak);
char ui_updated(unsigned int list_width, unsigned int list_height, const char *text, unsigned int selection, int key);
void enter_text(int key, unsigned int *cursor, char *text);
void ui_restore_selection(char *path, char *text, int *cursor, int *selection);

char mainScreen(char speak, char *fullPath) {
	char text[PATH_MAX], selectedFile[PATH_MAX];
	memset(text, 0, PATH_MAX);
	unsigned int cursor = 0, list_width = COLS - 4, list_height = LINES - 6;
	int ch, selection = -1;
	WINDOW *path = create_window(0, 0, COLS, 3);
	WINDOW *list = create_window(0, 3, COLS, list_height + 2); // +2 for borders
	ui_show_instructions(speak);
	if (strlen(fullPath) > 0) {
		ui_restore_selection(fullPath, text, &cursor, &selection);
		if (!ui_updated(list_width, list_height, text, selection, ch)) {
			// TO-DO: Add some logging here
			// In the while-loop, this function failing should end the
			// program; here though, it's just a minor inconvenience,
			// and doesn't actually break anything (or should it end?
			// I kind of think the screen not updating until the user
			// presses a key is better than a crash, but that's just me;
			// discussion for another day, when there's more than one
			// geek contributing to this project. :D).
		}
	}
	else move(1, 1);
	
	while(ch = getch()) {
		if (ch == KEY_F(1)) break;
		if (ch == KEY_DC && selection != -1) {
			strcpy(fullPath, text);
			if (fullPath[strlen(fullPath) - 1] != '/')
				strcat(fullPath, "/");
			strcat(fullPath, selectedFile);
			destroy_window(path);
			destroy_window(list);
			return WINDOW_CONFIRM_DELETE;
		}
		if (ch == KEY_UP) {
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
					say(selectedFile, SAY_NOW | SAY_ASYNC);
					break;
				}
				f = f->next;
			}
		}
		else if (ch == KEY_DOWN) {
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
					say(selectedFile, SAY_NOW | SAY_ASYNC);
					break;
				}
				f = f->next;
			}
		}
		else if (ch == KEY_ENTER || ch == 10) {
			strcpy(fullPath, text);
			if (fullPath[strlen(fullPath) - 1] != '/')
				strcat(fullPath, "/");
			strcat(fullPath, selectedFile);
			if (selection == -1) {
				say("Please select a file.", SAY_NOW | SAY_ASYNC);
				continue;
			}
			if (is_folder(fullPath)) {
				strcpy(text, fullPath);
				cursor = strlen(text);
				selection = -1;
			}
			if (is_executable(fullPath)) {
				system(fullPath);
				endwin();
				initscr();
			}
			else {
				char command[PATH_MAX + 10];
				strcpy(command, "nano \"");
				strcat(command, fullPath);
				strcat(command, "\"");
				stop();
				system(command);
				endwin();
				initscr();
			}
		}
		else if (ch > KEY_F(1) && ch <= KEY_F(32)) continue;
		else {
			selection = -1;
			curs_set(1);
			enter_text(ch, &cursor, text);
		}
		if (!ui_updated(list_width, list_height, text, selection, ch)) {
			// TO-DO: replace this with logging.  At this point,
			// this if-statement should never return true, but if
			// it ever does, it should handle this a bit more gracefully.
			// Users should get a nicer error and the error info should
			// just be written to a log file.
			perror("The program crashed with the following info:\n");
			return WINDOW_EXIT;
		}
		move(1, cursor + 1);
		wrefresh(path);
		wrefresh(list);
	}
	destroy_window(path);
	destroy_window(list);
	return WINDOW_EXIT;
}

char confirmDelete(char *fullPath) {
	WINDOW *dialog = create_window(0, 0, COLS, LINES);
	say("Are you sure you want to delete this file?", SAY_NOW | SAY_ASYNC);
	say(fullPath, SAY_ASYNC);
	say("Press Y or N", SAY_ASYNC);
	while(1) {
		mvwprintw(dialog, 1, 1, "Are you sure you want to delete this file?");
		mvwprintw(dialog, 3, 5, "%s", fullPath);
		mvwprintw(dialog, 5, 1, "Press y or n:");
		wrefresh(dialog);
		noecho();
		char ch = wgetch(dialog);
		echo();
		if (ch == 'y') {
			int deleted = delete_file(fullPath);
			if (deleted != 0) {
				mvwprintw(dialog, 8, 5, "%d", deleted);
			}
			say("file deleted", SAY_NOW | SAY_ASYNC);
			break;
		}
		else if (ch == 'n') {
			stop();
			break;
		}
		else {
			mvwprintw(dialog, 7, 1, "Invalid choice.");
			say("Invalid choice.", SAY_NOW | SAY_ASYNC);
		}
	}
	destroy_window(dialog);
	return WINDOW_MAIN;
}

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

void ui_show_instructions(char speak) {
	curs_set(0);
	mvprintw(LINES - 1, 1, "For help press F1.");
	int chars_left = COLS - 21;
	if (chars_left >= 22) {
		mvprintw(LINES - 1, 22, "To exit press F4.");
		chars_left -= 21;
	}
	if (chars_left >= 30) {
		const char *version = "Files version 1.0";
		mvprintw(LINES - 1, COLS - strlen(version), version);
		move(1,1);
		if (speak) {
			say(version, SAY_NOW | SAY_ASYNC);
			say("For help press F1. To exit press F4", SAY_ASYNC);
		}
	}
	else {
		move(1,1);
		if (speak) say("For help press F1. To exit press F4", SAY_NOW | SAY_ASYNC);
	}
	curs_set(1);
}

void ui_string_pad(char *line, char *value, unsigned int list_width) {
	memset(line, 0, list_width);
	int length = strlen(value);
	for (int i=0; i<list_width - 1; i++) {
		if (i >= length) line[i] = ' ';
		else line[i] = value[i];
	}
}

void ui_add2list(char *filename, int y, unsigned int selection, unsigned int ID) {
	move(y, 1);
	if (selection == ID) attron(A_REVERSE);
	printw("%s", filename);
	attroff(A_REVERSE);
}

char ui_updated(unsigned int list_width, unsigned int list_height, const char *text, unsigned int selection, int key) {
	curs_set(0);
	if (!ui_cleared(list_width, list_height)) return 1;
	FileList *files = get_updated_files(text);
	if (files == NULL) {
		mvprintw(1, 1, text);
		curs_set(1);
		return 1;
	}
	unsigned int total = files->ID;
	if (selection == -1) {
		for (int i = 0; i<=total; i++) {
			if (i == list_height) break;
			char line[list_width + 3];
			ui_string_pad(line, files->details->d_name, list_width + 3);
			ui_add2list(line, i + 4, selection, files->ID);
			files = files->next;
		}
	}
	else {
		int pages = total / list_height;
		int page = 0, counter = total - selection;
		while(counter > list_height - 1) {
			page++;
			counter -= list_height;
			for (int i=0; i<list_height; i++) {
				if (files == NULL) break;
				files = files->next;
			}
		}
		// Un-comment this next line to debug
		//mvprintw(0, 0, "selection = %d; total (top ID) = %d; list_height = %d; page %d of %d  ", selection, total, list_height, page, pages);
		for (int i = 0; i<=total; i++) {
			if (files == NULL || i == list_height) break;
			char line[list_width + 3];
			ui_string_pad(line, files->details->d_name, list_width + 3);
			if (line[0] == '.' && line[1] == ' ' && line[2] == ' ')
				continue;
			ui_add2list(line, i + 4, selection, files->ID);
			files = files->next;
		}
	}
    mvprintw(1, 1, text);
    if (selection == -1) curs_set(1);
    return 1;
}

void enter_text(int key, unsigned int *cursor, char *text) {
	if (key == KEY_BACKSPACE) {
		say("backspace", SAY_NOW | SAY_ASYNC);
		if ((*cursor) > 0) (*cursor)--;
		int len = strlen(text);
		for (int i=(*cursor); i<len; i++)
			text[i] = text[i + 1];
		if ((*cursor) == 0) {
			move(1, 1);
		}
		else move(1, (*cursor));
	}
	else if (key == KEY_DC) {
		say("delete", SAY_NOW | SAY_ASYNC);
		int len = strlen(text);
		for (int i=(*cursor); i<len; i++)
			text[i] = text[i + 1];
	}
	else if (key == KEY_LEFT) {
		if ((*cursor) > 0) (*cursor)--;
		if (text[(*cursor)] == 0)
			say("blank", SAY_NOW | SAY_ASYNC);
		else say_key(text[(*cursor)], SAY_NOW | SAY_ASYNC);
	}
	else if (key == KEY_RIGHT) {
		if (((*cursor) != 0 || text[(*cursor)] != 0) && (*cursor) <= strlen(text) - 1)
			(*cursor)++;
		if (text[(*cursor)] == 0)
			say("blank", SAY_NOW | SAY_ASYNC);
		else say_key(text[(*cursor)], SAY_NOW | SAY_ASYNC);
	}
	else if (key == KEY_HOME) {
		(*cursor) = 0;
		say_key(text[(*cursor)], SAY_NOW | SAY_ASYNC);
	}
	else if (key == KEY_END) {
		(*cursor) = strlen(text);
		say_key(text[(*cursor)], SAY_NOW | SAY_ASYNC);
	}
	else {
		say_key(key, SAY_NOW | SAY_ASYNC);
		text[(*cursor)] = key;
		(*cursor)++;
	}
}

void ui_restore_selection(char *path, char *text, int *cursor, int *selection) {
	char fileExists = (!file_exists(path));
	size_t length = strlen(path);
	if (length == 0) return;
	size_t filename_length = 0;
	size_t slash = 0;
	for (size_t i = length; i > 0; i--) {
		if (path[i] == '/') {
			slash = i + 1;
			break;
		}
		filename_length++;
	}
	
	if (filename_length == 0) {
		strcpy(text, path);
		mvprintw(1, 1, text);
		*cursor = strlen(text);
		*selection = -1;
	}
	else {
		char *name = malloc(filename_length);
		if (name == NULL) return;	// TO-DO: Add some logging here
		memset(name, 0, filename_length + 1);
		for (int i=slash; i<length; i++) {
			name[i - slash] = path[i];
			path[i] = 0;
		}
		strcpy(text, path);
		mvprintw(1, 1, text);
		*cursor = strlen(text);
		if (fileExists) return;
				FileList *files = get_updated_files(text);
		if (files == NULL) return;	// TO-DO: Add some logging
		while(files->next != NULL) {
			if (strcmp(name, files->details->d_name) == 0) {
				*selection = files->ID;
				break;
			}
			files = files->next;
		}
		say(name, SAY_ASYNC);
		free(name);
	}
}

#endif
