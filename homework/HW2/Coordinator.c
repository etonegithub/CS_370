#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char *divisor = argv[1];

    for (int i = 2; i < argc; i++) {
        pid_t pid = fork();

        // fork failed
        if (pid == -1) {
            printf("Coordinator: Fork failed.\n");
        }
        // current process is child
        else if (pid == 0) {
            execl("./checker", "checker", divisor, argv[i], NULL);
        }
        // current process is parent
        else {
            printf("Coordinator: forked process with ID %d.\n", pid);
            int status;
            printf("Coordinator: waiting for process [%d].\n", pid);
            wait(&status);
            //waitpid(pid, &status, 0);
            //if (WIFEXITED(status)) {
            int result = WEXITSTATUS(status);
            printf("Coordinator: child process %d returned %d.\n", pid, result);
            //}
            // else if (WIFSIGNALED(status)) {
            //     printf("Coordinator: child process %ld terminated due to signal %d\n", pid, WTERMSIG(status));
            // }
        }
    }
    printf("Coordinator: exiting.\n");
}

