# Makefile
#
# Usage instructions:
#
# 1. Open a terminal and navigate to the project directory:
#    cd CPP_ex3
#
# 2. To build both the test and GUI executables:
#    make
#
# 3. To run all tests:
#    make test
#
# 4. To run the GUI version:
#    make Main
#
# 5. To check for memory leaks with valgrind :
#    make valgrind
#
# 6. To clean all build artifacts:
#    make clean


CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = Game.cpp Player.cpp Role.cpp
HEADERS = Game.hpp Player.hpp Role.hpp doctest.h

all: test_game game_gui

test_game: test_game.cpp $(SRC) $(HEADERS)
	$(CXX) test_game.cpp $(SRC) -o test_game $(CXXFLAGS)

game_gui: main_gui.cpp $(SRC) $(HEADERS)
	$(CXX) main_gui.cpp $(SRC) -o game_gui $(CXXFLAGS) $(LDFLAGS)

Main: game_gui
	./game_gui.exe

test: test_game
	@if command -v winpty >/dev/null 2>&1; then winpty ./test_game.exe; \
	elif [ -f ./test_game.exe ]; then ./test_game.exe; \
	else ./test_game; fi

valgrind: test_game
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./test_game.exe

clean:
	rm -f test_game game_gui
