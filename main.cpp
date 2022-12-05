#include <iostream>
#include <vector>
#include "state.h"
#include "parse.h"

using namespace std;

int main() {
    state::State s;
    parse::MoveParser p;
    string move_notation;

    s.print();
    cout << "White to play.\n";

    while (true) {
        cin >> move_notation;

        if (s.move(move_notation)) {
            if (s.get_turn() == White)
                cout << "White to play.\n";
            else
                cout << "Black to play.\n";
            s.print();
        } else {
            cout << "Invalid move, please retry.\n";
        }
    }

    return 0;
}
