#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int divisor = atoi(argv[1]);
    int dividend = atoi(argv[2]);
    pid_t pid = getpid();

    printf("Checker process [%d]: Starting.\n", pid);

    // inputs are assumed to be positive as per assignment description
    if (dividend % divisor == 0) {
        printf("Checker process [%d]: %d *IS* divisible by %d.\n", pid, dividend, divisor);
        printf("Checker process [%d]: Returning 1.\n", pid);
        return 1;
    }
    else {
        printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", pid, dividend, divisor);
        printf("Checker process [%d]: Returning 0.\n", pid);
        return 0;
    }
}