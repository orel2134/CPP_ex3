all: test_game game_gui

test_game: test_game.cpp Game.cpp Player.cpp Game.hpp Player.hpp Role.hpp
	@g++ test_game.cpp Game.cpp Player.cpp -o test_game -std=c++17

game_gui: main_gui.cpp Game.cpp Player.cpp Game.hpp Player.hpp Role.hpp
	@g++ main_gui.cpp Game.cpp Player.cpp -o game_gui -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system

clean:
	@rm -f test_game game_gui
