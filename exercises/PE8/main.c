#include <stdio.h>

int main(int argc, char* argv[]) {
    char str[1000];
    scanf("%s", str);

    int running_count = 0;

    //printf("%s\n", str);

    int i = 0;
    while (str[i] != '\0') {
        //printf("%c\n", str[i]);
        //printf("%c", str[i]);
        if (str[i] == 'a' || str[i] == 'A' ||
            str[i] == 'e' || str[i] == 'E' ||
            str[i] == 'i' || str[i] == 'I' ||
            str[i] == 'o' || str[i] == 'O' ||
            str[i] == 'u' || str[i] == 'U') {
                //printf("vowel: %c", str[i]);
                running_count++;
        }
        i++;
    }
    
    printf("%d\n", running_count);
    
}