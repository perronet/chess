import numpy as np
from scipy.special import expit
import setup

def piece_char_to_num(ch=''):
    if not ch:
        return 0
    
    num = 0
    is_black = False
    if ch.islower():
        is_black = True
    
    ch = ch.upper()
    if ch == 'P':
        num = 1
    elif ch == 'R':
        num = 2
    elif ch == 'N':
        num = 3
    elif ch == 'B':
        num = 4
    elif ch == 'Q':
        num = 5
    elif ch == 'K':
        num = 6

    return -num if is_black else num

def fen_to_vector(fen_str):
    training_example = np.zeros(setup.N_FEATURES)
    fields = fen_str.split()
    curr_idx = 0

    # Pieces
    for row in fields[0].split("/"):
        for ch in row:
            if ch.isdigit():
                # Empty squares
                for _ in range(int(ch)):
                    training_example[curr_idx] = piece_char_to_num()
                    curr_idx += 1
            else:
                training_example[curr_idx] = piece_char_to_num(ch)
                curr_idx += 1

    # Side to move
    if fields[1] == 'b':
        training_example[curr_idx] = 1
    curr_idx += 1

    # Castling
    for ch in fields[2]:
        if ch == 'K':
            training_example[curr_idx] = 1
        elif ch == 'Q':
            training_example[curr_idx+1] = 1
        elif ch == 'k':
            training_example[curr_idx+2] = 1
        elif ch == 'q':
            training_example[curr_idx+3] = 1
    curr_idx += 4

    return training_example

# Returns a value between -1 and 1 using a sigmoid function
def normalize_stockfish_eval(score):
    if not score:
        return 0

    # Guaranteed checkmate
    if score[0] == "#":
        return 1 if int(score[1:]) >= 0 else 0

    return expit(int(score))
