import defs

class GameState:
    def __init__(self):
        self.board = defs.INIT_BOARD
        self.turn = defs.WHITE
        self.in_check = False
        self.checkmate = False
        self.stalemate = False
        self.white_castle_available = True
        self.black_castle_available = True
        self.move_cnt = 0
        self.piece_cnt_w = defs.INIT_PIECE_CNT
        self.piece_cnt_b = defs.INIT_PIECE_CNT

    def update(self):
        self.change_turn()
        self.move_cnt += 1

    def change_turn(self):
        self.turn = not self.turn

    def move_piece(self, move):
        from_idx = get_move_from_square(move)
        to_idx = get_move_to_square(move)
        self.board[to_idx[0]][to_idx[1]] = self.board[from_idx[0]][from_idx[1]]
        self.board[from_idx[0]][from_idx[1]] = defs.EMPTY
        self.update()

    # Move output format is ((from_x, from_y), (to_x, to_y), piece)
    def parse_check_move(self, move_str):
        piece = move_str[0]
        coord_col = move_str[1]
        coord_row = move_str[2]
        ambiguous = False

        # parse_move(piece, coord_x, coord_y) # TODO add input checks
        # TODO smarter ambiguity check
        if self.turn == defs.WHITE and self.piece_cnt_w[piece] > 1:
            ambiguous = True
        elif self.turn == defs.BLACK and self.piece_cnt_b[piece] > 1:
            ambiguous = True
        else:
            from_pos = self.get_piece_pos(piece, self.turn)[0]

        if ambiguous:
            coord_str = input("Which one? ")
            from_pos = coord_to_idx((coord_str[1], coord_str[0]))

        return (from_pos, coord_to_idx((coord_row, coord_col)), piece)

    def get_piece_pos(self, piece, color):
        ret = []
        for i in range(0, 8):
            for j in range(0, 8):
                if get_piece_type(self.board[i][j]) == piece and self.is_player_color(self.board[i][j]):
                    ret.append((i, j))
        return ret

    ### CHECKS ###

    # Target square must be empty, basic piece movement must be legal, must not put king in check, also check castle
    # def is_move_legal(move, board):
    # def is_castle_legal(board, game_state):

    def is_player_color(self, piece):
        return get_piece_color(piece) == self.turn

    def is_opponent_color(self, piece):
        return get_piece_color(piece) != self.turn

    def is_player_king(self, piece):
        return get_piece_type(piece) == defs.KING and self.is_player_color(piece)

    def is_opponent_king(self, piece):
        return get_piece_type(piece) == defs.KING and self.is_opponent_color(piece)

    # TODO add 50 moves rule and threefold
    def game_ended(self):
        self.checkmate or self.stalemate

    def draw_board(self): # TODO flip
        print(
            "    a   b   c   d   e   f   g   h  \n"
            "  ╔═══╤═══╤═══╤═══╤═══╤═══╤═══╤═══╗"
            )
        for i in range(0, 8):
            print(str(8-i) + " ║", end = "")
            for j in range(0, 8):
                final = " ║ " + str(8-i) + "\n" if j == 7 else " │"
                print(" " + prettify(self.board[7-i][j]), end = final)
            if i == 7:
                print(
                    "  ╚═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╝\n"
                    "    a   b   c   d   e   f   g   h  \n"
                    )
            else:
                print("  ╟───┼───┼───┼───┼───┼───┼───┼───╢")


### FUNCTIONS NOT INTERACTING WITH THE STATE ###

# Conversions
def coord_to_idx(coord_tuple):
    return (int(coord_tuple[0])-1, defs.COORD_TO_IDX_DICT[coord_tuple[1]])

def idx_to_coord(idx_tuple):
    return (str(idx_tuple[0]+1), defs.IDX_TO_COORD_DICT[idx_tuple[1]])

# Various getters
def get_move_piece(move):
    return move[2]

def get_move_from_square(move):
    return move[0]

def get_move_to_square(move):
    return move[1]

def get_piece_type(piece):
    return piece[0]

def get_piece_color(piece):
    return piece[1]

def is_white(piece):
    return get_piece_color(piece) == defs.WHITE

def is_black(piece):
    return get_piece_color(piece) == defs.BLACK

def prettify(piece):
    return defs.UGLY_TO_PRETTY_DICT[piece]
