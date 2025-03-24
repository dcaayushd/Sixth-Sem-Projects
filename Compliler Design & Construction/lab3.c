/*
    Lab No. 3
    Write a C program to test whether a given identifier is valid or not.
    - A valid identifier must start with a letter (A-Z or a-z) or an underscore (_).
    - The rest of the identifier can contain letters, digits (0-9), or underscores (_).
*/

#include <stdio.h>
#include <ctype.h>

int main() {
    char a[50]; // Increased size to handle longer identifiers
    int flag = 1, i = 1;

    printf("\n Enter an identifier: ");
    fgets(a, sizeof(a), stdin);
    a[strcspn(a, "\n")] = '\0'; // Remove newline character if present

    // Check first character
    if (!isalpha(a[0]) && a[0] != '_') {
        printf("\n Not a valid identifier");
        return 0;
    }

    // Check remaining characters
    while (a[i] != '\0') {
        if (!isalnum(a[i]) && a[i] != '_') {
            flag = 0;
            break;
        }
        i++;
    }

    if (flag == 1)
        printf("\n Valid identifier");
    else
        printf("\n Not a valid identifier");

    return 0;
}
