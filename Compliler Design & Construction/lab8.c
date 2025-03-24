// C Program to Implement Shift Reduce Parser  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ip_sym[15], stack[15];
int ip_ptr = 0, st_ptr = 0, len, i;
char temp[2], temp2[2];
char act[15];

void check();

int main() {
    printf("\n\t\t SHIFT REDUCE PARSER\n");

    printf("\nGRAMMAR:");
    printf("\nE -> E + E");
    printf("\nE -> E / E");
    printf("\nE -> E * E");
    printf("\nE -> a / b\n");

    printf("\nEnter the input symbol: ");
    fgets(ip_sym, sizeof(ip_sym), stdin);
    ip_sym[strcspn(ip_sym, "\n")] = 0; // Remove newline character

    printf("\n\tStack Implementation Table");
    printf("\nStack\t\t Input Symbol\t\t Action");
    printf("\n______\t\t ____________\t\t ______\n");
    
    printf("\n $ \t\t%s$\t\t --", ip_sym);

    strcpy(act, "shift ");
    temp[0] = ip_sym[ip_ptr];
    temp[1] = '\0';
    strcat(act, temp);

    len = strlen(ip_sym);

    for (i = 0; i < len; i++) {
        stack[st_ptr] = ip_sym[ip_ptr];
        stack[st_ptr + 1] = '\0';
        ip_sym[ip_ptr] = ' ';
        ip_ptr++;

        printf("\n $%s\t\t%s$\t\t %s", stack, ip_sym, act);

        strcpy(act, "shift ");
        temp[0] = ip_sym[ip_ptr];
        temp[1] = '\0';
        strcat(act, temp);

        check();
        st_ptr++;
    }
    
    st_ptr++;
    check();

    return 0;
}

void check() {
    int flag = 0;
    temp2[0] = stack[st_ptr];
    temp2[1] = '\0';

    if ((strcasecmp(temp2, "a") == 0) || (strcasecmp(temp2, "b") == 0)) {
        stack[st_ptr] = 'E';
        printf("\n $%s\t\t%s$\t\t E->%s", stack, ip_sym, temp2);
        flag = 1;
    }

    if ((strcasecmp(temp2, "+") == 0) || (strcasecmp(temp2, "*") == 0) || (strcasecmp(temp2, "/") == 0)) {
        flag = 1;
    }

    if ((strcasecmp(stack, "E+E") == 0) || (strcasecmp(stack, "E/E") == 0) || (strcasecmp(stack, "E*E") == 0)) {
        strcpy(stack, "E");
        st_ptr = 0;

        if (strcasecmp(stack, "E+E") == 0)
            printf("\n $%s\t\t%s$\t\t E->E+E", stack, ip_sym);
        else if (strcasecmp(stack, "E/E") == 0)
            printf("\n $%s\t\t%s$\t\t E->E/E", stack, ip_sym);
        else
            printf("\n $%s\t\t%s$\t\t E->E*E", stack, ip_sym);

        flag = 1;
    }

    if (strcasecmp(stack, "E") == 0 && ip_ptr == len) {
        printf("\n $%s\t\t%s$\t\t ACCEPT", stack, ip_sym);
        exit(0);
    }

    if (flag == 0) {
        printf("\n %s\t\t%s\t\t REJECT", stack, ip_sym);
        exit(0);
    }
}
