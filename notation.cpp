#include <string>
#include <optional>
#include "types.h"
#include "notation.h"
#include "state.h"

using namespace std;

namespace notation {
    optional<Move> parse(string move, const state::State& s) {
        piecetype::Piece piece = piecetype::Pawn;
        int i = 0;
        string coord_1;
        string coord_2;
        optional<Position> pos_from = nullopt;
        optional<Position> pos_to = nullopt;

        // Non-pawn piece
        if (isupper(move[i])) {
            piece = char_to_piece(move[i]);
            i++;
        }

        // Next is a coordinate
        coord_1 = move.substr(i, 2);
        i += 2;

        // There still could be 'x' and another coordinate
        if (i < move.length() && move[i] != '=') {
            if (move[i] == 'x')
                i++;
            if (i >= move.length())
                return nullopt;
            coord_2 = move.substr(i, 2);
            i += 2;
        }

        if (coord_1.length() == 2 && coord_2.length() == 2) {
            // coord_1 = from, coord_2 = to
            pos_from = coord_to_pos(coord_1);
            pos_to = coord_to_pos(coord_2);
        } else if (coord_1.length() == 2) {
            // coord_1 = to, must disambiguate from
            pos_to = coord_to_pos(coord_1);
            if (pos_to != nullopt)
                pos_from = disambiguate_piece(piece, pos_to.value(), s);
        }

        if (pos_from.has_value() && pos_to.has_value()) {
            Move m = Move(pos_from.value(), pos_to.value());
            int board_end = s.get_turn() == White ? 0 : BOARD_SIZE - 1;

            // Is it a promotion move? Then the user must provide a promotion type
            if (pos_to.value().i == board_end && 
                s.get_board()[pos_from.value()]->get_type() == piecetype::Pawn) {

                if (i != move.length() - 2 || move[i] != '=')
                    return nullopt;
                i++;
                piecetype::Piece typ = char_to_piece(move[i]);
                if (typ == piecetype::Empty)
                    return nullopt;

                m.set_promotion(typ);
            } 

            return m;
        }

        return nullopt;
    }

    optional<Position> disambiguate_piece(piecetype::Piece piece, Position pos_to, const state::State& s) {
        optional<Position> pos_from = nullopt;

        // TODO does this work for capturing?
        if (piece == piecetype::Pawn) {
            // Search backwards for the first pawn in the column, starting from pos_to
            if (s.get_turn() == White) {
                for (int row = pos_to.i; row < BOARD_SIZE; ++row) {
                    if (s.get_board()(row, pos_to.j)->get_type() == piecetype::Pawn) {
                        pos_from = Position{row, pos_to.j};
                        break;
                    }
                }
            } else {
                for (int row = pos_to.i; row >= 0; --row) {
                    if (s.get_board()(row, pos_to.j)->get_type() == piecetype::Pawn) {
                        pos_from = Position{row, pos_to.j};
                        break;
                    }
                }
            }
        }

        return pos_from;
    }

    optional<Position> coord_to_pos(string coord) {
        if (coord.length() != 2)
            return nullopt;
        
        Position pos = Position{BOARD_SIZE - (coord[1] - '0'), ((int)coord[0]) - 97};
        if (BOARD_SIZE > pos.i && pos.i >= 0 && BOARD_SIZE > pos.j && pos.j >= 0)
            return pos;

        return nullopt;
    }

    char col_to_coord(int j) {
        return (char)(j + 97);
    }

    char row_to_coord(int i) {
        return '0' + (BOARD_SIZE - i);
    }

    piecetype::Piece char_to_piece(char c) {
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
            case 'P':
                return piecetype::Pawn;
                break;
            default:
                return piecetype::Empty;
                break;
        }
    }
}