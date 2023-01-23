#pragma once

#include <vector>
#include <cmath>
#include <string>

#define BOARD_SIZE 8

enum Player {White, Black, None};

namespace piecetype { // Avoid name clash with class
    enum Piece {Pawn, Rook, Knight, Bishop, Queen, King, Empty};
}

struct Position {
    int i;
    int j;

    bool operator==(const Position& other) const { 
        return this->i == other.i && this->j == other.j; 
    }

    std::string to_string() const {
        return std::string("(") + std::to_string(i) + ", " + std::to_string(j) + ")";
    }

    bool check_bounds() const {
        return BOARD_SIZE > i && i >= 0 && BOARD_SIZE > j && j >= 0;
    }

    /* Returns an exclusive range on both ends */
    std::vector<Position> range(Position other) {
        using namespace std;
        vector<Position> v;

        if (this->is_orthogonal_to(other)) {
            if (this->i == other.i) {
                for (int j = min(this->j, other.j) + 1; j < max(this->j, other.j); ++j)
                    v.push_back({this->i, j});
            } else {
                for (int i = min(this->i, other.i) + 1; i < max(this->i, other.i); ++i)
                    v.push_back({i, this->j});
            }
        } else if (this->is_diagonal_to(other)) {
            int i, j;

            if (this->i <= other.i && this->j <= other.j || this->i >= other.i && this->j >= other.j) {
                // i increasing, j increasing: top-left to bottom-right
                for (i = min(this->i, other.i) + 1, j = min(this->j, other.j) + 1; i < max(this->i, other.i) && j < max(this->j, other.j); ++i, ++j)
                    v.push_back({i, j});
            } else {
                // i decreasing, j increasing: bottom-left to top-right
                for (i = max(this->i, other.i) - 1, j = min(this->j, other.j) + 1; i > min(this->i, other.i) && j < max(this->j, other.j); --i, ++j)
                    v.push_back({i, j});
            }
        }

        return v;
    }

    bool is_orthogonal_to(Position other) {
        return this->i == other.i || this->j == other.j;
    }

    bool is_diagonal_to(Position other) {
        return std::abs(this->i - other.i) == std::abs(this->j - other.j);
    }
};

struct Move {
    Position from;
    Position to;
    bool is_capture;
    bool is_castle = false;
    Position castle_rook_from = {-1, -1};
    Position castle_rook_to = {-1, -1};

    public:
        Move(Position from, Position to, bool is_capture = false) {
            this->from = from;
            this->to = to;
            this->is_capture = is_capture;
        }
        
        void add_castle(Position rook_from, Position rook_to) {
            this->castle_rook_from = rook_from;
            this->castle_rook_to = rook_to;
            this->is_castle = true;
        }

        bool operator==(const Move& other) const {
            return this->from == other.from && this->to == other.to;
        }
};

struct MoveHash {
    size_t operator()(const Move& move) const {
        return move.to.i ^ move.to.j;
    }
};