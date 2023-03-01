#include "../kernel/types.h"
#include "../kernel/param.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	char c, tc[512], *tcp, *p[argc], **tp;	
	int i;

	if (argc < 2) {
		fprintf(2, "usage: xargs [command] [paramter]...\n"); 
		exit(1);
	}

	if (argc > MAXARG) {
		fprintf(2, "xargs: too many arguments\n"); 
		exit(1);
	}
	
	tp = p;
	for (i = 1; i < argc; i++)
		*tp++ = argv[i];
	*tp = tcp = tc;

	while (read(0, &c, 1) && c) {
		if (c == '\n') {
			*tcp = '\0';
			tcp = tc;
			if (fork() == 0) 
				exec(argv[1], p);
		} else {
			*tcp++ = c;
		}
	}

	wait((void *)0);
	exit(0);
}
