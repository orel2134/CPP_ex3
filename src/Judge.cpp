// Judge.cpp - Implementation of the Judge class (Coup role: Judge)
#include <stdexcept>
#include "Judge.hpp"
#include "Game.hpp"

// Constructor: creates a new Judge player in the game
Judge::Judge(const std::string& name, Game* game)
    : Player(name, Role::Judge, game) {}

// Collect 1 coin (income)
void Judge::gather() { Player::gather(); }
// Take tax (Judge gets 2 coins)
void Judge::tax() { Player::tax(); }
// Arrest another player
void Judge::arrest(Player &target) { Player::arrest(target); }
// Pay bribe for extra action
void Judge::bribe() { Player::bribe(); }
// Perform a coup on another player
void Judge::coup(Player& target) { Player::coup(target); }
// Sanction another player
void Judge::sanction(Player& target) { Player::sanction(target); }
// Invest (not supported for Judge)
void Judge::invest() { throw std::logic_error("Judge cannot invest"); }
// Spy on another player (not supported for Judge)
void Judge::spyOn(Player& target) { throw std::logic_error("Judge cannot spyOn"); }
// Prevent a coup (not supported for Judge)
void Judge::preventCoup(Player& target) { throw std::logic_error("Judge cannot preventCoup"); }
// Judge bribe (Judge can cancel a bribe)
void Judge::judgeBribe(Player& target) { Player::judgeBribe(target); }
// Clear extra action status
void Judge::clearExtraAction() { Player::clearExtraAction(); }
// General block coup (not supported for Judge)
void Judge::generalBlockCoup(Player& attacker) { throw std::logic_error("Judge cannot generalBlockCoup"); }
// Block tax action (not supported for Judge)
void Judge::blockTax(Player& target) { throw std::logic_error("Judge cannot blockTax"); }
// Skip turn
void Judge::skipTurn() { Player::skipTurn(); }
//