// General.cpp - Implementation of the General class (Coup role: General)
#include "General.hpp"
#include "Game.hpp"
#include <stdexcept>

/**
 * @brief Construct a new General player.
 * @param name The player's name.
 * @param game Pointer to the game instance.
 */
General::General(const std::string& name, Game* game)
    : Player(name, Role::General, game) {}

/**
 * @brief Collect 1 coin (income).
 */
void General::gather() { Player::gather(); }
/**
 * @brief Take tax (General gets 2 coins).
 */
void General::tax() { Player::tax(); }
/**
 * @brief Arrest another player.
 * @param target The player to arrest.
 */
void General::arrest(Player &target) { Player::arrest(target); }
/**
 * @brief Pay bribe for extra action.
 */
void General::bribe() { Player::bribe(); }
/**
 * @brief Perform a coup on another player.
 * @param target The player to coup.
 */
void General::coup(Player& target) { Player::coup(target); }
/**
 * @brief Sanction another player.
 * @param target The player to sanction.
 */
void General::sanction(Player& target) { Player::sanction(target); }
/**
 * @brief Invest (not supported for General).
 */
void General::invest() { throw std::logic_error("General cannot invest"); }
/**
 * @brief Spy on another player (not supported for General).
 */
void General::spyOn(Player& target) { throw std::logic_error("General cannot spyOn"); }
/**
 * @brief Prevent a coup (General can use this ability).
 * @param target The player whose coup is being prevented.
 */
void General::preventCoup(Player& target) { Player::preventCoup(target); }
/**
 * @brief Judge bribe (not supported for General).
 */
void General::judgeBribe(Player& target) { throw std::logic_error("General cannot judgeBribe"); }
/**
 * @brief Clear extra action status.
 */
void General::clearExtraAction() { Player::clearExtraAction(); }
/**
 * @brief General block coup (General can use this ability).
 * @param attacker The player who attempted the coup.
 */
void General::generalBlockCoup(Player& attacker) { Player::generalBlockCoup(attacker); }
/**
 * @brief Block tax action (not supported for General).
 */
void General::blockTax(Player& target) { throw std::logic_error("General cannot blockTax"); }
/**
 * @brief Skip turn.
 */
void General::skipTurn() { Player::skipTurn(); }
