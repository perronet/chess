#include <iostream>
#include <algorithm>
#include <vector>
#include "types.h"
#include "piece.h"
#include "state.h"

using namespace state;
using namespace piece;
using namespace std;

piece::Piece*& Board::operator()(size_t i, size_t j) {
    return this->data[i*BOARD_SIZE + j];
}

piece::Piece* Board::operator()(size_t i, size_t j) const {
    return this->data[i*BOARD_SIZE + j];
}

piece::Piece*& Board::operator[](Position p) {
    return this->operator()(p.i, p.j);
}

piece::Piece* Board::operator[](Position p) const {
    return this->operator()(p.i, p.j);
}

State::State() {
    this->turn = White;
    this->white_pieces.player = White;
    this->black_pieces.player = Black;
    for (int i = 1; i < BOARD_SIZE-1; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (i == 1) {
                this->add_piece(Black, piecetype::Pawn, {i, j});
            } else if (i == BOARD_SIZE-2) {
                this->add_piece(White, piecetype::Pawn, {i, j});
            } else {
                this->add_empty({i, j});
            }
        }
    }
    
    piecetype::Piece order[] = {piecetype::Rook, piecetype::Knight, piecetype::Bishop, piecetype::Queen, piecetype::King,
                                piecetype::Bishop, piecetype::Knight, piecetype::Rook};
    for (Player player : {White, Black}) {
        int row;
        if (player == Black)
            row = 0;
        if (player == White)
            row = BOARD_SIZE-1;
        for (int j = 0; j < BOARD_SIZE; ++j)
            this->add_piece(player, order[j], {row, j});
    }
}

bool State::move(string move_notation) {
    optional<pair<Position, Position>> result = notation::parse(move_notation, *this);

    if (!result.has_value())
        return false;

    return this->move(result.value().first, result.value().second);
}

bool State::move(Position from, Position to) {
    if (board[from]->is_empty()
        || board[from]->get_player() != this->turn
        || board[to]->get_player() == this->turn) {
        return false;
    }

    vector<Position> moves = board[from]->get_legal_moves(*this);
    for (auto m : moves) {
        if (m == to) {
            this->remove_piece(to);
            board[from]->set_pos(to);
            board[to] = board[from];
            this->add_empty(from);

            this->turn = (Player)!(bool)this->get_turn();
            return true;
        }
    }
    return false;
}

bool State::in_check() const {
    return this->checking_pieces.empty();
}

vector<const piece::Piece*> State::get_checking_pieces() const {
    return this->checking_pieces;
}

vector<const piece::Piece*> State::get_pinned_pieces() const {
    return this->pinned_pieces;
}

void State::print() const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << notation::row_to_coord(i) << "  ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            cout << board(i, j)->get_symbol() << " ";
        }
        cout << "\n";
    }
    cout << "   ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
        cout << notation::col_to_coord(j) << " ";
    }
    cout << "\n\n";
}

const state::Board& State::get_board() const { 
    return this->board; 
}

const Material& State::get_pieces(Player p) const {
    if (p == White)
        return this->white_pieces;
    else
        return this->black_pieces;
}

const King* State::get_king(Player p) const {
    if (p == White)
        return this->white_pieces.king;
    else
        return this->black_pieces.king;
}

Player State::get_turn() const {
    return this->turn;
}

#define CASE_PIECE_ADD(piececlass) \
    case piecetype::piececlass: { \
        piececlass *new_piece = new piececlass(p, pos); \
        this->board[pos] = new_piece; \
        if (p == White) \
            this->white_pieces.pieces.push_back(new_piece); \
        else \
            this->black_pieces.pieces.push_back(new_piece); \
        break; } \

void State::add_piece(Player p, piecetype::Piece piece, Position pos) {
    int i = pos.i;
    int j = pos.j;
    switch (piece) {
        CASE_PIECE_ADD(Pawn);
        CASE_PIECE_ADD(Rook);
        CASE_PIECE_ADD(Knight);
        CASE_PIECE_ADD(Bishop);
        CASE_PIECE_ADD(Queen);
        case piecetype::King: {
            King *new_piece = new King(p, pos);
            this->board[pos] = new_piece;
            if (p == White)
                this->white_pieces.king = new_piece;
            else
                this->black_pieces.king = new_piece;
            break; }
        default:
            break;
    }
}

void State::add_empty(Position pos) {
    board[pos] = new Empty();
}

/*
    Cleanup references from: board, material, pinned_pieces, checking_pieces
*/
void State::remove_piece(Position pos) {
    Piece* to_remove = this->board[pos];
    piecetype::Piece piece = to_remove->get_type();
    Player p = to_remove->get_player();

    if (piece != piecetype::King) {
        this->board[pos] = nullptr;
        this->pinned_pieces.erase(
            remove(this->pinned_pieces.begin(), this->pinned_pieces.end(), to_remove),
            this->pinned_pieces.end()
        );
        this->checking_pieces.erase(
            remove(this->checking_pieces.begin(), this->checking_pieces.end(), to_remove),
            this->checking_pieces.end()
        );

        if (p == White) {
            this->white_pieces.pieces.erase(
                remove(this->white_pieces.pieces.begin(), this->white_pieces.pieces.end(), to_remove),
                this->white_pieces.pieces.end()
            );
        } else {
            this->black_pieces.pieces.erase(
                remove(this->black_pieces.pieces.begin(), this->black_pieces.pieces.end(), to_remove),
                this->black_pieces.pieces.end()
            );
        }
       
        delete to_remove;
    }
}

bool State::check_capture(Position pos) const {
    return BOARD_SIZE > pos.i && pos.i >= 0 && BOARD_SIZE > pos.j && pos.j >= 0 &&
    board[pos]->get_player() == !(bool)this->get_turn() &&
    board[pos]->get_type() != piecetype::King;
}