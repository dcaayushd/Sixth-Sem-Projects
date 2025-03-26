// Program: Intermediate Code Generation

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int i = 0, j = 0, no = 0, tmpch = 90; // ASCII 'Z'
char str[100], left[15], right[15];

void explore();
void fleft(int);
void fright(int);

struct exp
{
    int pos;
    char op;
} k[15];

int main()
{
    printf("\t\t INTERMEDIATE CODE GENERATION\n\n");

    // Read the expression
    printf("Enter the expression: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0; // Remove newline character

    // Identify operators in the expression
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '=')
        {
            k[j].pos = i;
            k[j++].op = str[i];
        }
    }
    k[j].op = '\0'; // Mark end of operators

    // Process the expression
    explore();

    return 0;
}

void explore()
{
    i = 0;
    while (k[i].op != '\0')
    {
        fleft(k[i].pos);
        fright(k[i].pos);

        str[k[i].pos] = tmpch--; // Replace operator with temporary variable

        printf("\t%c := %s %c %s\n", str[k[i].pos], left, k[i].op, right);

        // Print updated expression
        for (j = 0; j < strlen(str); j++)
        {
            if (str[j] != '$')
                printf("%c", str[j]);
        }
        printf("\n");
        i++;
    }

    fright(-1);
    if (no == 0)
    {
        fleft(strlen(str));
        printf("\t%s := %s\n", right, left);
        exit(0);
    }

    printf("\t%s := %c\n", right, str[k[--i].pos]);
}

void fleft(int x)
{
    int w = 0, flag = 0;
    x--;

    while (x != -1 && str[x] != '+' && str[x] != '*' && str[x] != '=' && str[x] != '\0' &&
           str[x] != '-' && str[x] != '/' && str[x] != ':')
    {
        if (str[x] != '$' && flag == 0)
        {
            left[w++] = str[x];
            left[w] = '\0';
            str[x] = '$';
            flag = 1;
        }
        x--;
    }
}

void fright(int x)
{
    int w = 0, flag = 0;
    x++;

    while (x != -1 && str[x] != '+' && str[x] != '*' && str[x] != '\0' &&
           str[x] != '=' && str[x] != ':' && str[x] != '-' && str[x] != '/')
    {
        if (str[x] != '$' && flag == 0)
        {
            right[w++] = str[x];
            right[w] = '\0';
            str[x] = '$';
            flag = 1;
        }
        x++;
    }
}