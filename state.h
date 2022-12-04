#pragma once

#include <vector>
#include "types.h"
#include "piece.h"

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
        Material white_pieces;
        Material black_pieces;
        Player turn;

        public:
            State();
            bool move(Position from, Position to);
            bool in_check();
            std::vector<piece::Piece*> get_checking_pieces();
            void print();
            void add_piece(Player p, piecetype::Piece piece, Position pos);
            piece::Piece* (*get_board())[8];
            Material *get_pieces(Player p);
            Material *get_pieces();
    };
}
