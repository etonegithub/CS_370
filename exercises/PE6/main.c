#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int a = atoi(argv[1]);

    unsigned long long factorial = 1;

    while (a > 0) {
        factorial *= a;
        a--;
    }
    printf("%llu\n", factorial);
    return 0;    
}