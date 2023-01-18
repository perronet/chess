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

            this->checking_pieces.clear();
            this->pinned_pieces.clear();

            // Updates pinned list and checking list (discovery check: Queen, Bishop, Rook)
            // TODO interposing piece doesn't mean you necessarily can't move!
            this->update_pins();

            // Update checking list (check from the piece that just moved: Knight, Pawn)
            Piece* moved_piece = board[to];
            if (moved_piece->get_type() == piecetype::Knight || moved_piece->get_type() == piecetype::Pawn) {
                vector<Position> next_moves = moved_piece->get_legal_moves(*this);
                if (find_if(next_moves.begin(), next_moves.end(), [this](auto pos){ 
                    return board[pos]->get_type() == piecetype::King && board[pos]->get_player() == !this->get_turn();
                }) < next_moves.end()) {
                    this->checking_pieces.push_back(moved_piece);
                }
            }

            for (auto p : this->checking_pieces)
                cout << "Check: " << p->get_symbol() << endl;
            for (auto p : this->pinned_pieces)
                cout << "Pinned: " << p->get_symbol() << endl;

            this->turn = (Player)!this->get_turn();
            return true;
        }
    }
    return false;
}

# define CHECK_INTERPOSING(i, j, typ) { \
    Piece* piece = board(i, j); \
    if (!piece->is_empty()) { \
        /* Found ally piece */ \
        if (piece->get_player() == king->get_player()) { \
            if (interposing_piece) \
                break; \
            interposing_piece = piece; \
        /* Found enemy piece */ \
        } else { \
            if (piece->get_type() == typ || piece->get_type() == piecetype::Queen) { \
                if (interposing_piece) \
                    this->pinned_pieces.push_back(interposing_piece); \
                else \
                    this->checking_pieces.push_back(piece); \
            } \
            break; \
        } \
    } \
} \

void State::update_pins() {
    const King* king = this->get_opponent_king();
    int pos_i = king->get_pos().i;
    int pos_j = king->get_pos().j;
    Piece* interposing_piece = nullptr;

    /* Rook, Queen */
    for (int i = pos_i + 1; i < BOARD_SIZE; ++i) {
        CHECK_INTERPOSING(i, pos_j, piecetype::Rook);
    }
    interposing_piece = nullptr;
    for (int i = pos_i - 1; i >= 0; --i) {
        CHECK_INTERPOSING(i, pos_j, piecetype::Rook);
    }
    interposing_piece = nullptr;
    for (int j = pos_j + 1; j < BOARD_SIZE; ++j) {
        CHECK_INTERPOSING(pos_i, j, piecetype::Rook);
    }
    interposing_piece = nullptr;
    for (int j = pos_j - 1; j >= 0; --j) {
        CHECK_INTERPOSING(pos_i, j, piecetype::Rook);
    }
    interposing_piece = nullptr;

    /* Bishop, Queen */
    int i, j;
    for (i = pos_i + 1, j = pos_j + 1; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j) {
        CHECK_INTERPOSING(i, j, piecetype::Bishop);
    }
    interposing_piece = nullptr;
    for (i = pos_i + 1, j = pos_j - 1; i < BOARD_SIZE && j >= 0; ++i, --j) {
        CHECK_INTERPOSING(i, j, piecetype::Bishop);
    }
    interposing_piece = nullptr;
    for (i = pos_i - 1, j = pos_j + 1; i >= 0 && j < BOARD_SIZE; --i, ++j) {
        CHECK_INTERPOSING(i, j, piecetype::Bishop);
    }
    interposing_piece = nullptr;
    for (i = pos_i - 1, j = pos_j - 1; i >= 0 && j >= 0; --i, --j) {
        CHECK_INTERPOSING(i, j, piecetype::Bishop);
    }
    interposing_piece = nullptr;
}

bool State::in_check() const {
    return !this->checking_pieces.empty();
}

/* Blockable check: only one piece checking out of Queen, Bishop, Rook */
bool State::in_blockable_check() const {
    return this->checking_pieces.size() == 1 && (
        this->checking_pieces[0]->get_type() == piecetype::Queen ||
        this->checking_pieces[0]->get_type() == piecetype::Bishop ||
        this->checking_pieces[0]->get_type() == piecetype::Rook
    );
}

// TODO
bool State::in_checkmate() const {
    return false;
}

// TODO
bool State::in_stalemate() const {
    return false;
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

const King* State::get_king() const {
    if (this->get_turn() == White)
        return this->white_pieces.king;
    else
        return this->black_pieces.king;
}

const King* State::get_opponent_king() const {
    if (this->get_turn() == White)
        return this->black_pieces.king;
    else
        return this->white_pieces.king;
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

/* King capture is considered legal in order to detect checks */
bool State::check_capture(Position pos) const {
    return pos.check_bounds() &&
    board[pos]->get_player() == !this->get_turn();
}