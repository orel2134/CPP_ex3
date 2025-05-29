# Coup Game - C++ Implementation

## Game Logic Overview
This project implements an advanced version of the classic game "Coup" in C++. The game is played by 2 or more players, each with a secret role and unique abilities. The main goal is to eliminate all other players and be the last one standing.

### Key Concepts
- **Turn-based:** Players act in a fixed order. Only the current player can perform actions.
- **Coins:** Players collect coins to perform powerful actions (like coup).
- **Roles:** Each player is assigned a role (Baron, General, Governor, Judge, Merchant, Spy), each with unique actions and blocks.
- **Actions:** Players can gather coins, perform special actions, block others, or launch a coup to eliminate a rival.
- **Blocking & Counteractions:** Some roles can block or counter specific actions of others.
- **Elimination:** When a player is couped, they are removed from the game and skip their turns.

### New Roles (Beyond Classic Coup)
- **Baron:** Can invest coins for a big return.
- **General:** Can block coups against themselves.
- **Governor:** Can tax for extra coins, but not twice in a row without resolving the previous action.
- **Judge:** Can cancel a bribe if done immediately after the bribe action.
- **Merchant:** Gets a bonus coin if starting a turn with 3+ coins.
- **Spy:** Can spy on others and interact with arrest mechanics.

## Building and Running

### Requirements
- C++17 compiler (e.g., g++)
- [doctest](https://github.com/doctest/doctest) (included in `tests/doctest.h`)
- (Optional) SFML for GUI

### Build All
```bash
make
```

### Run Main Game (Console)
```bash
make main
./build/main.exe
```

### Run GUI Version
```bash
make gui
```

### Run All Tests
```bash
make test
```

### Run Role-Specific Tests
```bash
make test_roles
```

## Important Tests
- **test_roles.cpp:**
  - Verifies all unique actions and edge cases for Baron, General, Governor, Judge, Merchant, and Spy.
  - Ensures correct turn order, action legality, and exception handling.
  - Checks that illegal actions (e.g., acting out of turn, double-taxing, investing without coins) throw exceptions.
  - Validates that special blocks and counteractions work as intended.
- **test_game.cpp:**
  - (Not detailed here) General game flow and classic Coup logic.

## Notes
- All tests are automated and can be run with `make test_roles` or `make test`.
- The project is modular and easy to extend with new roles or actions.
- For any logic or test failures, check the test output for clear error messages.

Enjoy playing and extending Coup!
