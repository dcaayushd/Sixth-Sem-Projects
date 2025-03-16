#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

// Structure for Quadruple Representation
struct Quadruple {
    string op, arg1, arg2, result;
};

// Structure for Triple Representation
struct Triple {
    string op, arg1, arg2;
};

int main() {
    vector<Quadruple> quadruples;
    vector<Triple> triples;
    
    // Expression: A = B + C − D * E + G
    // Step-wise breakdown:
    // t1 = D * E
    // t2 = B + C
    // t3 = t2 - t1
    // t4 = t3 + G
    // A = t4

    // Quadruple Representation
    quadruples.push_back({"*", "D", "E", "t1"});
    quadruples.push_back({"+", "B", "C", "t2"});
    quadruples.push_back({"-", "t2", "t1", "t3"});
    quadruples.push_back({"+", "t3", "G", "t4"});
    quadruples.push_back({"=", "t4", "", "A"});

    // Triple Representation
    triples.push_back({"*", "D", "E"});   // (0) t1 = D * E
    triples.push_back({"+", "B", "C"});   // (1) t2 = B + C
    triples.push_back({"-", "1", "0"});   // (2) t3 = t2 - t1
    triples.push_back({"+", "2", "G"});   // (3) t4 = t3 + G
    triples.push_back({"=", "3", "A"});   // (4) A = t4

    // Display Quadruple Representation
    cout << "Quadruple Representation:\n";
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
