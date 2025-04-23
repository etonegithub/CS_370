#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int a = atoi(argv[1]);
    
    if (a % 2 == 0) {
        printf("even");
    } else {
        printf("odd");
    }
    return 0;
}