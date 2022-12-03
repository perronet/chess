#include "piece.h"
#include "types.h"

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
}

bool Piece::is_empty() {
    return player == None;
}

Empty::Empty() {
    symbol = "-";
    this->player = None;
}

vector<Position> Empty::get_legal_moves() {
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

vector<Position> Pawn::get_legal_moves() {
    vector<Position> v;
    return v;
}

Rook::Rook(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♜";
    } else {
        symbol = "♖";
    }
}

vector<Position> Rook::get_legal_moves() {
    vector<Position> v;
    return v;
}

Knight::Knight(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♞";
    } else {
        symbol = "♘";
    }
}

vector<Position> Knight::get_legal_moves() {
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

vector<Position> Bishop::get_legal_moves() {
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

vector<Position> Queen::get_legal_moves() {
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

vector<Position> King::get_legal_moves() {
    vector<Position> v;
    return v;
}
