#include <string>
#include <optional>
#include "types.h"
#include "notation.h"
#include "state.h"

using namespace std;

namespace notation {
    optional<Move> parse(string move, const state::State& s) {
        piecetype::Piece piece = piecetype::Pawn;
        string coord_1;
        string coord_2;
        optional<Position> pos_from = nullopt;
        optional<Position> pos_to = nullopt;
        int i = 0;
        bool capture_found = false;

        // Check special moves first
        if (move == "resign") {
            Move m;
            m.is_resign = true;
            return m;
        }
        if (move == "draw") {
            Move m;
            m.is_draw_offer = true;
            return m;
        }

        // Non-pawn piece: record piece type
        if (i < move.length() && isupper(move[i])) {
            piece = char_to_piece(move[i]);
            i++;
        }

        // Ignore 'x' for capture (e.g. Qxd5)
        if (i < move.length() && move[i] == 'x') {
            capture_found = true;
            i++;
        }

        // Next is a coordinate
        if (i < move.length()) {
            coord_1 = move.substr(i, 2);
            i += 2;
        }

        // There still could be 'x' and another coordinate (e.g. Qe6xd5)
        // If there is '=', then coordinates ended. Parse the promotion later (e.g. h8=Q)
        if (i < move.length() && move[i] != '=') {
            if (move[i] == 'x') {
                if (capture_found)
                    return nullopt;
                i++;
            }

            if (i >= move.length())
                return nullopt;
            
            coord_2 = move.substr(i, 2);
            i += 2;
        }

        if (coord_1.length() == 2 && coord_2.length() == 2) {
            // coord_1 = "from", coord_2 = "to"
            pos_from = coord_to_pos(coord_1);
            pos_to = coord_to_pos(coord_2);
        } else if (coord_1.length() == 2 && coord_2.length() == 0) {
            // coord_1 = "to", must disambiguate "from"
            pos_to = coord_to_pos(coord_1);
            if (pos_to != nullopt)
                pos_from = disambiguate_piece(piece, pos_to.value(), s);
        }

        /* Parsing successful. But the move could still be invalid based on the state of the board. */
        if (pos_from.has_value() && pos_to.has_value()) {
            Move m = Move(pos_from.value(), pos_to.value());
            int board_end = s.get_turn() == White ? 0 : BOARD_SIZE - 1;

            // Is the specified piece type actually there?
            if (s.get_board()[m.from]->get_type() != piece)
                return nullopt;

            // Is it a promotion move? Then the user must provide a promotion type
            if (pos_to.value().i == board_end && 
                s.get_board()[pos_from.value()]->get_type() == piecetype::Pawn) {

                if (i != move.length() - 2 || move[i] != '=')
                    return nullopt;
                i++;
                piecetype::Piece typ = char_to_piece(move[i]);
                if (typ == piecetype::Empty)
                    return nullopt;
                i++;

                m.set_promotion(typ);
            }

            // There should be no chars left to parse
            if (i != move.length())
                return nullopt;

            return m;
        }

        return nullopt;
    }

    optional<Position> disambiguate_piece(piecetype::Piece typ, Position pos_to, const state::State& s) {
        optional<Position> pos_from = nullopt;
        auto pieces = s.get_pieces(s.get_turn(), typ);

        if (pieces.size() == 0)
            return nullopt;

        /* Only one possibiliy */
        if (pieces.size() == 1)
            return pieces[0]->get_pos();

        /* Check if only one piece can move to the square "pos_to" */
        for (auto piece : pieces) {
            vector<Move> legal_moves = piece->get_legal_moves(s);

            auto it = find_if(legal_moves.begin(), legal_moves.end(),
            [pos_to](Move m){
                return m.to == pos_to;
            });

            /* Move found */
            if (it != legal_moves.end()) {
                /* Another move was already found: can't disambiguate */
                if (pos_from.has_value())
                    return nullopt;
                pos_from = it->from;
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