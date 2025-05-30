# Coup Game - C++ Implementation

## Assignment-Specific Features (2025 Version)
This implementation fully matches the advanced assignment requirements:

- **Random Role Assignment:** At game start, each player is assigned a random role (Governor, Spy, Baron, General, Judge, Merchant). Duplicates are allowed.
- **Central Coin Bank (kupa):** All coins are drawn from and paid to a central bank. The bank starts with 50 coins by default. Player actions update both their own coin count and the bank.
- **Universal Actions:** All players can perform any base action (gather, tax, bribe, arrest, sanction, coup), with role-specific effects and blocks.
- **Strict Rule Enforcement:** All illegal actions (e.g., acting out of turn, insufficient coins, illegal blocks) throw clear exceptions.
- **Player Elimination:** Eliminated players are fully removed from the game and cannot block or act further.
- **Game End:** The last remaining player is declared the winner.

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
make make_main
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

### Clean Build Artifacts
```bash
make clean
```

## Implementation Details & Assignment Fulfillment

### Class Structure & Design
- **Game:** Manages the overall game state, player list, turn order, and win condition. Handles player elimination and ensures only the current player can act.
- **Player (Base Class):** Abstracts common player logic, coin management, and basic actions (gather, coup, etc.).
- **Role (Base Class):** Provides a polymorphic interface for all roles, allowing easy extension and role-specific logic.
- **Baron, General, Governor, Judge, Merchant, Spy:** Each inherits from Player and implements unique actions, blocks, and special rules as required by the assignment.

### Game Logic & Turn Management
- The game enforces strict turn order. Only the current player can act; others must wait for their turn.
- After each action, the turn advances automatically, unless an action requires resolution (e.g., blocks, counteractions).
- Player elimination is handled by removing the player from the active list and skipping their turns.

### Role Actions & Special Abilities
- **Baron:** Can invest coins (requires 3 coins, returns 6). Throws if not enough coins.
- **General:** Can block a coup targeting themselves (if not already eliminated). Throws if not targeted.
- **Governor:** Can tax for 3 coins, but cannot tax twice in a row without resolving the previous tax.
- **Judge:** Can cancel a bribe immediately after it is performed, before the briber ends their turn. Throws if no bribe is active.
- **Merchant:** Gets a bonus coin if starting a turn with 3 or more coins. No bonus otherwise.
- **Spy:** Can spy on other players (not themselves) and can block arrest only if they have spied on the arresting player.

### Exception Handling & Edge Cases
- All illegal actions (acting out of turn, insufficient coins, double-taxing, self-targeting, etc.) throw exceptions with clear messages.
- The code is robust against invalid state transitions and enforces all game rules strictly.

### Testing & Validation
- **Comprehensive Unit Tests:**
  - `tests/test_roles.cpp` covers all unique actions, edge cases, and error conditions for each role.
  - `tests/test_game.cpp` covers general game flow, turn order, elimination, and classic Coup logic.
- **Automated Test Execution:** All tests can be run with `make test` or individually with `make test_roles` and `make test_game`. Tests use the doctest framework and provide clear output for failures.
- **Test Coverage:** Tests ensure that:
  - Only the current player can act.
  - All role-specific actions and blocks work as intended.
  - Illegal actions throw exceptions.
  - Turn order and elimination are enforced.

### Makefile & Project Structure
- The Makefile provides clear targets for all main actions:
  - `make` - Build all executables.
  - `make make_main` - Build and run the main demo executable.
  - `make gui` - Build and run the GUI version.
  - `make test` - Run all tests.
  - `make test_roles` - Run only the role-specific tests.
  - `make test_game` - Run only the general game tests.
  - `make clean` - Remove all build artifacts.
  - `make valgrind` - Run valgrind on the main executable (Linux/Mac only).
- Usage instructions are provided at the top of the Makefile and in this README.

### Documentation & Code Quality
- All classes and methods are documented in English, with clear comments explaining logic and edge cases.
- The code is modular, extensible, and ready for further development or sharing on GitHub.
- The user's email is included in all source and header files for attribution.

## Important Tests
- **test_roles.cpp:**
  - Verifies all unique actions and edge cases for Baron, General, Governor, Judge, Merchant, and Spy.
  - Ensures correct turn order, action legality, and exception handling.
  - Checks that illegal actions (e.g., acting out of turn, double-taxing, investing without coins) throw exceptions.
  - Validates that special blocks and counteractions work as intended.
- **test_game.cpp:**
  - Covers general game flow and classic Coup logic.

## Notes
- All tests are automated and can be run with `make test_roles` or `make test`.
- The project is modular and easy to extend with new roles or actions.
- For any logic or test failures, check the test output for clear error messages.

Enjoy playing and extending Coup!
