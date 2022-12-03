#include <iostream>
#include <algorithm>
#include <vector>
#include "types.h"
#include "piece.h"
#include "state.h"

using namespace state;
using namespace piece;
using namespace std;

State::State() {
    this->turn = White;
    for (int i = 1; i < 7; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i == 1) {
                board[i][j] = new Pawn(Black, Position {i, j});
            } else if (i == 6) {
                board[i][j] = new Pawn(White, Position {i, j});
            } else {
                board[i][j] = new Empty();
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
        board[0][j] = start_black[j];
    }
    for (int j = 0; j < 8; ++j) {
        board[7][j] = start_white[j];
    }
}

bool State::move(Position from, Position to) {
    if (board[from.i][from.j]->is_empty()
        || board[to.i][to.j]->get_player() == this->turn) {
        return false;
    }

    vector<Position> moves = board[from.i][from.j]->get_legal_moves(this);
    for (auto m : moves) {
        if (m == to) {
            board[from.i][from.j]->set_pos(to);
            board[to.i][to.j] = board[from.i][from.j];
            board[from.i][from.j] = new Empty();
            return true;
        }
    }
    return false;
}

bool State::in_check() {
    return this->checking_pieces->empty();
}

vector<piece::Piece> *State::get_checking_pieces() {
    return this->checking_pieces;
}

void State::print() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j]->get_symbol() << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

piece::Piece* (*State::get_board())[8] { 
    return this->board; 
}
