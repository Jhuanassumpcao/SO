#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>


void create_processes(int current_depth, int max_depth, pid_t parent_pid) {
    if (current_depth > max_depth) {
        return;
    }

    pid_t pid1;
    if ((pid1 = fork()) == 0) { // child process 1
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
        create_processes(current_depth + 1, max_depth, getpid());
        printf("T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else { // parent process
        waitpid(pid1, NULL, 0); // wait for child process 1 to finish

        pid_t pid2;
        if ((pid2 = fork()) == 0) { // child process 2
            printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
            create_processes(current_depth + 1, max_depth, getpid());
            printf("T[%d, %d]\n", getpid(), parent_pid);
            exit(0);
        } else { // parent process
            waitpid(pid2, NULL, 0); // wait for child process 2 to finish
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <max_depth>\n", argv[0]);
        return 1;
    }

    int max_depth = atoi(argv[1]);
    if (max_depth <= 0) {
        printf("Invalid value for max_depth: %d. \n", max_depth);
        return 1;
    }

    printf("Creating process tree with max_depth=%d...\n", max_depth);
    clock_t start_time = clock();
    pid_t main_pid = getpid();

    printf("n=0 PID= %d (root)\n", main_pid);
    create_processes(1, max_depth, main_pid);
    printf("T[%d, %d]\n", main_pid, main_pid);

    clock_t end_time = clock();
    printf("Process tree creation finished in %f seconds.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    return 0;
}
