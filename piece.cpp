#include "types.h"
#include "piece.h"
#include "state.h"

using namespace piece;
using namespace std;

Piece::Piece() {}

Piece::Piece(Player p, Position pos) {
    this->player = p;
    this->pos = pos;
}

string Piece::get_symbol() {
    return symbol;
}

Player Piece::get_player() {
    return player;
}

Position Piece::get_pos() {
    return pos;
}

void Piece::set_pos(Position p) {
    pos = p;
    first_move = false;
}

bool Piece::is_empty() {
    return this->get_type() == piecetype::Empty;
}

bool Piece::is_pinned(const state::State& s) const {
    for (Piece *piece : s.get_pinned_pieces()) {
        if (piece == this)
            return true;
    }
    return false;
}

Empty::Empty() {
    symbol = "-";
    player = None;
}

piecetype::Piece Empty::get_type() {
    return piecetype::Empty;
}

vector<Position> Empty::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}

Pawn::Pawn(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♟︎";
    } else {
        symbol = "♙";
    }
}

piecetype::Piece Pawn::get_type() {
    return piecetype::Pawn;
}

vector<Position> Pawn::get_legal_moves(const state::State& s) {
    vector<Position> v;
    int i = this->pos.i;
    int j = this->pos.j;
    auto board = s.get_board();

    if (this->is_pinned(s))
        return v;

    if (this->player == White) {
        // Move
        if (board[i - 1][j]->is_empty())
            v.push_back(Position{i - 1, j});
        if (this->first_move && board[i - 2][j]->is_empty())
            v.push_back(Position{i - 2, j});

        // Capture
        if (s.check_capture(Position{i - 1, j - 1}))
            v.push_back(Position{i - 1, j - 1});
        if (s.check_capture(Position{i - 1, j + 1}))
            v.push_back(Position{i - 1, j + 1});
    } else if (this->player == Black) {
        // Move
        if (board[i + 1][j]->is_empty())
            v.push_back(Position{i + 1, j});
        if (this->first_move && board[i + 2][j]->is_empty())
            v.push_back(Position{i + 2, j});

        // Capture
        if (s.check_capture(Position{i + 1, j - 1}))
            v.push_back(Position{i + 1, j - 1});
        if (s.check_capture(Position{i + 1, j + 1}))
            v.push_back(Position{i + 1, j + 1});
    }

    return v;
}

Rook::Rook(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♜";
    } else {
        symbol = "♖";
    }
}

piecetype::Piece Rook::get_type() {
    return piecetype::Rook;
}

vector<Position> Rook::get_legal_moves(const state::State& s) {
    vector<Position> v;
    auto board = s.get_board();
    int pos_i = this->pos.i;
    int pos_j = this->pos.j;

    if (this->is_pinned(s))
        return v;

    for (int i = pos_i + 1; i < 7; ++i) {
        // TODO this will probably repeat a lot... Macro time?
        if (board[i][pos_j]->is_empty()) {
            v.push_back(Position{i, pos_j});
        } else {
            if (s.check_capture(Position{i, pos_j}))
                v.push_back(Position{i, pos_j});
            break;
        }
    }

    return v;
}

Knight::Knight(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♞";
    } else {
        symbol = "♘";
    }
}

piecetype::Piece Knight::get_type() {
    return piecetype::Knight;
}

vector<Position> Knight::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}

Bishop::Bishop(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♝";
    } else {
        symbol = "♗";
    }
}

piecetype::Piece Bishop::get_type() {
    return piecetype::Bishop;
}

vector<Position> Bishop::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}

Queen::Queen(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♛";
    } else {
        symbol = "♕";
    }
}

piecetype::Piece Queen::get_type() {
    return piecetype::Queen;
}

vector<Position> Queen::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}

King::King(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♚";
    } else {
        symbol = "♔";
    }
}

piecetype::Piece King::get_type() {
    return piecetype::King;
}

vector<Position> King::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}