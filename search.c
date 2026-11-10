#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc <= 1) {
		perror("needs a char or a word to be searched");
		return 1;
	}

	char *searchString = argv[1];

	struct dirent *entry;
	DIR *dir = opendir(".");
	if (dir == NULL) {
		perror("unable to open directory");
		return 1;
	}

	while ((entry = readdir(dir)) != NULL) {
		find_word(entry, searchString);
	}
	closedir(dir);

	return 0;
}

void find_word(const struct dirent *entry, const char *searchString)
{
	if (entry->d_name[0] == '.') {
		return;
	}
	printf("%s\n", entry->d_name);
	int fd = open(entry->d_name, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return;
	}
	
	FILE *f = fdopen(fd, "r");
	char line[1024];
	int line_n = 1;
	while (fgets(line, sizeof(line), f)) {
		char *p = strstr(line, searchString);
		if (p) {
			printf("%s:%d	%s", entry->d_name, line_n, line);	
		}
		line_n++;
	}
	close(fd);
}