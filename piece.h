#pragma once

#include <vector>
#include <iostream>
#include "types.h"

namespace state {
    class State;
}

namespace piece {    
    class Piece {
        protected:
            std::string symbol;
            Player player;
            Position pos;
            bool is_pinned = false;
            bool first_move = true;

        public:
            Piece();
            Piece(Player p, Position pos);
            virtual std::vector<Position> get_legal_moves(state::State *b) = 0;
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
            std::vector<Position> get_legal_moves(state::State *b);
    };

    class Pawn: public Piece {
        public:
            Pawn(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves(state::State *b);
    };

    class Rook: public Piece {
        public:
            Rook(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves(state::State *b);
    };

    class Knight: public Piece {
        public:
            Knight(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves(state::State *b);
    };

    class Bishop: public Piece {
        public:
            Bishop(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves(state::State *b);
    };

    class Queen: public Piece {
        public:
            Queen(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves(state::State *b);
    };

    class King: public Piece {
        public:
            King(Player p, Position pos);
            piecetype::Piece get_type();
            std::vector<Position> get_legal_moves(state::State *b);
    };
}
