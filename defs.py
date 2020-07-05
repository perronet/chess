import sys

WHITE = True
BLACK = False

PAWN_B = '♙'
KNIGHT_B = '♘'
BISHOP_B = '♗'
ROOK_B = '♖'
QUEEN_B = '♕'
KING_B = '♔'
PAWN_W = '♟︎'
KNIGHT_W = '♞'
BISHOP_W = '♝'
ROOK_W = '♜'
QUEEN_W = '♛'
KING_W = '♚'

PAWN_VAL = 1
KNIGHT_VAL = 3
BISHOP_VAL = 3
ROOK_VAL = 5
QUEEN_VAL = 9
KING_VAL = sys.maxsize

PAWN = 'p'
KNIGHT = 'n'
BISHOP = 'b'
ROOK = 'r'
QUEEN = 'q'
KING = 'k'
EMPTY = ((),())

# Pieces are tuple (piece_type, color)
INIT_BOARD = [
    [(ROOK, WHITE), (KNIGHT, WHITE), (BISHOP, WHITE), (QUEEN, WHITE), (KING, WHITE), (BISHOP, WHITE), (KNIGHT, WHITE), (ROOK, WHITE)],
    [(PAWN, WHITE), (PAWN, WHITE), (PAWN, WHITE), (PAWN, WHITE), (PAWN, WHITE), (PAWN, WHITE), (PAWN, WHITE), (PAWN, WHITE)],
    [EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY],
    [EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY],
    [EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY],
    [EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY],
    [(PAWN, BLACK), (PAWN, BLACK), (PAWN, BLACK), (PAWN, BLACK), (PAWN, BLACK), (PAWN, BLACK), (PAWN, BLACK), (PAWN, BLACK)],
    [(ROOK, BLACK), (KNIGHT, BLACK), (BISHOP, BLACK), (QUEEN, BLACK), (KING, BLACK), (BISHOP, BLACK), (KNIGHT, BLACK), (ROOK, BLACK)]
]

INIT_PIECE_CNT = {
    PAWN: 8,
    KNIGHT: 2,
    BISHOP: 2,
    ROOK: 2,
    QUEEN: 1,
    KING: 1
}

COORD_TO_IDX_DICT = {
    "a": 0,
    "b": 1,
    "c": 2,
    "d": 3,
    "e": 4,
    "f": 5,
    "g": 6,
    "h": 7,
}

IDX_TO_COORD_DICT = {
    0: "a",
    1: "b",
    2: "c",
    3: "d",
    4: "e",
    5: "f",
    6: "g",
    7: "h",
}

UGLY_TO_PRETTY_DICT = {
    (PAWN, WHITE): PAWN_W,
    (KNIGHT, WHITE): KNIGHT_W,
    (BISHOP, WHITE): BISHOP_W,
    (ROOK, WHITE): ROOK_W,
    (QUEEN, WHITE): QUEEN_W,
    (KING, WHITE): KING_W,
    (PAWN, BLACK): PAWN_B,
    (KNIGHT, BLACK): KNIGHT_B,
    (BISHOP, BLACK): BISHOP_B,
    (ROOK, BLACK): ROOK_B,
    (QUEEN, BLACK): QUEEN_B,
    (KING, BLACK): KING_B,
    EMPTY: ' ',
}
