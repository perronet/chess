GAME_DIR = game
BIN_DIR = bin

default:
	mkdir -p $(BIN_DIR)
	g++ $(GAME_DIR)/*.cpp -std=c++17 -o $(BIN_DIR)/chess.out
	./$(BIN_DIR)/chess.out

clean:
	rm -rf $(BIN_DIR)