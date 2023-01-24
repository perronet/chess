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

std::string Piece::get_symbol() const {
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

vector<Move> Piece::get_legal_moves_pinned(const state::State& s, const Piece* pinner) const {
    vector<Move> v;
    Position from = this->get_pos();

    if (this->get_type() == piecetype::Queen || 
    from.is_diagonal_to(pinner->get_pos()) && this->get_type() == piecetype::Bishop ||
    from.is_orthogonal_to(pinner->get_pos()) && this->get_type() == piecetype::Rook
    ) 
    {
        for (auto to : from.range(pinner->get_pos()))
            v.push_back(Move(from, to));
        v.push_back(Move(from, pinner->get_pos(), true)); // Capture
    }

    return v;
}

void Piece::filter_legal_moves_under_check(const state::State& s, vector<Move>& moves) const {
    if (!s.in_check() || s.in_double_check() || moves.empty())
        return;
    
    Position from = moves[0].from;
    Position checker_pos = s.get_checking_pieces()[0]->get_pos();
    unordered_set<Move, MoveHash> potential_moves;
    
    // Capturing the checking piece
    potential_moves.insert(Move(from, checker_pos));

    // Blocking the check
    if (s.in_blockable_check()) {
        for (Position blocking_pos: checker_pos.range(s.get_king()->get_pos()))
            potential_moves.insert(Move(from, blocking_pos));
    }

    // Take the intersection between moves and potential_moves
    moves.erase(
        remove_if(moves.begin(), moves.end(), [&](Move m) {
            return potential_moves.find(m) == potential_moves.end();
        }),
        moves.end()
    );
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

vector<Move> Empty::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    return v;
}

Pawn::Pawn(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White)
        symbol = "♟︎";
    else
        symbol = "♙";
}

piecetype::Piece Pawn::get_type() const {
    return piecetype::Pawn;
}

void Pawn::set_two_squares_move(int move_num) {
    this->two_squares_move = move_num;
}

/* En passant is legal only on the very next move. */
bool Pawn::check_capture_en_passant(const state::State& s, Position pos) const {
    Piece* piece = s.get_board()[pos];
    int curr_move = s.get_move_count();
    return s.check_capture(pos) && piece->get_type() == piecetype::Pawn &&
            ((Pawn*)piece)->two_squares_move == curr_move - 1;
}

vector<Move> Pawn::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    const state::Board& board = s.get_board();
    Position from = this->get_pos();
    int i = this->get_pos().i;
    int j = this->get_pos().j;

    if (s.in_double_check())
        return v;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value()) {
        v = this->get_legal_moves_pinned(s, pinner.value());
    } else {
        
        if (this->player == White) {
            // Move
            if (board(i - 1, j)->is_empty())
                v.push_back(Move(from, {i - 1, j}));
            if (this->first_move && board(i - 2, j)->is_empty())
                v.push_back(Move(from, {i - 2, j}));

            // Capture
            if (s.check_capture({i - 1, j - 1}))
                v.push_back(Move(from, {i - 1, j - 1}, true));
            if (s.check_capture({i - 1, j + 1}))
                v.push_back(Move(from, {i - 1, j + 1}, true));
            if (this->check_capture_en_passant(s, {i, j - 1})) {
                Move en_passant = Move(from, {i - 1, j - 1}, true);
                en_passant.set_en_passant({i, j - 1});
                v.push_back(en_passant);
            }
            if (this->check_capture_en_passant(s, {i, j + 1})) {
                Move en_passant = Move(from, {i - 1, j + 1}, true);
                en_passant.set_en_passant({i, j + 1});
                v.push_back(en_passant);
            }
        } else if (this->player == Black) {
            // Move
            if (board(i + 1,j)->is_empty())
                v.push_back(Move(from, {i + 1, j}));
            if (this->first_move && board(i + 2, j)->is_empty())
                v.push_back(Move(from, {i + 2, j}));

            // Capture
            if (s.check_capture({i + 1, j - 1}))
                v.push_back(Move(from, {i + 1, j - 1}, true));
            if (s.check_capture({i + 1, j + 1}))
                v.push_back(Move(from, {i + 1, j + 1}, true));
            if (this->check_capture_en_passant(s, {i, j - 1})) {
                Move en_passant = Move(from, {i + 1, j - 1}, true);
                en_passant.set_en_passant({i, j - 1});
                v.push_back(en_passant);
            }
            if (this->check_capture_en_passant(s, {i, j + 1})) {
                Move en_passant = Move(from, {i + 1, j + 1}, true);
                en_passant.set_en_passant({i, j + 1});
                v.push_back(en_passant);
            }
        }
    }

    this->filter_legal_moves_under_check(s, v);

    return v;
}

vector<Move> Pawn::get_legal_moves_pinned(const state::State& s, const Piece* pinner) const {
    vector<Move> v;
    const state::Board& board = s.get_board();
    Position from = this->get_pos();
    int i = this->get_pos().i;
    int j = this->get_pos().j;

    if (this->player == White) {
        // Move
        if (this->get_pos().is_orthogonal_to(pinner->get_pos())) {
            if (board(i - 1, j)->is_empty())
                v.push_back(Move(from, {i - 1, j}));
            if (this->first_move && board(i - 2, j)->is_empty())
                v.push_back(Move(from, {i - 2, j}));
        }

        // Capture
        if (pinner->get_pos() == Position{i - 1, j - 1} || pinner->get_pos() == Position{i - 1, j + 1})
            v.push_back(Move(from, pinner->get_pos(), true));
    } else if (this->player == Black) {
        // Move
        if (this->get_pos().is_orthogonal_to(pinner->get_pos())) {
            if (board(i + 1,j)->is_empty())
                v.push_back(Move(from, {i + 1, j}));
            if (this->first_move && board(i + 2, j)->is_empty())
            v.push_back(Move(from, {i + 2, j}));
        }

        // Capture
        if (pinner->get_pos() == Position{i + 1, j - 1} || pinner->get_pos() == Position{i + 1, j + 1})
            v.push_back(Move(from, pinner->get_pos(), true));
    }

    return v;    
}

Rook::Rook(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White)
        symbol = "♜";
    else
        symbol = "♖";
}

piecetype::Piece Rook::get_type() const {
    return piecetype::Rook;
}

#define CHECK_EMPTY_OR_CAPTURE(i, j) { \
    if (board(i, j)->is_empty()) { \
        v.push_back(Move(from, {i, j})); \
    } else { \
        if (s.check_capture({i, j})) \
            v.push_back(Move(from, {i, j}, true)); \
        break; \
    } \
} \

vector<Move> Rook::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    const state::Board& board = s.get_board();
    Position from = this->get_pos();
    int pos_i = this->get_pos().i;
    int pos_j = this->get_pos().j;

    if (s.in_double_check())
        return v;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value()) {
        v = this->get_legal_moves_pinned(s, pinner.value());
    } else {
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
    }

    this->filter_legal_moves_under_check(s, v);

    return v;
}

Knight::Knight(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White)
        symbol = "♞";
    else
        symbol = "♘";
}

piecetype::Piece Knight::get_type() const {
    return piecetype::Knight;
}

vector<Move> Knight::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    const state::Board& board = s.get_board();
    Position from = this->get_pos();
    int i = this->get_pos().i;
    int j = this->get_pos().j;

    if (s.in_double_check())
        return v;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value()) {
        v = this->get_legal_moves_pinned(s, pinner.value());
    } else {
        vector<Position> v_check {
            {i+1, j+2}, {i+2, j+1},
            {i-1, j-2}, {i-2, j-1},
            {i-1, j+2}, {i-2, j+1},
            {i+1, j-2}, {i+2, j-1},
        };
        for (Position to : v_check) {
            if (to.check_bounds()) {
                if (board[to]->is_empty())
                    v.push_back(Move(from, to));
                else if (s.check_capture(to))
                    v.push_back(Move(from, to, true));
            }
        }
    }

    this->filter_legal_moves_under_check(s, v);

    return v;
}

Bishop::Bishop(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White)
        symbol = "♝";
    else
        symbol = "♗";
}

piecetype::Piece Bishop::get_type() const {
    return piecetype::Bishop;
}

vector<Move> Bishop::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    const state::Board& board = s.get_board();
    Position from = this->get_pos();
    int pos_i = this->get_pos().i;
    int pos_j = this->get_pos().j;

    if (s.in_double_check())
        return v;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value()) {
        v = this->get_legal_moves_pinned(s, pinner.value());
    } else {
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
    }

    this->filter_legal_moves_under_check(s, v);

    return v;
}

Queen::Queen(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White)
        symbol = "♛";
    else
        symbol = "♕";
}

piecetype::Piece Queen::get_type() const {
    return piecetype::Queen;
}

vector<Move> Queen::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    Rook rook(this->player, this->pos);
    Bishop bishop(this->player, this->pos);

    if (s.in_double_check())
        return v;

    auto pinner = this->check_pinned(s);
    if (pinner.has_value()) {
        v = this->get_legal_moves_pinned(s, pinner.value());

        // No need to filter at the end: it is already done by Rook::get_legal_moves and Bishop::get_legal_moves
        this->filter_legal_moves_under_check(s, v);
    } else {
        v = rook.get_legal_moves(s);
        vector<Move> v_bishop = bishop.get_legal_moves(s);
        v.insert(v.end(), v_bishop.begin(), v_bishop.end());
    }

    return v;
}

King::King(Player p, Position pos) : Piece::Piece(p, pos) {
    if (p == White)
        symbol = "♚";
    else
        symbol = "♔";
}

piecetype::Piece King::get_type() const {
    return piecetype::King;
}

vector<Move> King::get_legal_moves(const state::State& s) const {
    vector<Move> v;
    Player curr_player = s.get_turn();
    Position from = this->get_pos();
    int i = this->get_pos().i;
    int j = this->get_pos().j;

    /* 
        Edge case: what if the square is not attacked right now,
        but will be in the next turn? This happens if the king is currently between
        that square and the attacking piece (i.e. the square is covered by the king itself). 

        Therefore: "remove" the king from the board before computing attacked squares.
    */
    state::State& state_temp = const_cast<state::State&>(s);
    Piece* king_ptr = state_temp.board[pos];
    state_temp.add_empty(pos);

    for (auto move : this->get_moves_unrestricted(s)) {
        if (!s.is_square_attacked(move.to, curr_player))
            v.push_back(move);
    }

    /* Put the king back. The board won"t be modified in the end, keeping the const guarantee. */
    state_temp.remove_piece(pos);
    state_temp.board[pos] = king_ptr;

    /* Castle */
    if (this->first_move && !s.in_check()) {
        /* Queen rook, King rook */
        for (Position rook_pos : {Position{i, j-4}, Position{i, j+3}}) {
            if (rook_pos.check_bounds()) {
                Piece* rook = s.get_board()[rook_pos];
                if (rook->is_first_move() && 
                    rook->get_player() == curr_player && 
                    rook->get_type() == piecetype::Rook) {

                    /* "Is it the queen rook?" */
                    Position new_king_pos = rook_pos.j < j ? Position{i, j-2} : Position{i, j+2};
                    Position new_king_pos_bound = rook_pos.j < j ? Position{i, j-3} : Position{i, j+3};
                    Position new_rook_pos = rook_pos.j < j ? Position{i, j-1} : Position{i, j+1};

                    /* All squares the King passes through must not be under attack */
                    vector<Position> v_check = this->get_pos().range(new_king_pos_bound);
                    if (all_of(v_check.begin(), v_check.end(), [&](Position p) {
                        return !s.is_square_attacked(p, curr_player);
                    })) {
                        Move move(from, new_king_pos);
                        move.set_castle(rook_pos, new_rook_pos);
                        v.push_back(move);
                    }
                }
            }
        }
    }

    return v;
}

std::vector<Move> King::get_moves_unrestricted(const state::State& s) const {
    vector<Move> v;
    const state::Board& board = s.get_board();
    Position from = this->get_pos();
    int i = this->get_pos().i;
    int j = this->get_pos().j;

    vector<Position> v_check {
        {i, j+1}, {i, j-1}, {i+1, j}, {i-1, j},
        {i+1, j+1}, {i-1, j-1}, {i+1, j-1}, {i-1, j+1}
    };

    for (Position to : v_check) {
        if (to.check_bounds()) {
            if (board[to]->is_empty())
                v.push_back(Move(from, to));
            else if (s.check_capture(to))
                v.push_back(Move(from, to, true));
        }
    }

    return v;
}