# Chess

This program is an implementation of chess. The interface can be used by either a human or a chess engine. All moves go through the `State` class (`state.h`), which records the state of the game and the board.

* Interface for humans: `bool State::move(string move_notation)` => parse notation => `bool State::move(Move input_move)`
* Interface for engines: `std::vector<Move> State::get_legal_moves()` => pick a move => `bool State::move(Move input_move)`

Pieces are defined as a hierarchy of classes in `piece.h`. Each piece overrides `std::vector<Move> get_legal_moves(const State& s)` to return its own legal moves. Legal moves *always* depend on the state of the board: for example, a piece could be pinned to the king, and the king cannot move on squares under attack.

## Quick start

Install the required packages:

`pip3 install -r requirements.txt`

Simply run `make` to compile and run.

## Algebraic notation

Standard [algebraic notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)) applies, with these exceptions:

* You cannot specify only the row or only the column (e.g. `R3e6` or `Rac6`). If there are two possible pieces that can move to the same square, please use the whole coordinate (`Re3e6`, `Ra6c6`).
* You cannot use "O-O" and "O-O-O" for kingside/queenside castle. Simply specify the square the king wants to move to: `Kg1`
* You don't have to add "x" when capturing (e.g. `Qxh4`, `Ba4xb5`), although you can if you want to. The x will simply be ignored.

To resign or offer/accept a draw, just type "`resign`" or "`draw`".

### Notation guide

Pieces: King = "K", Queen = "Q", Knight = "N", Bishop = "B", Rook = "R". You don't have to specify pawns with "P", although you can if you want to.

* Move a pawn: `e4`
* Move a pawn, unambiguous: `e2e4`
* Move the knight: `Nb5`
* Move the knight, unambiguous: `Nd3b5`
* Capture with a bishop: `Bb5`, `Bxb5`, `Bf1xb5` ("x" is not necessary)
* Promote a pawn: `h8=Q`, `h8=N`, `h7h8=N`, `h7xh8=N`
* Castle queenside, white: `Kc1`
* Castle kingside, white: `Kg1`
