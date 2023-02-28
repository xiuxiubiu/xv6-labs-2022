#include "../kernel/types.h"
#include "./user.h"
#include "../kernel/fcntl.h"
#include "../kernel/fs.h"
#include "../kernel/stat.h"

void find(char *, char *);

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(2, "usage: find [path] [name]\n");
		exit(0);
	}

	find(argv[1], argv[2]);

	exit(0);
}

void
find(char *path, char *name)
{
	int fd;
	char *p, *tp, buf[512];
	struct stat st;
	struct dirent de;
	

	if ((fd = open(path, O_RDONLY)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);	
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		return;
	}


	switch (st.type) {

		case T_DIR:
			if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
				printf("find: path too long\n");
				break;
			}

			p = buf;
			tp = path;
			while (*tp) 
				*p++ = *tp++;
			*p++ = '/';

			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (de.inum == 0)
					continue;

				if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
					continue;
				
				memcpy(p, de.name, sizeof(de.name));
				
				find(buf, name);	
			}
			break;
		
		case T_DEVICE:
		case T_FILE:
			tp = path + strlen(path);
			p = name;

			while (*tp != '/') 
				tp--;
			tp++;
			
			while (*p && *tp && *p++ == *tp++)
				if (!*p)
					printf("%s\n", path);

			break;
	}

	close(fd);

	return;
}
