#include "piece.h"
#include "types.h"

using namespace piece;
using namespace std;

string Piece::get_symbol() {
    return symbol;
}

Empty::Empty() {
    symbol = "-";
}

vector<int> Empty::get_legal_moves() {
    vector<int> v;
    return v;
}

Pawn::Pawn(Player p) {
    this->player = p;
    if (p == White) {
        symbol = "♟︎";
    } else {
        symbol = "♙";
    }
}

vector<int> Pawn::get_legal_moves() {
    vector<int> v;
    return v;
}

Rook::Rook(Player p) {
    this->player = p;
    if (p == White) {
        symbol = "♜";
    } else {
        symbol = "♖";
    }
}

vector<int> Rook::get_legal_moves() {
    vector<int> v;
    return v;
}

Knight::Knight(Player p) {
    this->player = p;
    if (p == White) {
        symbol = "♞";
    } else {
        symbol = "♘";
    }
}

vector<int> Knight::get_legal_moves() {
    vector<int> v;
    return v;
}

Bishop::Bishop(Player p) {
    this->player = p;
    if (p == White) {
        symbol = "♝";
    } else {
        symbol = "♗";
    }
}

vector<int> Bishop::get_legal_moves() {
    vector<int> v;
    return v;
}

Queen::Queen(Player p) {
    this->player = p;
    if (p == White) {
        symbol = "♛";
    } else {
        symbol = "♕";
    }
}

vector<int> Queen::get_legal_moves() {
    vector<int> v;
    return v;
}

King::King(Player p) {
    this->player = p;
    if (p == White) {
        symbol = "♚";
    } else {
        symbol = "♔";
    }
}

vector<int> King::get_legal_moves() {
    vector<int> v;
    return v;
}
