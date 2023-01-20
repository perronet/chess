#pragma once

#include <vector>
#include <iostream>
#include <optional>
#include <memory>
#include <algorithm>
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
            virtual std::vector<Position> get_legal_moves_pinned(const state::State& s, const Piece* pinner) const;
            virtual piecetype::Piece get_type() const = 0;
            std::string get_symbol() const;
            Position get_pos() const;
            void set_pos(Position p);
            Player get_player() const;
            bool is_first_move() const;
            bool is_empty() const;
            std::optional<const Piece*> check_pinned(const state::State& s) const; // Returns the pinner if the piece is pinned

            static std::unique_ptr<Piece> get_piece_by_type(piecetype::Piece typ, Player p, Position pos);
    };

    class Empty: public Piece {
        public:
            Empty();
            Empty(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
    };

    class Pawn: public Piece {
        public:
            Pawn(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;
            std::vector<Position> get_legal_moves_pinned(const state::State& s, const Piece* pinner) const override;
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
        friend class state::State;

        public:
            King(Player p, Position pos);
            piecetype::Piece get_type() const;
            std::vector<Position> get_legal_moves(const state::State& s) const;

        private:
            /* King moves without accounting for squares under attack. Some of these moves might be illegal. */
            std::vector<Position> get_moves_unrestricted(const state::State& s) const;
    };
}
