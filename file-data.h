#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <dirent.h>

typedef struct _FileList {
	struct dirent *details;
	struct _FileList *next;
	struct _FileList *previous;
} FileList;

FileList *init_FileList(struct dirent *details) {
	FileList *fl = malloc(sizeof(FileList));
	fl->details = details;
	fl->next = NULL;
	fl->previous = NULL;
	return fl;
}

FileList *get_updated_files(const char *text, FileList *head) {
	struct dirent *dir;
	DIR *d = opendir(text);
	if (!d) return NULL;
	
	FileList *temp;
	while ((dir = readdir(d)) != NULL) {
		temp = init_FileList(dir);
		temp->next = head;
		head = temp;
	}
    closedir(d);
    return head;
}

#endif
