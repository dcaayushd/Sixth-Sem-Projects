/*
    Lab No. 5
    C program to implement FIRST of a given grammar.

    This program calculates the FIRST set for a given grammar.
    FIRST(X) is defined as:
    - If X is a terminal, then FIRST(X) = {X}
    - If X is a non-terminal and X → Y1 Y2 ... Yk is a production, then
      FIRST(X) includes FIRST(Y1) (excluding epsilon, unless all Y's can derive epsilon).
*/

#include <stdio.h>
#include <ctype.h>

void FIRST(char[], char);
void addToResultSet(char[], char);

int numOfProductions;
char productionSet[10][10];

int main() {
    int i;
    char choice;
    char c;
    char result[20];

    printf("How many number of productions? : ");
    scanf(" %d", &numOfProductions);

    for (i = 0; i < numOfProductions; i++) { // Read production strings
        printf("Enter production Number %d : ", i + 1);
        scanf(" %s", productionSet[i]);
    }

    do {
        printf("\n Find the FIRST of : ");
        scanf(" %c", &c);

        FIRST(result, c); // Compute FIRST and store the result in 'result' array

        printf("\n FIRST(%c) = { ", c);
        for (i = 0; result[i] != '\0'; i++)
            printf(" %c ", result[i]); // Display result
        printf("}\n");

        printf("Press 'y' to continue : ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    return 0;
}

void FIRST(char* Result, char c) {
    int i, j, k;
    char subResult[20];
    int foundEpsilon;

    subResult[0] = '\0';
    Result[0] = '\0';

    // If X is a terminal, FIRST(X) = {X}
    if (!isupper(c)) {
        addToResultSet(Result, c);
        return;
    }

    // If X is a non-terminal, process each production
    for (i = 0; i < numOfProductions; i++) {
        // Find production with X as LHS
        if (productionSet[i][0] == c) {
            if (productionSet[i][2] == '$') { // Epsilon production
                addToResultSet(Result, '$');
            } else {
                j = 2;
                while (productionSet[i][j] != '\0') {
                    foundEpsilon = 0;
                    FIRST(subResult, productionSet[i][j]);

                    for (k = 0; subResult[k] != '\0'; k++)
                        addToResultSet(Result, subResult[k]);

                    // Check if epsilon is present in FIRST(Y)
                    for (k = 0; subResult[k] != '\0'; k++) {
                        if (subResult[k] == '$') {
                            foundEpsilon = 1;
                            break;
                        }
                    }

                    // If epsilon is not found, stop checking next elements
                    if (!foundEpsilon)
                        break;

                    j++;
                }
            }
        }
    }
    return;
}

void addToResultSet(char Result[], char val) {
    int k;
    for (k = 0; Result[k] != '\0'; k++)
        if (Result[k] == val)
            return; // Avoid duplicates

    Result[k] = val;
    Result[k + 1] = '\0';
}
