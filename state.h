#pragma once

#include <vector>
#include <array>
#include <cassert>
#include <unordered_map>
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
            bool operator==(const Board& other) const;
    };

    struct Material {
        Player player;
        std::vector<const piece::Piece*> pieces;
        const piece::King* king;
    };

    struct StateHash {
        size_t operator()(const State& s) const {
            return 0;
        }
    };

    class State {
        friend class piece::King;

        Board board;
        std::vector<Move> curr_legal_moves;
        std::vector<std::pair<const piece::Piece*, const piece::Piece*>> pinned_pieces; // (pinner, pinned)
        std::vector<const piece::Piece*> checking_pieces;

        Material white_pieces;
        Material black_pieces;
        Player turn;
        GameState game_state;
        int move_cnt = 0;

        /* Data to compute draws */
        std::unordered_map<std::string, int> occurred_state_freq;
        int draw_moves_cnt = 0;
        bool draw_offered = false;

        public:
            State();
            bool move(Move input_move);
            bool move(std::string move_notation);

            /* Checks */
            bool in_check() const;
            bool in_blockable_check() const;
            bool in_double_check() const;
            bool is_square_attacked(Position pos, Player p) const;
            bool game_ended() const;
            bool check_capture(Position pos) const;

            /* Getters */
            int get_move_count() const;
            GameState get_game_state() const;
            std::vector<Move> get_legal_moves() const;
            std::vector<std::pair<const piece::Piece*, const piece::Piece*>> get_pinned_pieces() const;
            std::vector<const piece::Piece*> get_checking_pieces() const;
            const Board& get_board() const;
            const Material& get_pieces(Player p) const;
            const std::vector<const piece::Piece*> get_pieces(Player p, piecetype::Piece typ) const;
            const piece::King* get_king() const;
            const piece::King* get_opponent_king() const;
            Player get_turn() const;

            /* Utility */
            bool operator==(const State& other) const;
            std::string to_string() const;
            void print() const;

        private:
            void add_piece(Player p, piecetype::Piece piece, Position pos);
            void add_empty(Position pos);
            void remove_piece(Position pos);
            void move_piece(Position from, Position to);
            void update_pins_and_checks();
            void update_game_state(Move& move);
            void compute_legal_moves();
    };
}
