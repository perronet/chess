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
                board[i][j] = new Empty();
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
    optional<pair<Position, Position>> result = this->move_parser.parse(move_notation, this);

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

    vector<Position> moves = board[from.i][from.j]->get_legal_moves(this);
    for (auto m : moves) {
        if (m == to) {
            if (this->check_capture(to))
                this->remove_piece(to);
            board[from.i][from.j]->set_pos(to);
            board[to.i][to.j] = board[from.i][from.j];
            board[from.i][from.j] = new Empty();

            this->turn = (Player)!(bool)this->get_turn();
            return true;
        }
    }
    return false;
}

bool State::in_check() {
    return this->checking_pieces.empty();
}

vector<piece::Piece*> State::get_checking_pieces() {
    return this->checking_pieces;
}

vector<piece::Piece*> State::get_pinned_pieces() {
    return this->pinned_pieces;
}

void State::print() {
    for (int i = 0; i < 8; ++i) {
        cout << this->move_parser.row_to_coord(i) << "  ";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j]->get_symbol() << " ";
        }
        cout << "\n";
    }
    cout << "   ";
    for (int j = 0; j < 8; ++j) {
        cout << this->move_parser.col_to_coord(j) << " ";
    }
    cout << "\n\n";
}

piece::Piece* (*State::get_board())[8] { 
    return this->board; 
}

// TODO test returning constant pointer
Material *State::get_pieces(Player p) {
    if (p == White)
        return &this->white_pieces;
    else
        return &this->black_pieces;
}

King *State::get_king(Player p) {
    if (p == White)
        return this->white_pieces.king;
    else
        return this->black_pieces.king;
}

Player State::get_turn() {
    return this->turn;
}

#define CASE_PIECE_ADD(piececlass, piecevector) \
    case piecetype::piececlass: { \
        piececlass *new_piece = new piececlass(p, pos); \
        this->board[i][j] = new_piece; \
        if (p == White) \
            this->white_pieces.piecevector.push_back(new_piece); \
        else \
            this->black_pieces.piecevector.push_back(new_piece); \
        break; } \

#define CASE_PIECE_REMOVE(piececlass, piecevector, to_remove) \
    case piecetype::piececlass: { \
        if (p == White) { \
            this->white_pieces.piecevector.erase(remove(this->white_pieces.piecevector.begin(), this->white_pieces.piecevector.end(), to_remove), this->white_pieces.piecevector.end()); \
        } else { \
            this->black_pieces.piecevector.erase(remove(this->white_pieces.piecevector.begin(), this->white_pieces.piecevector.end(), to_remove), this->white_pieces.piecevector.end()); \
        } \
        delete to_remove; \
        break; } \

void State::add_piece(Player p, piecetype::Piece piece, Position pos) {
    int i = pos.i;
    int j = pos.j;
    switch (piece) {
        CASE_PIECE_ADD(Pawn, pawn);
        CASE_PIECE_ADD(Rook, rook);
        CASE_PIECE_ADD(Knight, knight);
        CASE_PIECE_ADD(Bishop, bishop);
        CASE_PIECE_ADD(Queen, queen);
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

void State::remove_piece(Position pos) {
    int i = pos.i;
    int j = pos.j;
    Piece *to_remove = this->board[pos.i][pos.j];
    piecetype::Piece piece = to_remove->get_type();
    Player p = to_remove->get_player();
    switch (piece) {
        CASE_PIECE_REMOVE(Pawn, pawn, to_remove);
        CASE_PIECE_REMOVE(Rook, rook, to_remove);
        CASE_PIECE_REMOVE(Knight, knight, to_remove);
        CASE_PIECE_REMOVE(Bishop, bishop, to_remove);
        CASE_PIECE_REMOVE(Queen, queen, to_remove);
        default:
            break;
    }
}

bool State::check_capture(Position pos) {
    return 8 > pos.i && pos.i >= 0 && 8 > pos.j && pos.j >= 0 &&
    board[pos.i][pos.j]->get_player() == !(bool)this->get_turn() &&
    board[pos.i][pos.j]->get_type() != piecetype::King;
}