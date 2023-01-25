default:
	mkdir -p bin
	g++ *.cpp -std=c++17 -o bin/chess.out
	./bin/chess.out