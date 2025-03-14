#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>

using namespace std;

struct Item {
    string left;
    string right;
    int dotPos;

    bool operator==(const Item &other) const {
        return left == other.left && right == other.right && dotPos == other.dotPos;
    }
};

struct State {
    vector<Item> items;
    map<char, int> transitions;
};

bool contains(const vector<Item> &items, const Item &item) {
    for (const auto &it : items)
        if (it == item)
            return true;
    return false;
}

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
                        Item newItem = {string(1, symbol), prod, 0};
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

vector<Item> GOTO(const vector<Item> &state, char symbol, const map<char, vector<string>> &grammar) {
    vector<Item> newState;
    for (const auto &item : state) {
        if (item.dotPos < item.right.size() && item.right[item.dotPos] == symbol) {
            newState.push_back({item.left, item.right, item.dotPos + 1});
        }
    }
    return closure(newState, grammar);
}

int findState(const vector<State> &states, const vector<Item> &items) {
    for (int i = 0; i < states.size(); ++i) {
        if (states[i].items == items)
            return i;
    }
    return -1;
}

vector<State> constructLR0Table(const map<char, vector<string>> &grammar, char startSymbol) {
    vector<State> states;
    vector<Item> startItems = closure({{"S'", string(1, startSymbol), 0}}, grammar);
    states.push_back({startItems, {}});

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < states.size(); ++i) {
            for (const auto &item : states[i].items) {
                if (item.dotPos < item.right.size()) {
                    char symbol = item.right[item.dotPos];
                    vector<Item> nextState = GOTO(states[i].items, symbol, grammar);
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

map<char, set<char>> computeFollow(const map<char, vector<string>> &grammar, char startSymbol) {
    map<char, set<char>> follow;
    for (const auto &rule : grammar) {
        follow[rule.first] = {};
    }
    follow[startSymbol].insert('$');

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto &rule : grammar) {
            char nonTerminal = rule.first;
            for (const string &production : rule.second) {
                for (size_t i = 0; i < production.size(); ++i) {
                    char symbol = production[i];
                    if (grammar.find(symbol) != grammar.end()) {
                        size_t j = i + 1;
                        while (j < production.size() && grammar.find(production[j]) == grammar.end()) {
                            follow[symbol].insert(production[j]);
                            break;
                        }
                        if (j == production.size() || grammar.find(production[j]) != grammar.end()) {
                            int prevSize = follow[symbol].size();
                            follow[symbol].insert(follow[nonTerminal].begin(), follow[nonTerminal].end());
                            if (follow[symbol].size() != prevSize)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
    return follow;
}

map<pair<int, char>, string> constructSLR1Table(const vector<State> &states, const map<char, vector<string>> &grammar, const map<char, set<char>> &follow) {
    map<pair<int, char>, string> table;

    for (int i = 0; i < states.size(); ++i) {
        for (const auto &item : states[i].items) {
            if (item.dotPos < item.right.size()) {
                char symbol = item.right[item.dotPos];
                if (states[i].transitions.find(symbol) != states[i].transitions.end()) {
                    table[{i, symbol}] = "S" + to_string(states[i].transitions.at(symbol));
                }
            } else if (item.left != "S'") {
                for (char terminal : follow.at(item.left[0])) {
                    table[{i, terminal}] = "R(" + item.left + " -> " + item.right + ")";
                }
            }
        }
        if (table.find({i, '$'}) == table.end()) {
            table[{i, '$'}] = "ACCEPT";
        }
    }
    return table;
}

bool parseSLR1(const vector<State> &states, const map<pair<int, char>, string> &parsingTable, const string &input) {
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
    map<char, set<char>> follow = computeFollow(grammar, startSymbol);
    vector<State> states = constructLR0Table(grammar, startSymbol);
    map<pair<int, char>, string> parsingTable = constructSLR1Table(states, grammar, follow);

    cout << "SLR(1) Parsing Table:\n";
    for (const auto &entry : parsingTable) {
        cout << "State " << entry.first.first << " on '" << entry.first.second << "' -> " << entry.second << "\n";
    }

    string input = "id+id*id$";
    parseSLR1(states, parsingTable, input);

    return 0;
}
