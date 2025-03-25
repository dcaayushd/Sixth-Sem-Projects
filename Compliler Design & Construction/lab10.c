// Program: Final Code Generation

#include <stdio.h>
#include <string.h>

char op[2], arg1[5], arg2[5], result[5];

int main()
{
    FILE *fp1, *fp2;

    // Open input and output files
    fp1 = fopen("input.txt", "r");
    fp2 = fopen("output.txt", "w");

    // Check if files opened successfully
    if (fp1 == NULL || fp2 == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    // Read the operations from input file and generate the corresponding final code
    while (!feof(fp1))
    {
        fscanf(fp1, "%s%s%s%s", op, arg1, arg2, result);

        if (strcmp(op, "+") == 0)
        {
            fprintf(fp2, "\n MOV R0, %s", arg1);
            fprintf(fp2, "\n ADD R0, %s", arg2);
            fprintf(fp2, "\n MOV %s, R0", result);
        }
        if (strcmp(op, "*") == 0)
        {
            fprintf(fp2, "\n MOV R0, %s", arg1);
            fprintf(fp2, "\n MUL R0, %s", arg2);
            fprintf(fp2, "\n MOV %s, R0", result);
        }
        if (strcmp(op, "-") == 0)
        {
            fprintf(fp2, "\n MOV R0, %s", arg1);
            fprintf(fp2, "\n SUB R0, %s", arg2);
            fprintf(fp2, "\n MOV %s, R0", result);
        }
        if (strcmp(op, "/") == 0)
        {
            fprintf(fp2, "\n MOV R0, %s", arg1);
            fprintf(fp2, "\n DIV R0, %s", arg2);
            fprintf(fp2, "\n MOV %s, R0", result);
        }
        if (strcmp(op, "=") == 0)
        {
            fprintf(fp2, "\n MOV R0, %s", arg1);
            fprintf(fp2, "\n MOV %s, R0", result);
        }
    }

    // Close the files
    fclose(fp1);
    fclose(fp2);

    return 0;
}