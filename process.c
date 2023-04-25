#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define MAX_DEPTH 4

void create_processes(int current_depth, int max_depth, pid_t parent_pid) {
    if (current_depth > max_depth) {
        return;
    }

    pid_t pid;
    if ((pid = fork()) == 0) { // child process
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), getppid());
        create_processes(current_depth + 1, max_depth, getpid());
        printf("T[%d, %d]\n", getpid(), getppid());
        exit(0);
    } else if (pid < 0) { // error
        printf("Error creating process\n");
        exit(1);
    } else { // parent process
        waitpid(pid, NULL, 0); // wait for child process to finish
    }
}

int main() {
    printf("Creating process tree...\n");

    clock_t start_time = clock();
    pid_t main_pid = getpid();
    create_processes(1, MAX_DEPTH, main_pid);
    wait(NULL); // wait for all child processes to finish
    clock_t end_time = clock();

    printf("Process tree creation finished in %f seconds.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    return 0;
}
