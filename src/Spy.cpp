// orel2744@gmail.com
// Spy.cpp - Implementation of the Spy class (Coup role: Spy)
#include <stdexcept>
#include "Spy.hpp"
#include "Game.hpp"

/**
 * @brief Construct a new Spy player.
 * @param name The player's name.
 * @param game Pointer to the game instance.
 */
Spy::Spy(const std::string& name, Game* game)
    : Player(name, Role::Spy, game) {}

/**
 * @brief Collect 1 coin (income).
 */
void Spy::gather() { Player::gather(); }
/**
 * @brief Take tax (Spy gets 2 coins).
 */
void Spy::tax() { Player::tax(); }
/**
 * @brief Arrest another player.
 * @param target The player to arrest.
 */
void Spy::arrest(Player &target) { Player::arrest(target); }
/**
 * @brief Pay bribe for extra action.
 */
void Spy::bribe() { Player::bribe(); }
/**
 * @brief Perform a coup on another player.
 * @param target The player to coup.
 */
void Spy::coup(Player& target) { Player::coup(target); }
/**
 * @brief Sanction another player.
 * @param target The player to sanction.
 */
void Spy::sanction(Player& target) { Player::sanction(target); }
/**
 * @brief Invest (not supported for Spy).
 */
void Spy::invest() { throw std::logic_error("Spy cannot invest"); }
/**
 * @brief Spy on another player (Spy can use this ability).
 * @param target The player to spy on.
 */
void Spy::spyOn(Player& target) { Player::spyOn(target); }
/**
 * @brief Prevent a coup (not supported for Spy).
 */
void Spy::preventCoup(Player& target) { throw std::logic_error("Spy cannot preventCoup"); }
/**
 * @brief Judge bribe (not supported for Spy).
 */
void Spy::judgeBribe(Player& target) { throw std::logic_error("Spy cannot judgeBribe"); }
/**
 * @brief Clear extra action status.
 */
void Spy::clearExtraAction() { Player::clearExtraAction(); }
/**
 * @brief General block coup (not supported for Spy).
 */
void Spy::generalBlockCoup(Player& attacker) { throw std::logic_error("Spy cannot generalBlockCoup"); }
/**
 * @brief Block tax action (not supported for Spy).
 */
void Spy::blockTax(Player& target) { throw std::logic_error("Spy cannot blockTax"); }
/**
 * @brief Skip turn.
 */
void Spy::skipTurn() { Player::skipTurn(); }
