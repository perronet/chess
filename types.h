#pragma once

enum Player {White, Black};

namespace piecetype { // Avoid name clash with class
    enum Piece {Pawn, Rook, Knight, Bishop, Queen, King, Empty};
}

class Position {
    public:
        int i;
        int j;

    bool operator==(const Position& rhs) const { 
        return i == rhs.i && j == rhs.j; 
    }

    bool operator!=(const Position& rhs) const { 
        return i == rhs.i && j == rhs.j; 
    }
};