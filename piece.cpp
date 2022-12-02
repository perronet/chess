#include "piece.h"

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

Pawn::Pawn() {
    symbol = "p";
}

vector<int> Pawn::get_legal_moves() {
    vector<int> v;
    return v;
}
