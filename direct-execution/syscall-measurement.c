#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Usage: ./syscall-measurement.out <number of syscalls>\n");
		exit(1);
	}

	char* count = argv[1];
	struct timeval start_time;
       	struct timeval end_time;
	int syscall_count = atoi(count);
	if (syscall_count == 0) {
		gettimeofday(&start_time, NULL);
		gettimeofday(&end_time, NULL);
	}
	else if (syscall_count < 0) {
		printf("Error: number of syscalls must be 0 or greater\n");
		exit(1);
	}
	else {
		gettimeofday(&start_time, NULL);
		for (int i = 0; i < syscall_count; i++) {
			chmod("./testfile.txt", S_IRUSR);	
		}
		gettimeofday(&end_time, NULL);
	printf("The duration of each system call on average was %ld seconds or %ld microseconds.\n", (end_time.tv_sec - start_time.tv_sec) / syscall_count, (end_time.tv_usec - start_time.tv_usec) / syscall_count);
	}
	printf("The duration of %d system calls was %ld seconds or %ld microseconds.\n", syscall_count, (end_time.tv_sec - start_time.tv_sec), (end_time.tv_usec - start_time.tv_usec));
	return 0;
}
