#pragma once

#include <string>
#include <optional>
#include "types.h"

namespace state {
    class State;
}

namespace parse {
    class MoveParser {
        public:
            std::optional<std::pair<Position, Position>> parse(std::string move, state::State *s);
            std::optional<Position> disambiguate_piece(piecetype::Piece, Position pos_to, state::State *s);
            std::optional<Position> coord_to_pos(std::string coord);
            piecetype::Piece char_to_piece(char c);
            char col_to_coord(int idx);
            char row_to_coord(int idx);
    };
}
