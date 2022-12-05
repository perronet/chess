#pragma once

#include <vector>
#include "types.h"
#include "piece.h"
#include "parse.h"

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
        piece::Piece *board[8][8];
        std::vector<piece::Piece*> pinned_pieces;
        std::vector<piece::Piece*> checking_pieces;
        parse::MoveParser move_parser;
        Material white_pieces;
        Material black_pieces;
        Player turn;

        public:
            State();
            bool move(Position from, Position to);
            bool move(std::string move_notation);
            bool in_check();
            std::vector<piece::Piece*> get_pinned_pieces();
            std::vector<piece::Piece*> get_checking_pieces();
            void print();
            piece::Piece* (*get_board())[8];
            Material *get_pieces(Player p);
            piece::King *get_king(Player p);
            Player get_turn();
            bool check_capture(Position pos);

        private:
            void add_piece(Player p, piecetype::Piece piece, Position pos);
            void remove_piece(Position pos);
    };
}
