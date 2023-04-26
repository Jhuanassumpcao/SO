#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TREE_HEIGHT 4

void create_tree(int height, pid_t parent_pid) {
    if (height == 0) {
        return;
    }

    pid_t left_pid, right_pid;

    if ((left_pid = fork()) == 0) { // left child process
        printf("n=%d C[%d, %d]\n", TREE_HEIGHT - height + 1, getpid(), parent_pid);
        create_tree(height - 1, getpid());
        printf("T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else { // parent process
        waitpid(left_pid, NULL, 0); // wait for left child process to finish
    }

    if ((right_pid = fork()) == 0) { // right child process
        printf("n=%d C[%d, %d]\n", TREE_HEIGHT - height + 1, getpid(), parent_pid);
        create_tree(height - 1, getpid());
        printf("T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else { // parent process
        waitpid(right_pid, NULL, 0); // wait for right child process to finish
    }
}

int main() {
    printf("Creating process tree...\n");

    pid_t main_pid = getpid();
    printf("n=%d C[%d, %d]\n", TREE_HEIGHT, main_pid, main_pid);
    create_tree(TREE_HEIGHT, main_pid);
    printf("T[%d, %d]\n", main_pid, main_pid);

    printf("Process tree creation finished.\n");
    return 0;
}
