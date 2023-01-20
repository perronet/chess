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

string Piece::get_symbol() const {
    return symbol;
}

Player Piece::get_player() const {
    return player;
}

Position Piece::get_pos() const {
    return pos;
}

void Piece::set_pos(Position p) {
    pos = p;
    first_move = false;
}

bool Piece::is_empty() const {
    return this->get_type() == piecetype::Empty;
}

bool Piece::is_first_move() const {
    return this->first_move;
}

optional<const Piece*> Piece::check_pinned(const state::State& s) const {
    optional<const Piece*> ret = nullopt; 
    for (auto pin : s.get_pinned_pieces()) {
        if (pin.second == this) {
            ret = pin.first;
            break;
        }
    }
    return ret;
}

vector<Position> Piece::get_legal_moves_pinned(const state::State& s, const Piece* pinner) const {
    vector<Position> v;

    if (this->get_type() == piecetype::Queen || 
    this->get_pos().is_diagonal_to(pinner->get_pos()) && this->get_type() == piecetype::Bishop ||
    this->get_pos().is_orthogonal_to(pinner->get_pos()) && this->get_type() == piecetype::Rook
    ) 
    {
        v = this->get_pos().range(pinner->get_pos());
        v.push_back(pinner->get_pos()); // Capture
    }

    return v;
}

std::unique_ptr<Piece> Piece::get_piece_by_type(piecetype::Piece typ, Player p, Position pos) {
    switch (typ) {
        case piecetype::Pawn:
            return std::make_unique<Pawn>(p, pos);
        case piecetype::Knight:
            return std::make_unique<Knight>(p, pos);
        case piecetype::Bishop:
            return std::make_unique<Bishop>(p, pos);
        case piecetype::Queen:
            return std::make_unique<Queen>(p, pos);
        case piecetype::King:
            return std::make_unique<King>(p, pos);
        default:
            return std::make_unique<Empty>(p, pos);
    }
}

Empty::Empty() {
    symbol = "-";
    player = None;
}

Empty::Empty(Player p, Position pos) : Piece::Piece(p, pos) {
    symbol = "-";
}


piecetype::Piece Empty::get_type() const {
    return piecetype::Empty;
}

vector<Position> Empty::get_legal_moves(const state::State& s) const {
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

piecetype::Piece Pawn::get_type() const {
    return piecetype::Pawn;
}

vector<Position> Pawn::get_legal_moves(const state::State& s) const {
    vector<Position> v;
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value())
        return this->get_legal_moves_pinned(s, pinner.value());

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

vector<Position> Pawn::get_legal_moves_pinned(const state::State& s, const Piece* pinner) const {
    vector<Position> v;
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    if (this->player == White) {
        // Move
        if (this->get_pos().is_orthogonal_to(pinner->get_pos())) {
            if (board(i - 1, j)->is_empty())
                v.push_back({i - 1, j});
            if (this->first_move && board(i - 2, j)->is_empty())
                v.push_back({i - 2, j});
        }

        // Capture
        if (pinner->get_pos() == Position{i - 1, j - 1} || pinner->get_pos() == Position{i - 1, j + 1})
            v.push_back(pinner->get_pos());
    } else if (this->player == Black) {
        // Move
        if (this->get_pos().is_orthogonal_to(pinner->get_pos())) {
            if (board(i + 1,j)->is_empty())
                v.push_back({i + 1, j});
            if (this->first_move && board(i + 2, j)->is_empty())
            v.push_back({i + 2, j});
        }

        // Capture
        if (pinner->get_pos() == Position{i + 1, j - 1} || pinner->get_pos() == Position{i + 1, j + 1})
            v.push_back(pinner->get_pos());
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

piecetype::Piece Rook::get_type() const {
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

vector<Position> Rook::get_legal_moves(const state::State& s) const {
    vector<Position> v;
    state::Board board = s.get_board();
    int pos_i = this->pos.i;
    int pos_j = this->pos.j;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value())
        return this->get_legal_moves_pinned(s, pinner.value());

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

piecetype::Piece Knight::get_type() const {
    return piecetype::Knight;
}

vector<Position> Knight::get_legal_moves(const state::State& s) const {
    vector<Position> v;
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    /* Knight has no legal moves when pinned */
    auto pinner = this->check_pinned(s);
    if (pinner.has_value())
        return v;

    vector<Position> v_check {
        {i+1, j+2}, {i+2, j+1},
        {i-1, j-2}, {i-2, j-1},
        {i-1, j+2}, {i-2, j+1},
        {i+1, j-2}, {i+2, j-1},
    };
    for (Position p : v_check) {
        if (p.check_bounds()) {
            if (board[p]->is_empty() || s.check_capture(p))
                v.push_back(p);
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

piecetype::Piece Bishop::get_type() const {
    return piecetype::Bishop;
}

vector<Position> Bishop::get_legal_moves(const state::State& s) const {
    vector<Position> v;
    state::Board board = s.get_board();
    int pos_i = this->pos.i;
    int pos_j = this->pos.j;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value())
        return this->get_legal_moves_pinned(s, pinner.value());

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

piecetype::Piece Queen::get_type() const {
    return piecetype::Queen;
}

vector<Position> Queen::get_legal_moves(const state::State& s) const {
    vector<Position> v;
    Rook rook(this->player, this->pos);
    Bishop bishop(this->player, this->pos);

    auto pinner = this->check_pinned(s);
    if (pinner.has_value())
        return this->get_legal_moves_pinned(s, pinner.value());

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

piecetype::Piece King::get_type() const {
    return piecetype::King;
}

vector<Position> King::get_legal_moves(const state::State& s) const {
    vector<Position> v;
    Player curr_player = s.get_turn();
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    for (Position p : this->get_moves_unrestricted(s)) {
        if (!s.is_square_attacked(p, curr_player))
            v.push_back(p);
    }

    /* Castle */
    if (this->first_move && !s.in_check()) {
        /* Queen rook, King rook */
        for (Position rook_pos : {Position{i, j-4}, Position{i, j+3}}) {
            if (rook_pos.check_bounds()) {
                Piece* rook = board[rook_pos];
                if (rook->is_first_move() && 
                    rook->get_player() == curr_player && 
                    rook->get_type() == piecetype::Rook) {

                    Position new_king_pos = rook_pos.j < j ? Position{i, j-2} : Position{i, j+2};
                    Position new_king_pos_bound = rook_pos.j < j ? Position{i, j-3} : Position{i, j+3};

                    /* All squares the King passes through must not be under attack */
                    vector<Position> v_check = this->get_pos().range(new_king_pos_bound);
                    if (all_of(v_check.begin(), v_check.end(), [&](Position p) {
                        return !s.is_square_attacked(p, curr_player);
                    })) {
                        v.push_back(new_king_pos);
                    }
                }
            }
        }
    }

    return v;
}

std::vector<Position> King::get_moves_unrestricted(const state::State& s) const {
    vector<Position> v;
    state::Board board = s.get_board();
    int i = this->pos.i;
    int j = this->pos.j;

    vector<Position> v_check {
        {i, j+1}, {i, j-1}, {i+1, j}, {i-1, j},
        {i+1, j+1}, {i-1, j-1}, {i+1, j-1}, {i-1, j+1}
    };

    for (Position p : v_check) {
        if (p.check_bounds() && (board[p]->is_empty() || s.check_capture(p)))
            v.push_back(p);
    }

    return v;
}