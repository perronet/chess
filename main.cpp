#include <iostream>
#include <vector>
#include "state.h"
#include "notation.h"

using namespace std;

int main() {
    state::State s;
    string move_notation;

    s.print();
    cout << "White to play." << endl;

    while (true) {
        cin >> move_notation;

        if (s.move(move_notation)) {
            if (s.get_turn() == White)
                cout << "White to play." << endl;
            else
                cout << "Black to play." << endl;
            s.print();
        } else {
            cout << "Invalid move, please retry." << endl;
        }

        if (s.in_checkmate() || s.in_stalemate())
            break;

        if (s.in_check())
            cout << "Check!" << endl;
    }

    cout << "Game over." << endl;
}
