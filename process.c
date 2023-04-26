#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define MAX_DEPTH 2

void create_processes(int current_depth, int max_depth, pid_t parent_pid) {
    if (current_depth > max_depth) {
        return;
    }

    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) { // child process 1
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
        create_processes(current_depth + 1, max_depth, getpid());
        printf("T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else if ((pid2 = fork()) == 0) { // child process 2
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
        create_processes(current_depth + 1, max_depth, getpid());
        printf("T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else { // parent process
        waitpid(pid1, NULL, 0); // wait for child process 1 to finish
        waitpid(pid2, NULL, 0); // wait for child process 2 to finish
    }
}

int main() {
    printf("Creating process tree...\n");
	clock_t start_time = clock();
    pid_t main_pid = getpid();

    printf("n=0 PID= %d (root)\n", main_pid);
    create_processes(1, MAX_DEPTH, main_pid);
    printf("T[%d, %d]\n", main_pid, main_pid);

	clock_t end_time = clock();
    printf("Process tree creation finished in %f seconds.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    return 0;
}
