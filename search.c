#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

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
		if (entry->d_name[0] == '.') {
			continue;
		}
		int fd = open(entry->d_name, O_RDONLY);
		if (fd < 0) {
			perror("open");
			return 1;
		}
		
		FILE *f = fdopen(fd, "r");
		char line[1024];
		int line_n = 1;
		printf("filename: %s\n", entry->d_name);
		while (fgets(line, sizeof(line), f)) {
			printf("%d %s", line_n, line);
			line_n++;
		}
		printf("\n");
		close(fd);
	}
	closedir(dir);

	return 0;
}
