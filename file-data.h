#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <dirent.h>
#include <sys/stat.h>

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

FileList *get_updated_files(const char *text) {
	struct dirent *dir;
	DIR *d = opendir(text);
	if (!d) return NULL;
	
	FileList *head = NULL;
	FileList *temp; int ID = -1;
	while ((dir = readdir(d)) != NULL) {
		if (dir->d_name[0] == '.' && dir->d_name[1] == '.')
			continue;
		temp = init_FileList(dir, ID);
		temp->next = head;
		head = temp;
		ID++;
	}
    closedir(d);
    return head;
}

char is_folder(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

char is_executable(const char *path) {
	return 0;	// for now
}

#endif
