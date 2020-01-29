#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

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
    if (stat(path, &buf) < 0) return 0;
    return S_ISDIR(buf.st_mode);
}

char is_executable(const char *path) {
	struct stat buf;
    if (stat(path, &buf) < 0) return 0;
    if ((buf.st_mode & S_IEXEC) != 0) return 1;
    return 0;
}

char file_exists(const char *path) {
	FILE *file = fopen(path, "r");
	// TO-DO: Add some logging - I think this sets "errno" or similar.
    return file != NULL;
}

int delete_file(const char *path) {
	if (is_folder(path)) return rmdir(path) == 0 ? 0 : errno;
	return unlink(path) == 0 ? 0 : errno;
}

#endif
