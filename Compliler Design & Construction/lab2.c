/*
    Lab No. 2
    Write a C program to recognize strings under the following regular expressions:
    - 'a*'  (zero or more occurrences of 'a')
    - 'a*b+' (zero or more 'a' followed by one or more 'b')
    - 'abb' (exactly "abb")
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char s[20], c;
    int state = 0, i = 0;

    printf("\n Enter a string: ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0'; // Remove trailing newline if present

    while (s[i] != '\0')
    {
        switch (state)
        {
        case 0:
            c = s[i++];
            if (c == 'a')
                state = 1;
            else if (c == 'b')
                state = 2;
            else
                state = 6;
            break;

        case 1:
            c = s[i++];
            if (c == 'a')
                state = 3;
            else if (c == 'b')
                state = 4;
            else
                state = 6;
            break;

        case 2:
            c = s[i++];
            if (c == 'b')
                state = 5;
            else
                state = 6;
            break;

        case 3:
            c = s[i++];
            if (c == 'a')
                state = 3;
            else if (c == 'b')
                state = 4;
            else
                state = 6;
            break;

        case 4:
            c = s[i++];
            if (c == 'b')
                state = 5;
            else
                state = 6;
            break;

        case 5:
            c = s[i++];
            if (c == 'b')
                state = 5;
            else
                state = 6;
            break;

        case 6:
            printf("\n Invalid string");
            return 0;
        }
    }

    // Check final state for acceptance
    if (state == 1 || state == 3)
    {
        printf("\n String is accepted under 'a*'");
    }
    else if (state == 4 || state == 5)
    {
        printf("\n String is accepted under 'a*b+'");
    }
    else if (state == 5)
    {
        printf("\n String is accepted as 'abb'");
    }
    else
    {
        printf("\n Invalid string");
    }

    return 0;
}