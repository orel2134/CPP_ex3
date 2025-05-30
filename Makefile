# Makefile
#
# Usage instructions:
#
# 1. Open a terminal and navigate to the project directory:
#    cd CPP_ex3
#
# 2. To build all executables (main, GUI, and both test suites):
#    make
#
# 3. To run the main test suite (test_game):
#    make test_game
#
# 4. To run the roles test suite (test_roles):
#    make test_roles
#
# 5. To run all tests (both test_game and test_roles, sequentially):
#    make test
#
# 6. To run the GUI version (build and run in one step):
#    make gui
#
# 7. To run the main demo executable:
#    make make_main
#
# 8. To clean all build artifacts:
#    make clean
#
# Note: All tests use the doctest framework.
#
# -----------------------------------------------------------------------------
# Executable and directory definitions

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
TEST_ROLES_SRC := $(TESTS_DIR)/test_roles.cpp

MAIN_EXE := $(BUILD_DIR)/main.exe
GUI_EXE := $(BUILD_DIR)/game_gui.exe
TEST_EXE := $(BUILD_DIR)/test_game.exe
TEST_ROLES_EXE := $(BUILD_DIR)/test_roles.exe

CXX := g++
CXXFLAGS := -std=c++17 -I$(INC_DIR) -I$(TESTS_DIR) -Wall -Wextra -g
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Source files excluding main.cpp and main_gui.cpp
SRCS_NO_MAIN := $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/main_gui.cpp, $(SRCS))

all: $(MAIN_EXE) $(GUI_EXE)

# Build main.exe with main.cpp only
$(MAIN_EXE): $(SRCS_NO_MAIN) $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build game_gui.exe with main_gui.cpp only
$(GUI_EXE): $(SRCS_NO_MAIN) $(SRC_DIR)/main_gui.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Build test_game.exe
$(TEST_EXE): $(SRCS_NO_MAIN) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build test_roles.exe
$(TEST_ROLES_EXE): $(SRCS_NO_MAIN) $(TEST_ROLES_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run only test_game
.PHONY: test_game

test_game: $(TEST_EXE)
	@if command -v winpty >/dev/null 2>&1; then winpty $(TEST_EXE); \
	elif [ -f $(TEST_EXE) ]; then $(TEST_EXE); \
	else ./test_game.exe; fi

# Run only test_roles
.PHONY: test_roles

test_roles: $(TEST_ROLES_EXE)
	@if command -v winpty >/dev/null 2>&1; then winpty $(TEST_ROLES_EXE); \
	elif [ -f $(TEST_ROLES_EXE) ]; then $(TEST_ROLES_EXE); \
	else ./test_roles.exe; fi

# Run both test suites sequentially
.PHONY: test

test: test_game test_roles

# Build and run the GUI version in one step
.PHONY: gui

gui: $(GUI_EXE)
	./$(GUI_EXE)

# Run the main demo executable
.PHONY: make_main

make_main: $(MAIN_EXE)
	@if command -v winpty >/dev/null 2>&1; then winpty $(MAIN_EXE); \
	elif [ -f $(MAIN_EXE) ]; then $(MAIN_EXE); \
	else ./main.exe; fi

# Run valgrind on the main executable (Linux/Mac only)
.PHONY: valgrind

valgrind: $(MAIN_EXE)
	valgrind --leak-check=full $(MAIN_EXE)

clean:
	rm -f $(BUILD_DIR)/*.exe

.PHONY: all test clean
