import game, defs

state = game.GameState()
state.draw_board()

while not state.game_ended():
    turn_str = "White" if state.turn == defs.WHITE else "Black"
    move_str = input(turn_str + " to move: ")
    move = state.parse_check_move(move_str)
    state.move_piece(move)
    state.draw_board()

print("Game over")
