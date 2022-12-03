#pragma once

#include <vector>
#include "types.h"

namespace piece {    
    class Piece;
}

namespace state {
    class State {
        piece::Piece *board[8][8];
        std::vector<piece::Piece> *pinned_pieces;
        std::vector<piece::Piece> *checking_pieces;
        // std::vector<piece::Piece> *white_pieces;
        // std::vector<piece::Piece> *black_pieces;
        Player turn;

        public:
            State();
            bool move(Position from, Position to);
            bool in_check();
            std::vector<piece::Piece> *get_checking_pieces();
            void print();
            piece::Piece* (*get_board())[8];
    };
}
