// orel2744@gmail.com
// Baron.cpp - Implementation of the Baron class (Coup role: Baron)
#include "Baron.hpp"
#include "Game.hpp"
#include <stdexcept>

/**
 * @brief Construct a new Baron player.
 * @param name The player's name.
 * @param game Pointer to the game instance.
 */
Baron::Baron(const std::string& name, Game* game)
    : Player(name, Role::Baron, game) {}

/**
 * @brief Collect 1 coin (income).
 */
void Baron::gather() { Player::gather(); }
/**
 * @brief Take tax (Baron gets 2 coins).
 */
void Baron::tax() { Player::tax(); }
/**
 * @brief Arrest another player.
 * @param target The player to arrest.
 */
void Baron::arrest(Player &target) { Player::arrest(target); }
/**
 * @brief Pay bribe for extra action.
 */
void Baron::bribe() { Player::bribe(); }
/**
 * @brief Perform a coup on another player.
 * @param target The player to coup.
 */
void Baron::coup(Player& target) { Player::coup(target); }
/**
 * @brief Sanction another player.
 * @param target The player to sanction.
 */
void Baron::sanction(Player& target) { Player::sanction(target); }
/**
 * @brief Invest (Baron can use this ability).
 */
void Baron::invest() { Player::invest(); }
/**
 * @brief Spy on another player (not supported for Baron).
 */
void Baron::spyOn(Player& target) { throw std::logic_error("Baron cannot spyOn"); }
/**
 * @brief Prevent a coup (not supported for Baron).
 */
void Baron::preventCoup(Player& target) { throw std::logic_error("Baron cannot preventCoup"); }
/**
 * @brief Judge bribe (not supported for Baron).
 */
void Baron::judgeBribe(Player& target) { throw std::logic_error("Baron cannot judgeBribe"); }
/**
 * @brief Clear extra action status.
 */
void Baron::clearExtraAction() { Player::clearExtraAction(); }
/**
 * @brief General block coup (not supported for Baron).
 */
void Baron::generalBlockCoup(Player& attacker) { throw std::logic_error("Baron cannot generalBlockCoup"); }
/**
 * @brief Block tax action (not supported for Baron).
 */
void Baron::blockTax(Player& target) { throw std::logic_error("Baron cannot blockTax"); }
/**
 * @brief Skip turn.
 */
void Baron::skipTurn() { Player::skipTurn(); }
