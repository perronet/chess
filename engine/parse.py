import numpy as np
from scipy.special import expit
import setup
import codecs

def piece_char_to_bits(ch=''):
    if not ch:
        return 6
    bits = 0
    if ch.islower():
        bits |= 1 << 3
    ch = ch.upper()
    if ch == 'P':
        bits += 0
    elif ch == 'R':
        bits += 1
    elif ch == 'N':
        bits += 2
    elif ch == 'B':
        bits += 3
    elif ch == 'Q':
        bits += 4
    elif ch == 'K':
        bits += 5

    return bits

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
                    training_example[curr_idx] = piece_char_to_bits()
                    curr_idx += 1
            else:
                training_example[curr_idx] = piece_char_to_bits(ch)
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

    # En passant
    if fields[3] != '-':
        if fields[3][1] == '3':
            square_id = 0
        elif fields[3][1] == '6':
            square_id = 8
        
        square_id += ord(fields[3][0]) - 96
        training_example[curr_idx] = square_id

    return training_example

# Returns a value between -1 and 1 using a sigmoid function
def normalize_stockfish_eval(score):
    if not score:
        return 0

    # Guaranteed checkmate
    if score[0] == "#":
        return 1 if int(score[1:]) >= 0 else -1

    return expit(int(score)) - 0.5
