#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define FILENAME "execution_count.txt"

void create_branch(int current_depth, int max_depth, pid_t parent_pid) {
    if (current_depth > max_depth) {
        return;
    }

    pid_t pid1;
    if ((pid1 = fork()) == 0) { // child process 1
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
        create_branch(current_depth + 1, max_depth, getpid());
        printf("    T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else { // parent process
        waitpid(pid1, NULL, 0); // wait for child process 1 to finish

        pid_t pid2;
        if ((pid2 = fork()) == 0) { // child process 2
            printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
            create_branch(current_depth + 1, max_depth, getpid());
            printf("    T[%d, %d]\n", getpid(), parent_pid);
            exit(0);
        } else { // parent process
            waitpid(pid2, NULL, 0); // wait for child process 2 to finish
        }
    }
}
void create_free(int current_depth, int max_depth, pid_t parent_pid) {
    if (current_depth > max_depth) {
        return;
    }

    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) { // child process 1
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
        create_free(current_depth + 1, max_depth, getpid());
        printf("    T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else if ((pid2 = fork()) == 0) { // child process 2
        printf("n=%d C[%d, %d]\n", current_depth, getpid(), parent_pid);
        create_free(current_depth + 1, max_depth, getpid());
        printf("    T[%d, %d]\n", getpid(), parent_pid);
        exit(0);
    } else { // parent process
        waitpid(pid1, NULL, 0); // wait for child process 1 to finish
        waitpid(pid2, NULL, 0); // wait for child process 2 to finish
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

    FILE* fp = fopen(FILENAME, "r");
    int num_executions = 0;
    if (fp) {
        fscanf(fp, "%d", &num_executions);
        fclose(fp);
    }

    double times_branch[30];
    double times_free[30];
    
    for(int i = 0; i < 30; i++){
        clock_t start_time_branch = clock();
	    clock_t start_time_free = clock();
        pid_t main_pid = getpid();

        printf("\n--------- Branch ---------\n");

        printf("n=0 PID=%d (root)\n", main_pid);
        create_branch(1, max_depth, main_pid);

	    printf("--- Branch creation finished ---\n");
        clock_t end_time_branch = clock();
        times_branch[i] = (double)(end_time_branch - start_time_branch) / CLOCKS_PER_SEC;
		printf("Branch execution time in %f seconds.\n", times_branch[i]);

	    printf("\n");
	    printf("--------- Free ---------\n");
        printf("n=0 PID=%d (root)\n", main_pid);
        create_free(1, max_depth, main_pid);

	    printf("--- Free creation finished ---\n");
        clock_t end_time_free = clock();
        times_free[i] = (double)(end_time_free - start_time_free) / CLOCKS_PER_SEC;
		printf("Free execution time in %f seconds.\n", times_free[i]);

        printf("\nFim PID = %d(root)\n", main_pid);
    }

	double sum_branch = 0, sum_free = 0;
    for (int i = 0; i < 30; i++) {
        sum_branch += times_branch[i];
        sum_free += times_free[i];
    }

    double avg_branch = sum_branch / 30;
    double avg_free = sum_free / 30;

    printf("\nAverage execution time for Branch: %f seconds\n", avg_branch);
    printf("Average execution time for Free: %f seconds\n", avg_free);

    // Abrir arquivo para escrita
    FILE* file = fopen("results.txt", "a");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    num_executions++;

    fp = fopen(FILENAME, "w");
    if (fp) {
        fprintf(fp, "%d\n", num_executions);
        fprintf(fp, "Average execution time for Branch: %f seconds\n", avg_branch);
        fprintf(fp, "Average execution time for Free: %f seconds\n", avg_free);
        fclose(fp);
    }

    return 0;
}
