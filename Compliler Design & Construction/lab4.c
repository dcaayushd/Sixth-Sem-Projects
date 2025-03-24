/*
    Lab No. 4
    Program for Lexical Analyzer in C.

    This program reads a source code file and identifies:
    - Operators: +, -, *, /, %, =
    - Keywords: C programming keywords (e.g., int, if, else, while, etc.)
    - Identifiers: Any valid variable/function names that are not keywords
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char buffer[]) {
    char keywords[32][10] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    int i, flag = 0;
    for (i = 0; i < 32; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            flag = 1;
            break;
        }
    }
    return flag;
}

int main() {
    char ch, buffer[15], operators[] = "+-*/%=";
    FILE *fp;
    int i, j = 0;

    fp = fopen("aa.txt", "r");
    if (fp == NULL) {
        printf("Error while opening the file\n");
        exit(0);
    }

    while ((ch = fgetc(fp)) != EOF) {
        // Check for operators
        for (i = 0; i < 6; ++i) {
            if (ch == operators[i]) {
                printf("%c is an operator\n", ch);
            }
        }

        // Check for alphanumeric characters (part of keywords or identifiers)
        if (isalnum(ch)) {
            buffer[j++] = ch;
        }
        // If whitespace or newline is encountered, process the word
        else if ((ch == ' ' || ch == '\n') && (j != 0)) {
            buffer[j] = '\0';
            j = 0;

            if (isKeyword(buffer) == 1)
                printf("%s is a keyword\n", buffer);
            else
                printf("%s is an identifier\n", buffer);
        }
    }

    fclose(fp);
    return 0;
}
