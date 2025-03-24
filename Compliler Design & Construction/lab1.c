/*
    Unit 9 - Lab No. 1
    Write a program in C to test whether the given entered string 
    is within a valid comment section or not.
*/
#include <stdio.h>
#include <string.h>

int main() {
    char com[30];
    int i = 2, a = 0;

    printf("\n Enter comment: ");
    fgets(com, sizeof(com), stdin);

    if (com[0] == '/') {
        if (com[1] == '/') {
            printf("\n It is a comment");
        } else if (com[1] == '*') {
            for (i = 2; i < 30; i++) {
                if (com[i] == '*' && com[i + 1] == '/') {
                    printf("\n It is a comment");
                    a = 1;
                    break;
                }
            }
            if (a == 0) {
                printf("\n It is not a comment");
            }
        } else {
            printf("\n It is not a comment");
        }
    } else {
        printf("\n It is not a comment");
    }

    return 0;
}
