#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <sstream>

using namespace std;

// Structure for LR(0) items
struct Item {
    string left;
    string right;
    int dotPos;

    bool operator==(const Item &other) const {
        return left == other.left && right == other.right && dotPos == other.dotPos;
    }
};

// Structure for LR(0) states
struct State {
    vector<Item> items;
    map<char, int> transitions;
};

// Function to check if an item exists in a list
bool contains(const vector<Item> &items, const Item &item) {
    for (const auto &it : items)
        if (it == item)
            return true;
    return false;
}

// Closure function for LR(0)
vector<Item> closure(vector<Item> items, const map<char, vector<string>> &grammar) {
    bool changed = true;
    while (changed) {
        changed = false;
        vector<Item> newItems = items;
        for (const auto &item : items) {
            if (item.dotPos < item.right.size()) {
                char symbol = item.right[item.dotPos];
                if (grammar.find(symbol) != grammar.end()) {
                    for (const string &prod : grammar.at(symbol)) {
                        Item newItem = {string(1, symbol), prod, 0};  // Fixed string conversion
                        if (!contains(newItems, newItem)) {
                            newItems.push_back(newItem);
                            changed = true;
                        }
                    }
                }
            }
        }
        items = newItems;
    }
    return items;
}

// GOTO function for LR(0)
vector<Item> GOTO(const vector<Item> &state, char symbol, const map<char, vector<string>> &grammar) {
    vector<Item> newState;
    for (const auto &item : state) {
        if (item.dotPos < item.right.size() && item.right[item.dotPos] == symbol) {
            newState.push_back({item.left, item.right, item.dotPos + 1});
        }
    }
    return closure(newState, grammar);
}

// Check if a state already exists
int findState(const vector<State> &states, const vector<Item> &items) {
    for (int i = 0; i < states.size(); ++i) {
        if (states[i].items == items)
            return i;
    }
    return -1;
}

// Construct the LR(0) parsing table
vector<State> constructLR0Table(const map<char, vector<string>> &grammar, char startSymbol) {
    vector<State> states;
    
    // Ensure first production is a valid string
    vector<Item> startItems = closure({{string(1, startSymbol), grammar.at(startSymbol)[0], 0}}, grammar);
    states.push_back({startItems, {}});

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < states.size(); ++i) {
            set<char> symbols;
            for (const auto &item : states[i].items) {
                if (item.dotPos < item.right.size()) {
                    symbols.insert(item.right[item.dotPos]);
                }
            }

            for (char symbol : symbols) {
                vector<Item> newItems = GOTO(states[i].items, symbol, grammar);
                if (!newItems.empty()) {
                    int index = findState(states, newItems);
                    if (index == -1) {
                        states.push_back({newItems, {}});
                        index = states.size() - 1;
                        changed = true;
                    }
                    states[i].transitions[symbol] = index;
                }
            }
        }
    }
    return states;
}

// Simulate LR(0) Parsing
bool parseLR0(const vector<State> &states, const map<pair<int, char>, string> &parsingTable, const string &input) {
    stack<int> stateStack;
    stateStack.push(0);
    int pos = 0;

    while (true) {
        int currState = stateStack.top();
        char lookahead = input[pos];

        if (parsingTable.find({currState, lookahead}) == parsingTable.end()) {
            cout << "Parsing error at position " << pos << "!\n";
            return false;
        }

        string action = parsingTable.at({currState, lookahead});
        if (action[0] == 'S') {
            int newState = stoi(action.substr(1));
            stateStack.push(newState);
            pos++;
        } else if (action[0] == 'R') {
            cout << "Reduction step not implemented yet!\n";
            return false;
        } else if (action == "ACCEPT") {
            cout << "Parsing successful!\n";
            return true;
        }
    }
}

int main() {
    map<char, vector<string>> grammar = {
        {'S', {"E"}},
        {'E', {"E+T", "T"}},
        {'T', {"T*F", "F"}},
        {'F', {"(E)", "id"}}
    };

    char startSymbol = 'S';
    vector<State> states = constructLR0Table(grammar, startSymbol);

    // Display the states and transitions
    cout << "LR(0) States and Transitions:\n";
    for (int i = 0; i < states.size(); i++) {
        cout << "State " << i << ":\n";
        for (const auto &item : states[i].items) {
            cout << "  " << item.left << " -> " << item.right.substr(0, item.dotPos) << "." 
                 << item.right.substr(item.dotPos) << "\n";
        }
        cout << "Transitions:\n";
        for (const auto &trans : states[i].transitions) {
            cout << "  " << trans.first << " -> State " << trans.second << "\n";
        }
        cout << "-----------------\n";
    }

    return 0;
}