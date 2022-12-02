#include <iostream>
#include "state.h"
#include "piece.h"

using namespace state;
using namespace piece;
using namespace std;

Board::Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Empty *p = new Empty();
            state[i][j] = p;
        }
    }
    turn = White;
    check = false;
}

bool Board::move(pair<int, int> from, pair<int, int> to) {
    return true;
}

void Board::print() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << state[i][j]->get_symbol() << " ";
        }
        cout << "\n";
    }
}
