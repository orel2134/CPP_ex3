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
# 4. To run the GUI version (build and run in one step):
#    make gui
#
# 5. To check for memory leaks with valgrind :
#    make valgrind
#
# 6. To clean all build artifacts:
#    make clean

SRC_DIR := src
INC_DIR := include
ASSETS_DIR := assets
TESTS_DIR := tests
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
HDRS := $(wildcard $(INC_DIR)/*.hpp)

MAIN_SRC := $(SRC_DIR)/main.cpp
GUI_SRC := $(SRC_DIR)/main_gui.cpp

TEST_SRC := $(TESTS_DIR)/test_game.cpp

MAIN_EXE := $(BUILD_DIR)/main.exe
GUI_EXE := $(BUILD_DIR)/game_gui.exe
TEST_EXE := $(BUILD_DIR)/test_game.exe

CXX := g++
CXXFLAGS := -std=c++17 -I$(INC_DIR) -I$(TESTS_DIR) -Wall -Wextra -g
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# קבצי מקור ללא main.cpp ו-main_gui.cpp
SRCS_NO_MAIN := $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/main_gui.cpp, $(SRCS))

all: $(MAIN_EXE) $(GUI_EXE)

# בניית main.exe רק עם main.cpp
$(MAIN_EXE): $(SRCS_NO_MAIN) $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# בניית game_gui.exe רק עם main_gui.cpp
$(GUI_EXE): $(SRCS_NO_MAIN) $(SRC_DIR)/main_gui.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# בניית test_game.exe
$(TEST_EXE): $(SRCS_NO_MAIN) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TEST_EXE)
	@if command -v winpty >/dev/null 2>&1; then winpty $(TEST_EXE); \
	elif [ -f $(TEST_EXE) ]; then $(TEST_EXE); \
	else ./test_game; fi

# בניית והרצת הגרסה הגרפית (GUI) בפעולה אחת
.PHONY: gui

gui: $(GUI_EXE)
	./$(GUI_EXE)

clean:
	rm -f $(BUILD_DIR)/*.exe

.PHONY: all test clean
