#include <iostream>
#include <vector>
#include <stack>
#include <cctype>
#include <sstream>

using namespace std;

// Structure for Quadruple Representation
struct Quadruple {
    string op, arg1, arg2, result;
};

// Structure for Triple Representation
struct Triple {
    string op, arg1, arg2;
};

vector<Quadruple> quadruples;
vector<Triple> triples;
int tempCount = 0; // Counter for temporary variables

// Function to generate a new temporary variable
string newTemp() {
    return "t" + to_string(tempCount++);
}

// Function to check if a character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Function to get operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to convert infix expression to postfix
vector<string> infixToPostfix(string expr) {
    vector<string> postfix;
    stack<char> operators;
    string operand;

    for (char c : expr) {
        if (isalnum(c)) {
            operand += c;
        } else {
            if (!operand.empty()) {
                postfix.push_back(operand);
                operand.clear();
            }
            if (isOperator(c)) {
                while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                    postfix.push_back(string(1, operators.top()));
                    operators.pop();
                }
                operators.push(c);
            }
        }
    }
    if (!operand.empty()) postfix.push_back(operand);
    while (!operators.empty()) {
        postfix.push_back(string(1, operators.top()));
        operators.pop();
    }
    return postfix;
}

// Function to generate quadruple and triple representations
void generateRepresentations(vector<string> postfix) {
    stack<string> operands;
    for (const string &token : postfix) {
        if (isOperator(token[0]) && token.length() == 1) {
            string op2 = operands.top(); operands.pop();
            string op1 = operands.top(); operands.pop();
            string temp = newTemp();
            
            quadruples.push_back({token, op1, op2, temp});
            triples.push_back({token, op1, op2});
            
            operands.push(temp);
        } else {
            operands.push(token);
        }
    }
    quadruples.push_back({"=", operands.top(), "", "RESULT"});
    triples.push_back({"=", operands.top(), "RESULT"});
}

int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);
    
    vector<string> postfix = infixToPostfix(expression);
    generateRepresentations(postfix);
    
    // Display Quadruple Representation
    cout << "\nQuadruple Representation:\n";
    cout << "---------------------------------------------------\n";
    cout << "| Operator | Arg1  | Arg2  | Result  |\n";
    cout << "---------------------------------------------------\n";
    for (const auto& q : quadruples) {
        cout << "| " << q.op << "\t   | " << q.arg1 << "\t | " << q.arg2 << "\t | " << q.result << "\t |\n";
    }
    cout << "---------------------------------------------------\n\n";

    // Display Triple Representation
    cout << "Triple Representation:\n";
    cout << "-------------------------------\n";
    cout << "| Index | Operator | Arg1  | Arg2  |\n";
    cout << "-------------------------------\n";
    for (size_t i = 0; i < triples.size(); ++i) {
        cout << "| " << i << "\t | " << triples[i].op << "\t   | " << triples[i].arg1 << "\t | " << triples[i].arg2 << "\t |\n";
    }
    cout << "-------------------------------\n";

    return 0;
}
