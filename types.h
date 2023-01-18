#pragma once

#define BOARD_SIZE 8

enum Player {White, Black, None};

namespace piecetype { // Avoid name clash with class
    enum Piece {Pawn, Rook, Knight, Bishop, Queen, King, Empty};
}

struct Position {
    int i;
    int j;

    bool operator==(const Position& rhs) const { 
        return i == rhs.i && j == rhs.j; 
    }

    bool operator!=(const Position& rhs) const { 
        return !(*this == rhs); 
    }

    bool check_bounds() const {
        return BOARD_SIZE > i && i >= 0 && BOARD_SIZE > j && j >= 0;
    }
};