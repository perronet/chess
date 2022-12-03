#include <iostream>
#include <algorithm>
#include <vector>
#include "state.h"
#include "piece.h"
#include "types.h"

using namespace state;
using namespace piece;
using namespace std;

Board::Board() {
    this->turn = White;
    for (int i = 1; i < 7; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i == 1) {
                state[i][j] = new Pawn(Black, Position {i, j});
            } else if (i == 6) {
                state[i][j] = new Pawn(White, Position {i, j});
            } else {
                state[i][j] = new Empty();
            }
        }
    }
    Piece *start_black[] = {new Rook(Black, Position {0, 0}), new Knight(Black, Position {0, 1}), new Bishop(Black, Position {0, 2}), 
    new Queen(Black, Position {0, 3}), new King(Black, Position {0, 4}), new Bishop(Black, Position {0, 5}),
    new Knight(Black, Position {0, 6}), new Rook(Black, Position {0, 7})};
    Piece *start_white[] = {new Rook(White, Position {7, 0}), new Knight(White, Position {7, 1}), new Bishop(White, Position {7, 2}), 
    new Queen(White, Position {7, 3}), new King(White, Position {7, 4}), new Bishop(White, Position {7, 5}),
    new Knight(White, Position {7, 6}), new Rook(White, Position {7, 7})};
    for (int j = 0; j < 8; ++j) {
        state[0][j] = start_black[j];
    }
    for (int j = 0; j < 8; ++j) {
        state[7][j] = start_white[j];
    }
}

bool Board::move(Position from, Position to) {
    if (state[from.i][from.j]->is_empty()
        || state[to.i][to.j]->get_player() == this->turn) {
        return false;
    }

    vector<Position> moves = state[from.i][from.j]->get_legal_moves();
    for (auto m : moves) {
        if (m == to) {
            // TODO check for pins
            // TODO check for checks
            state[to.i][to.j] = state[from.i][from.j];
            state[from.i][from.j] = new Empty();
            return true;
        }
            return true;
    }
    return false;
}

bool Board::in_check() {
    return this->checking_pieces->empty();
}

vector<piece::Piece> *Board::get_checking_pieces() {
    return this->checking_pieces;
}

void Board::print() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << state[i][j]->get_symbol() << " ";
        }
        cout << "\n";
    }
}
