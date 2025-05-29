// Merchant.cpp - Implementation of the Merchant class (Coup role: Merchant)
#include <iostream>
#include <stdexcept>
#include "Merchant.hpp"
#include "Game.hpp"

/**
 * @brief Construct a new Merchant player.
 * @param name The player's name.
 * @param game Pointer to the game instance.
 */
Merchant::Merchant(const std::string& name, Game* game)
    : Player(name, Role::Merchant, game) {}

/**
 * @brief Collect 1 coin (income).
 */
void Merchant::gather() { Player::gather(); }
/**
 * @brief Take tax (Merchant gets 2 coins).
 */
void Merchant::tax() { Player::tax(); }
/**
 * @brief Arrest another player.
 * @param target The player to arrest.
 */
void Merchant::arrest(Player &target) { Player::arrest(target); }
/**
 * @brief Pay bribe for extra action.
 */
void Merchant::bribe() { Player::bribe(); }
/**
 * @brief Perform a coup on another player.
 * @param target The player to coup.
 */
void Merchant::coup(Player& target) { Player::coup(target); }
/**
 * @brief Sanction another player.
 * @param target The player to sanction.
 */
void Merchant::sanction(Player& target) { Player::sanction(target); }
/**
 * @brief Invest (not supported for Merchant).
 */
void Merchant::invest() { throw std::logic_error("Merchant cannot invest"); }
/**
 * @brief Spy on another player (not supported for Merchant).
 */
void Merchant::spyOn(Player& target) { throw std::logic_error("Merchant cannot spyOn"); }
/**
 * @brief Prevent a coup (not supported for Merchant).
 */
void Merchant::preventCoup(Player& target) { throw std::logic_error("Merchant cannot preventCoup"); }
/**
 * @brief Judge bribe (not supported for Merchant).
 */
void Merchant::judgeBribe(Player& target) { throw std::logic_error("Merchant cannot judgeBribe"); }
/**
 * @brief Clear extra action status.
 */
void Merchant::clearExtraAction() { Player::clearExtraAction(); }
/**
 * @brief General block coup (not supported for Merchant).
 */
void Merchant::generalBlockCoup(Player& attacker) { throw std::logic_error("Merchant cannot generalBlockCoup"); }
/**
 * @brief Merchant bonus: gain 1 coin if starting turn with 3+ coins.
 */
void Merchant::merchantBonus() {
    if (getCoins() >= 3) {
        addCoins(1);
        std::cout << getName() << " (Merchant) gained 1 bonus coin for starting with 3+.\n";
    }
}
/**
 * @brief Block tax action (not supported for Merchant).
 */
void Merchant::blockTax(Player& target) { throw std::logic_error("Merchant cannot blockTax"); }
/**
 * @brief Skip turn.
 */
void Merchant::skipTurn() { Player::skipTurn(); }
//
