#pragma once

#include <vector>
#include <array>
#include "types.h"
#include "piece.h"
#include "notation.h"

namespace state {
    struct Material {
        Player player;
        std::vector<piece::Pawn*> pawn;
        std::vector<piece::Rook*> rook;
        std::vector<piece::Knight*> knight;
        std::vector<piece::Bishop*> bishop;
        std::vector<piece::Queen*> queen;
        piece::King* king;
    };

    class State {
        typedef std::array<std::array<piece::Piece*, 8>, 8> board_t;

        board_t board;
        std::vector<piece::Piece*> pinned_pieces;
        std::vector<piece::Piece*> checking_pieces;
        Material white_pieces;
        Material black_pieces;
        Player turn;

        public:
            State();
            bool move(Position from, Position to);
            bool move(std::string move_notation);
            bool in_check() const;
            std::vector<piece::Piece*> get_pinned_pieces() const;
            std::vector<piece::Piece*> get_checking_pieces() const;
            void print() const;
            const board_t& get_board() const;
            const Material& get_pieces(Player p) const;
            piece::King *get_king(Player p) const;
            Player get_turn() const;
            bool check_capture(Position pos) const;

        private:
            void add_piece(Player p, piecetype::Piece piece, Position pos);
            void remove_piece(Position pos);
    };
}
