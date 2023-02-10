import numpy as np
from scipy.special import expit
import setup

'''
bitmap order: P R N B Q K p r n b q k
each bit is 1 if it belongs to the side moving, 0 if not present, and -1 for the opponent
'''
def piece_char_to_bitmap(white_to_move, ch=''):
    bitmap = np.zeros(12)
    if not ch:
        return bitmap
    
    is_white_piece = False
    if ch.isupper():
        is_white_piece = True
    offset = 0 if is_white_piece else 6
    value = 1 if white_to_move == is_white_piece else -1
    
    ch = ch.upper()
    if ch == 'P':
        bitmap[offset+0] = value
    elif ch == 'R':
        bitmap[offset+1] = value
    elif ch == 'N':
        bitmap[offset+2] = value
    elif ch == 'B':
        bitmap[offset+3] = value
    elif ch == 'Q':
        bitmap[offset+4] = value
    elif ch == 'K':
        bitmap[offset+5] = value

    return bitmap

def fen_to_vector(fen_str):
    training_example = np.zeros(setup.N_FEATURES)
    fields = fen_str.split()
    curr_idx = 0
    white_to_move = True if fields[1] == 'w' else False

    # Pieces bitmap
    for row in fields[0].split("/"):
        for ch in row:
            if ch.isdigit():
                # Empty squares
                for _ in range(int(ch)):
                    for bit in piece_char_to_bitmap(white_to_move):
                        training_example[curr_idx] = bit
                        curr_idx += 1
            else:
                for bit in piece_char_to_bitmap(white_to_move, ch):
                    training_example[curr_idx] = bit
                    curr_idx += 1

    return training_example

# Convert evaluation based on moving side (positive = winning for moving side)
def convert_stockfish_eval(fen_str, score_str):
    if not score_str:
        return 0

    # Guaranteed checkmate
    if score_str[0] == "#":
        score = setup.CENTIPAWN_MAX if int(score_str[1:]) >= 0 else setup.CENTIPAWN_MIN
    else:
        score = int(score_str)

    black_to_move = True if fen_str.split()[1] == 'b' else False
    if black_to_move:
        score = -score

    if score >= setup.CENTIPAWN_MAX:
        return 1
    if score <= setup.CENTIPAWN_MIN:
        return 0

    return (score - setup.CENTIPAWN_MIN)/(setup.CENTIPAWN_MAX - setup.CENTIPAWN_MIN)
