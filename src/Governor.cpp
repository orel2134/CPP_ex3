// Governor.cpp - Implementation of the Governor class (Coup role: Governor)
#include <stdexcept>
#include "Governor.hpp"
#include "Game.hpp"

// Constructor: creates a new Governor player in the game
// @param name - the player's name
// @param game - pointer to the game instance
Governor::Governor(const std::string& name, Game* game)
    : Player(name, Role::Governor, game) {}

// Collect 1 coin (income)
void Governor::gather() { Player::gather(); }
// Take tax (Governor gets 3 coins)
void Governor::tax() { Player::tax(); }
// Arrest another player
void Governor::arrest(Player &target) { Player::arrest(target); }
// Pay bribe for extra action
void Governor::bribe() { Player::bribe(); }
// Perform a coup on another player
void Governor::coup(Player& target) { Player::coup(target); }
// Sanction another player
void Governor::sanction(Player& target) { Player::sanction(target); }
// Invest (not supported for Governor)
void Governor::invest() { throw std::logic_error("Governor cannot invest"); }
// Spy on another player (not supported for Governor)
void Governor::spyOn(Player& target) { throw std::logic_error("Governor cannot spyOn"); }
// Prevent a coup (not supported for Governor)
void Governor::preventCoup(Player& target) { throw std::logic_error("Governor cannot preventCoup"); }
// Judge bribe (not supported for Governor)
void Governor::judgeBribe(Player& target) { throw std::logic_error("Governor cannot judgeBribe"); }
// Clear extra action status
void Governor::clearExtraAction() { Player::clearExtraAction(); }
// General block coup (not supported for Governor)
void Governor::generalBlockCoup(Player& attacker) { throw std::logic_error("Governor cannot generalBlockCoup"); }
// Block tax action (Governor can block tax)
void Governor::blockTax(Player& target) { Player::blockTax(target); }
// Skip turn
void Governor::skipTurn() { Player::skipTurn(); }
//