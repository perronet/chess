#include <iostream>
#include <vector>
#include "state.h"
#include "parse.h"

using namespace std;

int main() {
    state::State s;
    parse::MoveParser p;
    string move_notation;

    // Position test = p.coord_to_pos("h3").value_or(Position{0,0});
    // p.parse("Bh3", &s);

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
