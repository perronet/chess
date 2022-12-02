#pragma once

#include "piece.h"
#include "types.h"

namespace state {
    class Board {
        piece::Piece *state[8][8];
        Player turn;
        bool check;

        public:
            Board();

            bool move(std::pair<int, int> from, std::pair<int, int> to);

            void print();
    };
}
