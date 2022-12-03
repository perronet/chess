#pragma once

#include <vector>
#include <iostream>
#include "types.h"

namespace piece {
    class Piece {
        protected:
            std::string symbol;
            Player player;
            Position pos;
            bool is_pinned = false;

        public:
            Piece();
            Piece(Player p, Position pos);
            virtual std::vector<Position> get_legal_moves() = 0;
            virtual piecetype::Piece get_type() = 0;
            std::string get_symbol();
            Position get_pos();
            void set_pos(Position p);
            Player get_player();
            bool is_empty();
    };

    class Empty: public Piece {
        public:
            Empty();
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };

    class Pawn: public Piece {
        bool first_move = true;

        public:
            Pawn(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };

    class Rook: public Piece {
        bool first_move = true;

        public:
            Rook(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };

    class Knight: public Piece {
        public:
            Knight(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };

    class Bishop: public Piece {
        public:
            Bishop(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };

    class Queen: public Piece {
        public:
            Queen(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };

    class King: public Piece {
        bool first_move = true;

        public:
            King(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves();
    };
}
