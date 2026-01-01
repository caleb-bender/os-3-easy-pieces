#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#ifdef PROBLEM_ONE

/* 1. Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x? */

// Both the parent and child process have x = 100. When the parent
// or child change x, it changes x inside their memory space because the
// are distinct processes with their own virtual memory address space.

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

// Both the parent and child process can access the file descriptor and
// both can write to the file.  

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

#ifdef PROBLEM_THREE

/*Write another program using fork(). The child process should
print “hello”; the parent process should print “goodbye”. You should
try to ensure that the child process always prints first; can you do
this without calling wait() in the parent?
*/


// By adding a small delay before printing goodbye in the parent (1 us),
// the scheduler is essentially guaranteed to have the child print first.

int main(int argc, char* argv[]) {

	int rc = fork();
	if (rc == 0) {
		printf("hello\n");
		exit(0);
	}
	else if (rc > 0) {
		usleep(1000);
		printf("goodbye\n");
	}
	return 0;
}
#endif

#ifdef PROBLEM_FOUR

/*
Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including execl(), execle(), execlp(),
execv(), execvp(), and execvpe(). Why do you think there
are so many variants of the same basic call?
*/

// There are likely that many variants because different types of
// programs need flexible calling mechanisms for executing commands.
// For example, a program that
// passes the argv array passed to main() to the command being executed
// only needs to forward just that array. However, if you need to make a
// call to the command inline with a certain number of arguments, or if
// you need to specify custom environment variables (e.g. changing output
// colors), you will need different function variants.

int main(int argc, char* argv[]) {


	if (argc != 2) {
		printf("USAGE: ./fork-example.out <exec function>\n");
		return 1;
	}

	char* const envp[] = {
		"LS_COLORS=fi=00;37;41:ex=01;04;35",
		NULL
	};
	char* const args[] = {
		"-l", "-a", NULL
	};
	char* const args_with_envp[] = {
		"ls", "--color=auto", "-l", "-a", NULL
	};
	int rc = fork();
	if (rc == 0) {

		if (strcmp(argv[1], "execl") == 0) {
			printf("Using execl variant...\n");
			execl("/bin/ls", "-l", "-a", NULL);	
		}
		else if (strcmp(argv[1], "execlp") == 0) {
			printf("Using execlp variant...\n");
			execlp("ls", "-l", "-a", NULL);	
		}
		else if (strcmp(argv[1], "execle") == 0) {
			printf("Using execle variant...\n");
			execle("/bin/ls", "ls", "--color=auto", "-l", "-a", NULL, envp);
		}
		else if (strcmp(argv[1], "execv") == 0) {
			printf("Using execv variant...\n");
			execv("/bin/ls", args);
		}
		else if (strcmp(argv[1], "execvp") == 0) {
			printf("Using execvp variant...\n");
			execvp("ls", args);
		}
		else if (strcmp(argv[1], "execvpe") == 0) {	
			printf("Using execvpe variant...\n");
			execvpe("ls", args_with_envp, envp);
		}
	}
	else {
		wait(NULL);
	}
	return 0;
}

#endif
