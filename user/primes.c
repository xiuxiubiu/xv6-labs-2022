#include "../kernel/types.h"
#include "user.h"

#define MIN 2
#define MAX 32
#define LEN sizeof(int) * 2


int prime();


int
main()
{
	int i, wfd, buf[2];

	wfd = prime();
	for (i = MIN; i <= MAX; i++) {
		buf[0] = MIN;
		buf[1] = i;
		write(wfd, buf, LEN);
	}

	close(wfd);

	wait((void *)0);
	exit(0);
}

int
prime()
{
	int p[2];
	
	pipe(p);
	
	// child
	if (fork() == 0) {
		close(p[1]);

		int buf[2];
		int dividend = 0;
		int wfd;

		while (read(p[0], buf, LEN) != 0) {

			if (buf[0] == buf[1])
				printf("prime %d\n", buf[0]);
			else {
				if (buf[1] % buf[0] != 0) {
					if (dividend == 0) {
						dividend = buf[1];
						wfd = prime();
					}
					buf[0] = dividend;
					write(wfd, buf, LEN);	
				}
			}
		}

		if (dividend != 0) {
			close(wfd);
		}
		close(p[0]);

		wait((void *)0);
		exit(0);
	}

	close(p[0]);
	return p[1];
}
