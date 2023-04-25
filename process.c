#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_DEPTH 4

void create_processes(int current_depth, int max_depth, pid_t parent_pid) {

	if(current_depth > max_depth){
		return;
	}

	pid_t pid;

	if((pid = fork()) == 0){
		printf("n=%d C[%d, %d] \n", current_depth, getpid(), parent_pid);
		create_processes(current_depth + 1, max_depth, getpid());
		printf("T[%d, %d]\n", getpid(), getppid());
		exit(0);
	}else {
		waitpid(pid, NULL, 0);
	}

}

int main() {
	printf("Creating process tree...\n");

	pid_t main_pid = getpid();
	create_processes(1, MAX_DEPTH, main_pid);

	printf("Process tree creating finish.\n");
	
	return 0;
}
