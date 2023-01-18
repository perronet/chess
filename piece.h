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
            bool first_move = true;

        public:
            Piece();
            Piece(Player p, Position pos);
            virtual std::vector<Position> get_legal_moves(const state::State& s) const = 0;
            virtual piecetype::Piece get_type() const = 0;
            std::string get_symbol() const;
            Position get_pos() const;
            void set_pos(Position p);
            Player get_player() const;
            bool is_empty() const;
            bool is_pinned(const state::State& s) const;
    };

    class Empty: public Piece {
        public:
            Empty();
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class Pawn: public Piece {
        public:
            Pawn(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class Rook: public Piece {
        public:
            Rook(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class Knight: public Piece {
        public:
            Knight(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class Bishop: public Piece {
        public:
            Bishop(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class Queen: public Piece {
        public:
            Queen(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class King: public Piece {
        public:
            King(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };
}
