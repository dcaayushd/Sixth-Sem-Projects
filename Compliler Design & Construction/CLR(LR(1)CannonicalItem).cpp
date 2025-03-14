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

vector<State> constructCLRTable(const map<char, vector<string>> &grammar, char startSymbol, const map<char, set<char>> &first) {
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
                        states[i].transitions[symbol] = stateIndex;
                    }
                }
            }
        }
    }
    return states;
}

map<char, set<char>> computeFirst(const map<char, vector<string>> &grammar) {
    map<char, set<char>> first;
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto &rule : grammar) {
            char nonTerminal = rule.first;
            for (const string &production : rule.second) {
                char firstSymbol = production[0];
                if (!isupper(firstSymbol)) {
                    if (first[nonTerminal].insert(firstSymbol).second)
                        changed = true;
                } else {
                    size_t prevSize = first[nonTerminal].size();
                    first[nonTerminal].insert(first[firstSymbol].begin(), first[firstSymbol].end());
                    if (first[nonTerminal].size() != prevSize)
                        changed = true;
                }
            }
        }
    }
    return first;
}

map<pair<int, char>, string> constructCLR1Table(const vector<State> &states, const map<char, vector<string>> &grammar) {
    map<pair<int, char>, string> table;
    for (int i = 0; i < states.size(); ++i) {
        for (const auto &item : states[i].items) {
            if (item.dotPos < item.right.size()) {
                char symbol = item.right[item.dotPos];
                if (states[i].transitions.find(symbol) != states[i].transitions.end()) {
                    table[{i, symbol}] = "S" + to_string(states[i].transitions.at(symbol));
                }
            } else if (item.left != "S'") {
                table[{i, item.lookahead}] = "R(" + item.left + " -> " + item.right + ")";
            }
        }
        if (table.find({i, '$'}) == table.end()) {
            table[{i, '$'}] = "ACCEPT";
        }
    }
    return table;
}

bool parseCLR1(const vector<State> &states, const map<pair<int, char>, string> &parsingTable, const string &input) {
    stack<int> stateStack;
    stateStack.push(0);
    int pos = 0;

    while (true) {
        int currState = stateStack.top();
        char lookahead = input[pos];

        auto it = parsingTable.find({currState, lookahead});
        if (it == parsingTable.end()) {
            cout << "Parsing error at position " << pos << "!\n";
            return false;
        }

        string action = it->second;
        if (action[0] == 'S') {
            int newState = stoi(action.substr(1));
            stateStack.push(newState);
            pos++;
        } else if (action[0] == 'R') {
            cout << "Performing " << action << "\n";
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
    map<char, set<char>> first = computeFirst(grammar);
    vector<State> states = constructCLRTable(grammar, startSymbol, first);
    map<pair<int, char>, string> parsingTable = constructCLR1Table(states, grammar);

    cout << "CLR(1) Parsing Table:\n";
    for (const auto &entry : parsingTable) {
        cout << "State " << entry.first.first << " on '" << entry.first.second << "' -> " << entry.second << "\n";
    }

    string input = "id+id*id$";
    parseCLR1(states, parsingTable, input);

    return 0;
}
