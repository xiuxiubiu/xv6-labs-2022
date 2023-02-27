#include "../kernel/types.h"
#include "./user.h"

int
main()
{

	int p[2];
	char *buf = "0";

	pipe(p);

	if (fork() == 0) {

		read(p[0], buf, 1);
		close(p[0]);
		printf("%d: received ping\n", getpid());

		buf = "0";
		write(p[0], buf, 1);
		close(p[1]);

		exit(0);

	} else {
			
		write(p[1], "0", 1);
		close(p[1]);

		wait((void *)0);

		read(p[0], buf, 1);	
		close(p[0]);

		printf("%d: received pong\n", getpid());
		
	}
	
	exit(0);
	
}
