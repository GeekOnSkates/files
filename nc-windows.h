/**
 * Header:		nc-windows.h
 * Purpose:		To give developers like me some better TUI functions
 * Author:		The Geek on Skates
 * License:		The Un-License
 */
#ifndef _NC_WINDOWS_H_
#define _NC_WINDOWS_H_

#include <ncurses.h>

/**
 * Creates a window
 * @param The window's horizontal ("X") potiion/coordinate
 * @param The window's vertical ("Y") potiion/coordinate
 * @param The window's width
 * @param The window's height
 * @remarks All parameters are in characters.
 * NOTE: I know this is a bit different from the usual (and IMO counter-
 * intuitive) way ncurses does things.  This is intentional.  Ncurses is
 * from a time when developers were forced to use cryptic shorthand like
 * initscr(), endwin(), attron() mvprintw() etc. to save bits.  But in
 * the 21st Century, it's possible for us to write code that is clear,
 * where you can tell what it's doing without a bunch of obscure letters
 * thrown together in whatever way made sense in 1985.  Seriously though
 * I've never seen a single tutorial that didn't add a pile of comments
 * after every line to explain what the puck the code is doing.
 */
WINDOW *create_window(int x, int y, int width, int height) {
	WINDOW *win;
	win = newwin(height, width, y, x);
	box(win, 0 , 0);
	wrefresh(win);
	return win;
}

/**
 * Destroys a window
 * @param Pointer to the window to destroy
 */
void destroy_window(WINDOW *win) {
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(win);
	delwin(win);
}

#endif
