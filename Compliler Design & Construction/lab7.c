/*
    Lab No. 7
    C Program to construct an LL(1) Parser.

    This program takes an input string and parses it using an LL(1) parsing table.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char s[20], stack[20];

int main()
{
    // LL(1) Parsing Table (Grammar Production Rules)
    char m[5][6][4] = {
        {"tb", " ", " ", "tb", " ", " "},
        {" ", "+tb", " ", " ", "n", "n"},
        {"fc", " ", " ", "fc", " ", " "},
        {" ", "n", "*fc", "n", "n", "n"},
        {"i", " ", " ", "(e)", " ", " "}};

    // Size table to track production lengths
    int size[5][6] = {
        {2, 0, 0, 2, 0, 0},
        {0, 3, 0, 0, 1, 1},
        {2, 0, 0, 2, 0, 0},
        {0, 1, 3, 0, 1, 1},
        {1, 0, 0, 3, 0, 0}};

    int i, j, k, n, str1, str2;

    // Get input string
    printf("\nEnter the input string: ");
    scanf("%s", s);
    strcat(s, "$"); // Append end marker
    n = strlen(s);

    // Initialize stack with start symbol
    stack[0] = '$';
    stack[1] = 'e';
    i = 1;
    j = 0;

    printf("\nStack     Input\n");
    printf("__________________\n");

    while ((stack[i] != '$') && (s[j] != '$'))
    {
        if (stack[i] == s[j])
        {
            i--;
            j++;
        }

        // Map stack top to parsing table row index
        switch (stack[i])
        {
        case 'e':
            str1 = 0;
            break;
        case 'b':
            str1 = 1;
            break;
        case 't':
            str1 = 2;
            break;
        case 'c':
            str1 = 3;
            break;
        case 'f':
            str1 = 4;
            break;
        default:
            str1 = -1;
        }

        // Map input symbol to parsing table column index
        switch (s[j])
        {
        case 'i':
            str2 = 0;
            break;
        case '+':
            str2 = 1;
            break;
        case '*':
            str2 = 2;
            break;
        case '(':
            str2 = 3;
            break;
        case ')':
            str2 = 4;
            break;
        case '$':
            str2 = 5;
            break;
        default:
            str2 = -1;
        }

        if (str1 == -1 || str2 == -1 || m[str1][str2][0] == '\0')
        {
            printf("\nERROR: Invalid input string.\n");
            exit(0);
        }

        if (m[str1][str2][0] == 'n')
        {
            i--; // Pop stack
        }
        else if (m[str1][str2][0] == 'i')
        {
            stack[i] = 'i'; // Replace stack top
        }
        else
        {
            for (k = size[str1][str2] - 1; k >= 0; k--)
            {
                stack[i] = m[str1][str2][k];
                i++;
            }
            i--;
        }

        // Display stack and input
        for (k = 0; k <= i; k++)
            printf(" %c", stack[k]);

        printf("       ");

        for (k = j; k <= n; k++)
            printf("%c", s[k]);

        printf("\n");
    }

    printf("\nSUCCESS: The input string is successfully parsed.\n");
    return 0;
}
