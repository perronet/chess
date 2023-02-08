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

    # Piecesbitmap
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

# Returns a value between -1 and 1 using a sigmoid function
def normalize_stockfish_eval(score):
    if not score:
        return 0

    # Guaranteed checkmate
    if score[0] == "#":
        return 1 if int(score[1:]) >= 0 else 0

    return expit(int(score))
