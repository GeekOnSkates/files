#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <dirent.h>

typedef struct _FileList {
	struct dirent *details;
	struct _FileList *next;
	unsigned int ID;
} FileList;

FileList *init_FileList(struct dirent *details, unsigned int id) {
	FileList *fl = malloc(sizeof(FileList));
	fl->details = details;
	fl->ID = id;
	fl->next = NULL;
	return fl;
}

FileList *get_updated_files(const char *text, FileList *head) {
	struct dirent *dir;
	DIR *d = opendir(text);
	if (!d) return NULL;
	
	FileList *temp; int ID = 0;
	while ((dir = readdir(d)) != NULL) {
		temp = init_FileList(dir, ID);
		temp->next = head;
		head = temp;
		ID++;
	}
    closedir(d);
    return head;
}

#endif
