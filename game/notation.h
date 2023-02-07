#pragma once

#include <string>
#include <optional>
#include <unordered_set>
#include "types.h"

namespace state {
    class State;
}

namespace notation {
    std::optional<Move> parse(std::string move, const state::State& s);
    std::optional<Position> disambiguate_piece(piecetype::Piece, Position pos_to, const state::State& s);
    
    std::optional<Position> coord_to_pos(std::string coord);
    piecetype::Piece char_to_piece(char c);
    char col_to_coord(int idx);
    char row_to_coord(int idx);
}
