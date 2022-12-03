#pragma once

#include "piece.h"
#include "types.h"
#include <vector>

namespace state {
    class Board {
        piece::Piece *state[8][8];
        std::vector<piece::Piece> *pinned_pieces;
        std::vector<piece::Piece> *checking_pieces;
        // std::vector<piece::Piece> *white_pieces;
        // std::vector<piece::Piece> *black_pieces;
        Player turn;

        public:
            Board();
            bool move(Position from, Position to);
            bool in_check();
            std::vector<piece::Piece> *get_checking_pieces();
            void print();
    };
}
