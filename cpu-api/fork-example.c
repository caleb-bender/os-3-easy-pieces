#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

#ifdef PROBLEM_ONE

/* 1. Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x? */

// Both the parent and child process have x = 100. When the parent
// or child change x, it changes x inside their memory space because they are distinct processes with their own virtual memory address space.

int main(int argc, char* argv[]) {
	
	int x;
	x = 100;
	int rc = fork();
	if (rc == 0) {
		printf("In child process, x is %d\n", x);
		x = 200;
		printf("In child process, x is now %d\n", x);
	}
	else if (rc > 0) {
		wait(NULL);
		printf("In parent process, x is %d\n", x);
		x = 300;
		printf("In parent process, x is now %d\n", x);
	}
	else {
		printf("Child process did not create.\n");
	}
	return 0;
}
#endif

#ifdef PROBLEM_TWO

/* 2. Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?
*/

// Both the parent and child process can access the file descriptor and both can write to the file.  

int main(int argc, char* argv[]) {
	int fd = open("test.txt", O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
	int rc = fork();
	if (rc == 0) {
		int i = 0;
		while (i < 2) {
			write(fd, "Hello from child!\n", 18);
			++i;
			usleep(1000u);
		}
		exit(0);
	}
	else if (rc > 0) {
		int i = 0;
		while (i < 2) {
			write(fd, "Hello from parent!\n", 19);
			++i;
			usleep(1000u);
		}
	}
	close(fd);
	return 0;
}
#endif
