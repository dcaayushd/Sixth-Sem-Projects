/*
    Lab No. 6
    C Program to Calculate FOLLOW(A) for a given grammar.

    This program computes the FOLLOW set of a given non-terminal in a grammar.
    The FOLLOW set of a non-terminal A is defined as:
    - If S is the start symbol, then FOLLOW(S) includes '$' (end of input).
    - If A → αBβ is a production, then everything in FIRST(β) (except ε) is in FOLLOW(B).
    - If A → αB or A → αBβ where FIRST(β) contains ε, then FOLLOW(B) includes FOLLOW(A).
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n, i = 0, j = 0;
char a[10][10], Result[10], subResult[20];

void follow(char* Result, char c);
void first(char* Result, char c);
void addToResultSet(char[], char);

int main() {
    int choice;
    char c;

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter %d productions\n(Note: Productions with multiple terms should be given as separate productions)\n", n);
    for (i = 0; i < n; i++)
        scanf("%s", a[i]);

    do {
        printf("Find FOLLOW of --> ");
        scanf(" %c", &c);

        follow(Result, c); // Compute FOLLOW set

        printf("FOLLOW(%c) = { ", c);
        for (i = 0; Result[i] != '\0'; i++)
            printf(" %c ", Result[i]);
        printf(" }\n");

        printf("Do you want to continue? (Press 1 to continue...): ");
        scanf("%d", &choice);

    } while (choice == 1);

    return 0;
}

void follow(char* Result, char c) {
    int k;
    subResult[0] = '\0';
    Result[0] = '\0';

    // If the start symbol, add '$' to FOLLOW set
    if (a[0][0] == c)
        addToResultSet(Result, '$');

    for (i = 0; i < n; i++) {
        for (j = 2; j < strlen(a[i]); j++) {
            if (a[i][j] == c) {
                // If B → αAβ, then FIRST(β) ⊆ FOLLOW(A)
                if (a[i][j + 1] != '\0')
                    first(subResult, a[i][j + 1]);

                // If B → αA or B → αAβ where FIRST(β) contains ε, then FOLLOW(B) ⊆ FOLLOW(A)
                if (a[i][j + 1] == '\0' && c != a[i][0])
                    follow(subResult, a[i][0]);

                for (k = 0; subResult[k] != '\0'; k++)
                    addToResultSet(Result, subResult[k]);
            }
        }
    }
}

void first(char* R, char c) {
    int k, m;

    // If it's a terminal, add it to the FIRST set
    if (!isupper(c) && c != '#')
        addToResultSet(R, c);

    for (k = 0; k < n; k++) {
        if (a[k][0] == c) {
            if (a[k][2] == '#' && c != a[i][0])
                follow(R, a[i][0]);
            else if (!isupper(a[k][2]) && a[k][2] != '#')
                addToResultSet(R, a[k][2]);
            else
                first(R, a[k][2]);

            for (m = 0; R[m] != '\0'; m++)
                addToResultSet(Result, R[m]);
        }
    }
}

void addToResultSet(char Result[], char val) {
    int k;
    for (k = 0; Result[k] != '\0'; k++)
        if (Result[k] == val)
            return; // Avoid duplicates

    Result[k] = val;
    Result[k + 1] = '\0';
}
