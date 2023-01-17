#include <iostream>
#include <algorithm>
#include <vector>
#include "types.h"
#include "piece.h"
#include "state.h"

using namespace state;
using namespace piece;
using namespace std;

State::State() {
    this->turn = White;
    this->white_pieces.player = White;
    this->black_pieces.player = Black;
    for (int i = 1; i < 7; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i == 1) {
                this->add_piece(Black, piecetype::Pawn, Position {i, j});
            } else if (i == 6) {
                this->add_piece(White, piecetype::Pawn, Position {i, j});
            } else {
                this->add_empty(Position {i, j});
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
            row = 7;
        for (int j = 0; j < 8; ++j)
            this->add_piece(player, order[j], Position {row, j});
    }
}

bool State::move(string move_notation) {
    optional<pair<Position, Position>> result = notation::parse(move_notation, *this);

    if (!result.has_value())
        return false;

    return this->move(result.value().first, result.value().second);
}

bool State::move(Position from, Position to) {
    if (board[from.i][from.j]->is_empty()
        || board[from.i][from.j]->get_player() != this->turn
        || board[to.i][to.j]->get_player() == this->turn) {
        return false;
    }

    vector<Position> moves = board[from.i][from.j]->get_legal_moves(*this);
    for (auto m : moves) {
        if (m == to) {
            this->remove_piece(to);
            board[from.i][from.j]->set_pos(to);
            board[to.i][to.j] = board[from.i][from.j];
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
    for (int i = 0; i < 8; ++i) {
        cout << notation::row_to_coord(i) << "  ";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j]->get_symbol() << " ";
        }
        cout << "\n";
    }
    cout << "   ";
    for (int j = 0; j < 8; ++j) {
        cout << notation::col_to_coord(j) << " ";
    }
    cout << "\n\n";
}

const State::board_t& State::get_board() const { 
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
        this->board[i][j] = new_piece; \
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
            this->board[i][j] = new_piece;
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
    this->board[pos.i][pos.j] = new Empty();
}

/*
    Cleanup references from: board, material, pinned_pieces, checking_pieces
*/
void State::remove_piece(Position pos) {
    int i = pos.i;
    int j = pos.j;
    Piece *to_remove = this->board[i][j];
    piecetype::Piece piece = to_remove->get_type();
    Player p = to_remove->get_player();

    if (piece != piecetype::King) {
        this->board[i][j] = nullptr;
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
    return 8 > pos.i && pos.i >= 0 && 8 > pos.j && pos.j >= 0 &&
    board[pos.i][pos.j]->get_player() == !(bool)this->get_turn() &&
    board[pos.i][pos.j]->get_type() != piecetype::King;
}