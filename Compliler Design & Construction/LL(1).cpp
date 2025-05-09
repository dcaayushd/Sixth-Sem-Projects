#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

int main() {
    char tmp[15];
    char str[15], csf[15] = {"E"};
    int ssm = 0, i, j, v, k, pos = 0, a;
    

    system("clear"); 
    
    char pt[6][5][4] = {
        {"", "i", "+", "*", "/"},
        {"E", "TX", "n", "n", "n"},
        {"X", "n", "TX", "n", ""},
        {"T", "VY", "n", "n", "n"},
        {"Y", "n", "", "VY", ""},
        {"V", "i", "n", "n", "n"}
    };

    cout << "\n Enter An Expression: " << endl;
    cin >> str;

    while(str[ssm] != '/') {
        pos = 0;
        while(csf[pos] != 'E' && csf[pos] != 'X' && csf[pos] != 'T' && csf[pos] != 'Y' && csf[pos] != 'V' && csf[pos] != '\0')
            pos++;
        if (csf[pos] == '\0')
            break;

        for(i = 1; i < 6; i++) {
            if(csf[pos] == pt[i][0][0])
                break;
        }

        for(j = 1; j < 5; j++) {
            if(str[ssm] == pt[0][j][0])
                break;
        }

        if(strcmp(pt[i][j], "n") == 0) {
            cout << "\n Null Value: " << endl;
            cout << pt[i][j];
            exit(0); // or return 0;
        }

        for(k = 0; k < pos; k++)
            tmp[k] = csf[k];
        tmp[k] = '\0';
        strcat(tmp, pt[i][j]);

        int l = strlen(tmp);
        k = l;

        for(a = pos + 1; csf[a] != '\0'; a++, k++)
            tmp[k] = csf[a];
        strcpy(csf, tmp);
    }

    strcat(csf, "/");
    if(strcmp(csf, str) == 0) {
        cout << "\t csf" << csf;
        cout << "\tstr" << str;
        cout << "\n Valid Expression: " << endl;
    } else {
        cout << "\n Invalid Expression:" << endl;
    }

    cin.get(); // Wait for user input
    return 0;
   }