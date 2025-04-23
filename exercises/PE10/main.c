#include <stdio.h>
#include "sort.h"


int main() {
    int a;
    scanf("%d", &a);
    
    int arr[a];
    
    for (int i = 0; i < a; i++) {
        scanf("%d", &arr[i]);
    }

    sort(arr, a);

    for (int i = 0; i < a; i++) {
        printf("%d\n", arr[i]);
    }
}

