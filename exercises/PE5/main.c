#include <stdio.h>

int main() {
    int a;
    scanf("%d", &a);
    
    for (int i = 0; i < 10; i++) {
        printf("%d\n\n", a * (i + 1));
    }

    return 0;
}