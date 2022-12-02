#pragma once

#include <vector>
#include <iostream>
#include "types.h"

namespace piece {
    class Piece {
        protected:
            std::string symbol;
            Player player;

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
            Pawn(Player p);
            std::vector<int> get_legal_moves();
    };

    class Rook: public Piece {
        public:
            Rook(Player p);
            std::vector<int> get_legal_moves();
    };

    class Knight: public Piece {
        public:
            Knight(Player p);
            std::vector<int> get_legal_moves();
    };

    class Bishop: public Piece {
        public:
            Bishop(Player p);
            std::vector<int> get_legal_moves();
    };

    class Queen: public Piece {
        public:
            Queen(Player p);
            std::vector<int> get_legal_moves();
    };

    class King: public Piece {
        public:
            King(Player p);
            std::vector<int> get_legal_moves();
    };
}
