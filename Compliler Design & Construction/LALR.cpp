#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>

using namespace std;

struct Item {
    string left;
    string right;
    int dotPos;
    char lookahead;

    bool operator==(const Item &other) const {
        return left == other.left && right == other.right && dotPos == other.dotPos && lookahead == other.lookahead;
    }
};

struct State {
    vector<Item> items;
    map<char, int> transitions;
};

bool contains(const vector<Item> &items, const Item &item) {
    return find(items.begin(), items.end(), item) != items.end();
}

vector<Item> closure(vector<Item> items, const map<char, vector<string>> &grammar, const map<char, set<char>> &first) {
    bool changed = true;
    while (changed) {
        changed = false;
        vector<Item> newItems = items;
        for (const auto &item : items) {
            if (item.dotPos < item.right.size()) {
                char symbol = item.right[item.dotPos];
                if (grammar.find(symbol) != grammar.end()) {
                    set<char> lookaheadSet;
                    if (item.dotPos + 1 < item.right.size()) {
                        char nextSymbol = item.right[item.dotPos + 1];
                        lookaheadSet = first.at(nextSymbol);
                    } else {
                        lookaheadSet = {item.lookahead};
                    }
                    for (char lookahead : lookaheadSet) {
                        for (const string &prod : grammar.at(symbol)) {
                            Item newItem = {string(1, symbol), prod, 0, lookahead};
                            if (!contains(newItems, newItem)) {
                                newItems.push_back(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        items = newItems;
    }
    return items;
}

vector<Item> GOTO(const vector<Item> &state, char symbol, const map<char, vector<string>> &grammar, const map<char, set<char>> &first) {
    vector<Item> newState;
    for (const auto &item : state) {
        if (item.dotPos < item.right.size() && item.right[item.dotPos] == symbol) {
            newState.push_back({item.left, item.right, item.dotPos + 1, item.lookahead});
        }
    }
    return closure(newState, grammar, first);
}

int findState(const vector<State> &states, const vector<Item> &items) {
    for (int i = 0; i < states.size(); ++i) {
        if (states[i].items == items)
            return i;
    }
    return -1;
}

vector<State> constructLALRTable(const map<char, vector<string>> &grammar, char startSymbol, const map<char, set<char>> &first) {
    vector<State> states;
    vector<Item> startItems = closure({{"S'", string(1, startSymbol), 0, '$'}}, grammar, first);
    states.push_back({startItems, {}});

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < states.size(); ++i) {
            for (const auto &item : states[i].items) {
                if (item.dotPos < item.right.size()) {
                    char symbol = item.right[item.dotPos];
                    vector<Item> nextState = GOTO(states[i].items, symbol, grammar, first);
                    if (!nextState.empty()) {
                        int stateIndex = findState(states, nextState);
                        if (stateIndex == -1) {
                            stateIndex = states.size();
                            states.push_back({nextState, {}});
                            changed = true;
                        }
                        states[i].transitions[symbol] = static_cast<unsigned char>(stateIndex);  // cast to avoid narrowing
                    }
                }
            }
        }
    }

    // Merge states with the same LR(0) core to construct LALR(1) table
    vector<State> lalrStates;
    map<int, int> stateMap; // Maps old state index to new merged index

    for (int i = 0; i < states.size(); ++i) {
        bool merged = false;
        for (int j = 0; j < lalrStates.size(); ++j) {
            // Check if the LR(0) core is the same (ignoring lookaheads)
            vector<Item> core1, core2;
            for (const auto &item : states[i].items) core1.push_back({item.left, item.right, item.dotPos, ' '});
            for (const auto &item : lalrStates[j].items) core2.push_back({item.left, item.right, item.dotPos, ' '});
            sort(core1.begin(), core1.end());
            sort(core2.begin(), core2.end());

            if (core1 == core2) {
                for (const auto &item : states[i].items) {
                    if (!contains(lalrStates[j].items, item)) {
                        lalrStates[j].items.push_back(item);
                    }
                }
                stateMap[i] = j;
                merged = true;
                break;
            }
        }
        if (!merged) {
            stateMap[i] = lalrStates.size();
            lalrStates.push_back(states[i]);
        }
    }

    // Update transitions based on merged states
    for (auto &state : lalrStates) {
        map<char, int> newTransitions;
        for (const auto &t : state.transitions) {
            newTransitions[t.first] = stateMap[t.second];
        }
        state.transitions = newTransitions;
    }

    return lalrStates;
}

void printLALRTable(const vector<State> &states) {
    cout << "\nLALR(1) Parsing Table:\n";
    for (int i = 0; i < states.size(); ++i) {
        cout << "State " << i << ":\n";
        for (const auto &item : states[i].items) {
            cout << "  " << item.left << " -> " << item.right.substr(0, item.dotPos) << "." 
                 << item.right.substr(item.dotPos) << ", " << item.lookahead << "\n";
        }
        for (const auto &t : states[i].transitions) {
            cout << "  On '" << t.first << "' -> State " << t.second << "\n";
        }
        cout << "----------------------\n";
    }
}

int main() {
    map<char, vector<string>> grammar = {
        {'S', {"AB"}},
        {'A', {"aA", "ε"}},  // ε will now be represented as '\0'
        {'B', {"bB", "ε"}}   // ε will now be represented as '\0'
    };

    map<char, set<char>> first = {
        {'S', {'a', 'b'}},
        {'A', {'a', '\0'}},  // ε represented as '\0'
        {'B', {'b', '\0'}}   // ε represented as '\0'
    };

    char startSymbol = 'S';
    vector<State> lalrTable = constructLALRTable(grammar, startSymbol, first);

    printLALRTable(lalrTable);

    return 0;
}
