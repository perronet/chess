#pragma once

#include <vector>
#include <iostream>

namespace piece {
    class Piece {
        protected:
            std::string symbol;

        public:
            virtual std::vector<int> get_legal_moves() = 0;
            std::string get_symbol();
    };

    class Empty: public Piece {
        public:
            Empty();
            std::vector<int> get_legal_moves();
    };

    class Pawn: public Piece {
        public:
            Pawn();
            std::vector<int> get_legal_moves();
    };
}
