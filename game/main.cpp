#include <iostream>
#include <vector>
#include "state.h"
#include "notation.h"

using namespace std;

int main() {
    state::State s;
    string move_notation;

    s.print();
    cout << "White to play." << endl;

    while (true) {
        cin >> move_notation;

        if (s.move(move_notation)) {
            if (s.game_ended())
                break;

            if (s.get_turn() == White)
                cout << "White to play." << endl;
            else
                cout << "Black to play." << endl;
            if (s.in_check())
                cout << "Check!" << endl;

            s.print();
        } else {
            cout << "Invalid move, please retry." << endl;
        }
    }
    s.print();

    GameState game_state = s.get_game_state();
    switch (game_state) {
        case GameState::Checkmate:
            cout << "Checkmate!" << endl;
            break;
        case GameState::Stalemate:
            cout << "Stalemate." << endl;
            break;
        case GameState::Resigned:
            cout << "Resign!" << endl;
            break;
        case GameState::Draw_Deadpos:
            cout << "Draw by dead position." << endl;
            break;
        case GameState::Draw_Repetition:
            cout << "Draw by threefold repetition." << endl;
            break;
        case GameState::Draw_Maxmoves:
            cout << "Draw by the fifty-moves rule." << endl;
            break;
        case GameState::Draw_Agreed:
            cout << "Players agreed to a draw." << endl;
            break;
    }

    if (game_state == GameState::Checkmate || game_state == GameState::Resigned)
        cout << (s.get_turn() == Black ? "White" : "Black") << " wins." << endl;
}
