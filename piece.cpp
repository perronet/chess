#include "types.h"
#include "piece.h"
#include "state.h"

using namespace piece;
using namespace std;

Piece::Piece() {}

Piece::Piece(Player p, Position pos) {
    this->player = p;
    this->pos = pos;
}

string Piece::get_symbol() {
    return symbol;
}

Player Piece::get_player() {
    return player;
}

Position Piece::get_pos() {
    return pos;
}

void Piece::set_pos(Position p) {
    pos = p;
    first_move = false;
}

bool Piece::is_empty() {
    return this->get_type() == piecetype::Empty;
}

bool Piece::is_pinned(const state::State& s) const {
    for (auto piece : s.get_pinned_pieces()) {
        if (piece == this)
            return true;
    }
    return false;
}

Empty::Empty() {
    symbol = "-";
    player = None;
}

piecetype::Piece Empty::get_type() {
    return piecetype::Empty;
}

vector<Position> Empty::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}

Pawn::Pawn(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♟︎";
    } else {
        symbol = "♙";
    }
}

piecetype::Piece Pawn::get_type() {
    return piecetype::Pawn;
}

vector<Position> Pawn::get_legal_moves(const state::State& s) {
    vector<Position> v;
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    if (this->is_pinned(s))
        return v;

    if (this->player == White) {
        // Move
        if (board(i - 1, j)->is_empty())
            v.push_back({i - 1, j});
        if (this->first_move && board(i - 2, j)->is_empty())
            v.push_back({i - 2, j});

        // Capture
        if (s.check_capture({i - 1, j - 1}))
            v.push_back({i - 1, j - 1});
        if (s.check_capture({i - 1, j + 1}))
            v.push_back({i - 1, j + 1});
    } else if (this->player == Black) {
        // Move
        if (board(i + 1,j)->is_empty())
            v.push_back({i + 1, j});
        if (this->first_move && board(i + 2, j)->is_empty())
            v.push_back({i + 2, j});

        // Capture
        if (s.check_capture({i + 1, j - 1}))
            v.push_back({i + 1, j - 1});
        if (s.check_capture({i + 1, j + 1}))
            v.push_back({i + 1, j + 1});
    }

    return v;
}

Rook::Rook(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♜";
    } else {
        symbol = "♖";
    }
}

piecetype::Piece Rook::get_type() {
    return piecetype::Rook;
}

#define CHECK_EMPTY_OR_CAPTURE(i, j) { \
    if (board(i, j)->is_empty()) { \
        v.push_back({i, j}); \
    } else { \
        if (s.check_capture({i, j})) \
            v.push_back({i, j}); \
        break; \
    } \
} \

vector<Position> Rook::get_legal_moves(const state::State& s) {
    vector<Position> v;
    state::Board board = s.get_board();
    int pos_i = this->pos.i;
    int pos_j = this->pos.j;

    if (this->is_pinned(s))
        return v;

    for (int i = pos_i + 1; i < BOARD_SIZE; ++i) {
        CHECK_EMPTY_OR_CAPTURE(i, pos_j);
    }
    for (int i = pos_i - 1; i >= 0; --i) {
        CHECK_EMPTY_OR_CAPTURE(i, pos_j);
    }
    for (int j = pos_j + 1; j < BOARD_SIZE; ++j) {
        CHECK_EMPTY_OR_CAPTURE(pos_i, j);
    }
    for (int j = pos_j - 1; j >= 0; --j) {
        CHECK_EMPTY_OR_CAPTURE(pos_i, j);
    }

    return v;
}

Knight::Knight(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♞";
    } else {
        symbol = "♘";
    }
}

piecetype::Piece Knight::get_type() {
    return piecetype::Knight;
}

vector<Position> Knight::get_legal_moves(const state::State& s) {
    vector<Position> v;
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    if (this->is_pinned(s))
        return v;

    vector<Position> v_check{
        {i+1, j+2}, {i+2, j+1},
        {i-1, j-2}, {i-2, j-1},
        {i-1, j+2}, {i-2, j+1},
        {i+1, j-2}, {i+2, j-1},
    };
    for (Position p : v_check) {
        if (notation::check_range(p)) {
            if (board[p]->is_empty()) {
                v.push_back(p);
            } else {
                if (s.check_capture(p))
                    v.push_back(p);
            }
        }
    }

    return v;
}

Bishop::Bishop(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♝";
    } else {
        symbol = "♗";
    }
}

piecetype::Piece Bishop::get_type() {
    return piecetype::Bishop;
}

vector<Position> Bishop::get_legal_moves(const state::State& s) {
    vector<Position> v;
    state::Board board = s.get_board();
    int pos_i = this->pos.i;
    int pos_j = this->pos.j;

    if (this->is_pinned(s))
        return v;

    int i, j;
    for (i = pos_i + 1, j = pos_j + 1; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j) {
        CHECK_EMPTY_OR_CAPTURE(i, j);
    }
    for (i = pos_i + 1, j = pos_j - 1; i < BOARD_SIZE && j >= 0; ++i, --j) {
        CHECK_EMPTY_OR_CAPTURE(i, j);
    }
    for (i = pos_i - 1, j = pos_j + 1; i >= 0 && j < BOARD_SIZE; --i, ++j) {
        CHECK_EMPTY_OR_CAPTURE(i, j);
    }
    for (i = pos_i - 1, j = pos_j - 1; i >= 0 && j >= 0; --i, --j) {
        CHECK_EMPTY_OR_CAPTURE(i, j);
    }

    return v;
}

Queen::Queen(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♛";
    } else {
        symbol = "♕";
    }
}

piecetype::Piece Queen::get_type() {
    return piecetype::Queen;
}

vector<Position> Queen::get_legal_moves(const state::State& s) {
    vector<Position> v;
    Rook rook(this->player, this->pos);
    Bishop bishop(this->player, this->pos);

    if (this->is_pinned(s))
        return v;

    v = rook.get_legal_moves(s);
    vector<Position> v_bishop = bishop.get_legal_moves(s);
    v.insert(v.end(), v_bishop.begin(), v_bishop.end());
    return v;
}

King::King(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White) {
        symbol = "♚";
    } else {
        symbol = "♔";
    }
}

piecetype::Piece King::get_type() {
    return piecetype::King;
}

vector<Position> King::get_legal_moves(const state::State& s) {
    vector<Position> v;
    return v;
}