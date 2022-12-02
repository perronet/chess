#include <iostream>
#include "state.h"
#include "piece.h"
#include "types.h"

using namespace state;
using namespace piece;
using namespace std;

Board::Board() {
    turn = White;
    check = false;
    for (int i = 1; i < 7; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i == 1) {
                state[i][j] = new Pawn(Black);
            } else if (i == 6) {
                state[i][j] = new Pawn(White);
            } else {
                state[i][j] = new Empty();
            }
        }
    }
    Piece *start_black[] = {new Rook(Black), new Knight(Black), new Bishop(Black), new Queen(Black), new King(Black), new Bishop(Black), new Knight(Black), new Rook(Black)};
    Piece *start_white[] = {new Rook(White), new Knight(White), new Bishop(White), new Queen(White), new King(White), new Bishop(White), new Knight(White), new Rook(White)};
    for (int j = 0; j < 8; ++j) {
        state[0][j] = start_black[j];
    }
    for (int j = 0; j < 8; ++j) {
        state[7][j] = start_white[j];
    }
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
