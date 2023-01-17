#pragma once

#include <vector>
#include <array>
#include "types.h"
#include "piece.h"
#include "notation.h"

namespace state {
    class Board {
        piece::Piece* data[BOARD_SIZE*BOARD_SIZE];
        
        public:
            piece::Piece*& operator()(size_t i, size_t j);
            piece::Piece* operator()(size_t i, size_t j) const;
            piece::Piece*& operator[](Position p);
            piece::Piece* operator[](Position p) const;
    };

    struct Material {
        Player player;
        std::vector<const piece::Piece*> pieces;
        const piece::King* king;
    };

    class State {
        Board board;
        std::vector<const piece::Piece*> pinned_pieces;
        std::vector<const piece::Piece*> checking_pieces;
        Material white_pieces;
        Material black_pieces;
        Player turn;

        public:
            State();
            bool move(Position from, Position to);
            bool move(std::string move_notation);
            bool in_check() const;
            std::vector<const piece::Piece*> get_pinned_pieces() const;
            std::vector<const piece::Piece*> get_checking_pieces() const;
            void print() const;
            const Board& get_board() const;
            const Material& get_pieces(Player p) const;
            const piece::King* get_king(Player p) const;
            Player get_turn() const;
            bool check_capture(Position pos) const;

        private:
            void add_piece(Player p, piecetype::Piece piece, Position pos);
            void add_empty(Position pos);
            void remove_piece(Position pos);
    };
}
