#include <stdio.h>
#include <stdbool.h>


bool is_prime(int n) {
    // loop through all numbers between 2 and n
    for (int i = 2; i < n; i++) {
        // if n is divisible by i, number is not prime
        if (n % i == 0) {
            return false;
        }
    }
    // if n is not divisible by any number between 2 and itself, it is prime
    return true;
}


int main() {
    int num;
    scanf("%d", &num);

    // loop through all numbers between 2 and input number (b/c 2 is the first prime number)
    for (int i = 2; i <= num; i++) {
        if (is_prime(i)) {
            printf("%d\n", i);
        }
    }  
}

