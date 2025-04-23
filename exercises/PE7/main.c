#include <stdio.h>

int main(int argc, char* argv[]) {
    int a;
    scanf("%d", &a);
    
    int size = 2 * a;
    int arr[size];
    
    for (int i = 0; i < a; i++) {
        scanf("%d", &arr[i]);
    }
    
    for (int i = a - 1; i >= 0 ; i--) {
        printf("%d\n\n", arr[i]);
    }
    
}