#include <string>
#include <optional>
#include "types.h"
#include "parse.h"
#include "state.h"

using namespace std;
using namespace parse;
using namespace state;

optional<pair<Position, Position>> MoveParser::parse(string move, state::State *s) {
    piecetype::Piece piece = piecetype::Pawn;
    int i = 0;
    string coord_1;
    string coord_2;
    optional<Position> pos_from = nullopt;
    optional<Position> pos_to = nullopt;

    // Non-pawn piece
    if (isupper(move[i])) {
        piece = this->char_to_piece(move[i]);
        ++i;
    }

    // Next is a coordinate
    coord_1 = move.substr(i, 2);
    i += 2;

    // There still could be 'x' and another coordinate
    if (i < move.length()) {
        if (move[i] == 'x')
            ++i;
        if (i != move.length() - 2)
            return nullopt;
        coord_2 = move.substr(i, 2);
    }

    if (coord_1.length() == 2 && coord_2.length() == 2) {
        // coord_1 = from, coord_2 = to
        pos_from = this->coord_to_pos(coord_1);
        pos_to = this->coord_to_pos(coord_2);
    } else if (coord_1.length() == 2) {
        // coord_1 = to, must disambiguate from
        pos_to = this->coord_to_pos(coord_1);
        if (pos_to != nullopt)
            pos_from = this->disambiguate_piece(piece, pos_to.value(), s);
    }

    if (pos_from.has_value() && pos_to.has_value())
        return pair(pos_from.value(), pos_to.value());
    return nullopt;
}

optional<Position> MoveParser::disambiguate_piece(piecetype::Piece piece, Position pos_to, state::State *s) {
    optional<Position> pos_from = nullopt;

    // TODO does this work for capturing?
    if (piece == piecetype::Pawn) {
        // Search backwards for the first pawn in the column, starting from pos_to
        if (s->get_turn() == White) {
            for (int row = pos_to.i; row < 8; ++row) {
                if (s->get_board()[row][pos_to.j]->get_type() == piecetype::Pawn) {
                    pos_from = Position{row, pos_to.j};
                    break;
                }
            }
        } else {
            for (int row = pos_to.i; row >= 0; --row) {
                if (s->get_board()[row][pos_to.j]->get_type() == piecetype::Pawn) {
                    pos_from = Position{row, pos_to.j};
                    break;
                }
            }
        }
    }

    // switch (piece) {
    //     case piecetype::Rook:
    //         break;
    //     default:
    //         break; // TODO
    // }

    return pos_from;
}

optional<Position> MoveParser::coord_to_pos(string coord) {
    if (coord.length() != 2)
        return nullopt;
    
    Position pos = Position{8 - (coord[1] - '0'), ((int)coord[0]) - 97};
    if (8 > pos.i && pos.i >= 0 && 8 > pos.j && pos.j >= 0)
        return pos;

    return nullopt;
}

char MoveParser::col_to_coord(int j) {
    return (char)(j + 97);
}

char MoveParser::row_to_coord(int i) {
    return '0' + (8 - i);
}

piecetype::Piece MoveParser::char_to_piece(char c) {
    switch (c) {
        case 'R':
            return piecetype::Rook;
            break;
        case 'N':
            return piecetype::Knight;
            break;
        case 'B':
            return piecetype::Bishop;
            break;
        case 'Q':
            return piecetype::Queen;
            break;
        case 'K':
            return piecetype::King;
            break;
        default:
            return piecetype::Pawn;
            break;
    }
} 
