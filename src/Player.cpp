// Player.cpp –  include Baron, Spy, General

#include "Player.hpp"
#include "Game.hpp"

#include <iostream>
#include <stdexcept>
using namespace std;

/**
 * @brief Constructs a new Player object, assigns it a name, role, and game, and registers it in the game.
 *        Throws if the game pointer is null. This constructor ensures that every player is always associated
 *        with a game and is automatically added to the game's player list upon creation.
 * @param name The name of the player (unique identifier in the game).
 * @param role The role assigned to the player (Governor, Spy, Baron, etc.).
 * @param game Pointer to the Game object this player belongs to.
 * @throws std::invalid_argument if the game pointer is null.
 */
Player::Player(const std::string& name, Role role, Game* game)
    : name(name), role(role), coins(0), alive(true), game(game), extraAction(false), pendingAction(PendingAction::None) {
    if (!game) throw std::invalid_argument("Player must be assigned to a game.");
    game->addPlayer(this);
}

/**
 * @brief Gets the name of the player.
 * @return Reference to the player's name string.
 */
const std::string& Player::getName() const { return name; }

/**
 * @brief Gets the role of the player (Governor, Spy, etc.).
 * @return The player's role as an enum value.
 */
Role Player::getRole() const { return role; }

/**
 * @brief Gets the current number of coins the player has.
 * @return The player's coin count.
 */
int Player::getCoins() const { return coins; }

/**
 * @brief Checks if the player is currently alive in the game.
 * @return True if alive, false if eliminated.
 */
bool Player::isAlive() const { return alive; }

/**
 * @brief Pays 4 coins to bribe and gain an extra action this turn. Only possible if alive, on turn, and has enough coins.
 *        Marks the bribe in the game so it can be tracked and possibly canceled by a Judge. Throws if not allowed.
 * @throws std::logic_error if player is dead, not their turn, or not enough coins.
 */
void Player::bribe() {
    if (!alive) throw std::logic_error("Dead player cannot bribe.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (coins < 4) throw std::logic_error("Not enough coins to bribe");
    if (extraAction) throw std::logic_error("Already bribed this turn");
    if (pendingAction != PendingAction::None) throw std::logic_error("You must resolve previous action (tax/bribe) before new action.");

    coins -= 4;
    extraAction = true;
    game->markBribe(this);
    std::cout << name << " paid 4 coins to bribe and earned an extra action.\n";
    pendingAction = PendingAction::Bribe;
    // לא מסיים תור, כי מותר לבצע פעולה נוספת
}

/**
 * @brief Pays 3 coins to sanction another player, preventing them from acting on their next turn. Only possible if both players are alive, on turn, and enough coins.
 *        If the target is a Judge, 1 coin is added to the bank. Throws if not allowed.
 * @param target The player to sanction.
 * @throws std::logic_error if either player is dead, not your turn, or not enough coins.
 */
void Player::sanction(Player& target) {
    if (this == &target) throw std::logic_error("Cannot sanction yourself");
    if (!alive || !target.isAlive()) throw std::logic_error("Both players must be alive");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn");
    if (coins < 3) throw std::logic_error("Not enough coins to sanction");
    if (target.getRole() == Role::Judge) {
        game->addCoinsToBank(1);
    }
    coins -= 3;
    game->applySanction(&target);
    std::cout << name << " sanctioned " << target.getName() << "." << std::endl;
    endTurn();
}

/**
 * @brief Performs a coup on another player, eliminating them from the game. Costs 7 coins. Throws if not allowed.
 *        If the coup is blocked by a General, coins are still lost but the coup fails. Only possible if both players are alive, on turn, and enough coins.
 * @param target The player to coup.
 * @throws std::logic_error if self-coup, either player is dead, not your turn, or not enough coins.
 */
void Player::coup(Player& target) {
    if (this == &target) throw std::logic_error("Cannot coup yourself");
    if (!alive || !target.isAlive()) throw std::logic_error("Both players must be alive");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn");
    if (coins < 7) throw std::logic_error("Not enough coins to perform a coup");
    if (pendingAction != PendingAction::None) {
        pendingAction = PendingAction::None;
        extraAction = false;
    }
    game->registerCoupAttempt(this, &target);
    if (game->isCoupBlocked(&target)) {
        coins -= 7;
        std::cout << name << " tried to coup " << target.getName() << " but it was blocked! Coins lost." << std::endl;
        game->cancelCoup(&target);
        endTurn();
        return;
    }
    coins -= 7;
    game->eliminate(&target);
    std::cout << name << " performed a coup on " << target.getName() << "." << std::endl;
    endTurn();
}

/**
 * @brief Allows a Baron to invest 3 coins and gain 6 coins in return. Only possible for Barons, on their turn, if alive and enough coins.
 * @throws std::logic_error if not a Baron, not enough coins, not your turn, or dead.
 */
void Player::invest() {
    if (!alive) throw std::logic_error("Dead player cannot invest");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn");
    if (role != Role::Baron) throw std::logic_error("Only a Baron can invest");
    if (coins < 3) throw std::logic_error("Not enough coins to invest");
    coins -= 3; coins += 6;
    std::cout << name << " invested and gained 6 coins" << std::endl;
    endTurn();
}

/**
 * @brief Allows a Spy to spy on another player, revealing their coin count and blocking arrest on them for the next turn. Only possible for Spies, if both players are alive.
 * @param target The player to spy on.
 * @throws std::logic_error if not a Spy, either player is dead.
 */
void Player::spyOn(Player& target) {
    if (!alive || !target.isAlive())
        throw std::logic_error("Both players must be alive.");
    if (role != Role::Spy)
        throw std::logic_error("Only Spy can use spyOn.");
    if (&target == this)
        throw std::logic_error("Cannot spy on yourself.");

    std::cout << name << " spies on " << target.getName() << ": " << target.getCoins() << " coins." << std::endl;
    game->blockArrest(&target);
    
}

/**
 * @brief Allows a General to prevent a coup against a target by paying 5 coins. Only possible for Generals, on their turn, if both players are alive, and enough coins.
 * @param target The player whose coup is being prevented.
 * @throws std::logic_error if not a General, not your turn, not enough coins, or either player is dead.
 */
void Player::preventCoup(Player& target) {
    if (!alive || !target.isAlive()) throw std::logic_error("Both players must be alive.");
    if (role != Role::General) throw std::logic_error("Only General can prevent coup.");
    if (coins < 5) throw std::logic_error("Not enough coins to block coup.");

    coins -= 5;
    game->blockCoup(&target);
    std::cout << name << " (General) blocked coup against " << target.getName() << "." << std::endl;
    endTurn();
}

/**
 * @brief Removes a specified amount of coins from the player. Throws if not enough coins.
 * @param amount The number of coins to remove.
 * @throws std::logic_error if not enough coins.
 */
void Player::removeCoins(int amount) {
    if (coins < amount) throw std::logic_error("Not enough coins.");
    coins -= amount;
}

/**
 * @brief Eliminates the player from the game, setting their alive status to false.
 */
void Player::eliminate() {
    alive = false;
    std::cout << name << " has been eliminated." << std::endl;
}

/**
 * @brief Ends the player's turn. If the player has an extra action (from bribe), consumes it instead of ending the turn. Otherwise, advances the game turn.
 */
void Player::endTurn() {
    if (extraAction) {
        extraAction = false;
        return;
    }
    game->clearCoupMarks();
    game->nextTurn();
}

/**
 * @brief Allows a Judge to cancel a bribe used by another player this turn. Only possible for Judges, if both players are alive, and if the target used a bribe.
 * @param target The player whose bribe is being canceled.
 * @throws std::logic_error if not a Judge, either player is dead, or no bribe to cancel.
 */
void Player::judgeBribe(Player& target) {
    if (!alive || !target.isAlive())
        throw std::logic_error("Both players must be alive.");
    if (role != Role::Judge)
        throw std::logic_error("Only Judge can cancel bribes.");
    if (!game->wasBribeUsedBy(&target))
        throw std::logic_error("No bribe to cancel.");
    if (target.pendingAction == PendingAction::Bribe) {
        target.extraAction = false;
        target.pendingAction = PendingAction::None;
        game->cancelBribe(&target);
        std::cout << name << " canceled bribe by " << target.getName() << std::endl;
        target.endTurn();
    } else {
        throw std::logic_error("No pending bribe to block.");
    }
}

/**
 * @brief Clears the player's extra action status (used when a bribe is canceled).
 */
void Player::clearExtraAction() {
    extraAction = false;
}

/**
 * @brief Allows a General to block a coup attempt against them by paying 5 coins. The attacker is refunded the coup cost. Only possible for Generals, if a coup can be blocked, and enough coins.
 * @param attacker The player who attempted the coup.
 * @throws std::logic_error if not a General, no coup to block, or not enough coins.
 */
void Player::generalBlockCoup(Player& attacker) {
    if (this->role != Role::General) throw std::logic_error("Only General can block coups.");
    if (!game->canBlockCoup(this)) throw std::logic_error("No coup to block.");
    if (coins < 5) throw std::logic_error("General needs 5 coins to block coup.");
    coins -= 5;
    // החזר בדיוק 7 מטבעות לתוקף (רק אם ירדו לו)
    // נוודא שהתוקף לא מקבל יותר מדי מטבעות
    if (attacker.getCoins() < 7) {
        attacker.addCoins(7 - attacker.getCoins());
    } else {
        attacker.addCoins(0); // לא להחזיר אם יש לו כבר 7 או יותר
    }
    game->cancelCoup(this);
    alive = true;
    std::cout << name << " blocked the coup by " << attacker.getName() << " and paid 5 coins.\n";
}

/**
 * @brief Allows the player to gather 1 coin. Merchants may receive a bonus. Throws if dead, not your turn, or sanctioned.
 * @throws std::logic_error if dead, not your turn, or sanctioned.
 */
void Player::gather() {
    if (!alive) throw std::logic_error("Dead player cannot gather.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (game->isSanctioned(this)) throw std::logic_error("You are sanctioned and cannot gather.");
    if (pendingAction != PendingAction::None) {
        pendingAction = PendingAction::None;
        extraAction = false;
    }
    merchantBonus(); 
    coins += 1;
    std::cout << name << " gathered 1 coin.\n";
    endTurn();
}

/**
 * @brief Allows the player to tax, gaining 2 coins (or 3 if Governor). Merchants may receive a bonus. Throws if dead, not your turn, or sanctioned.
 * @throws std::logic_error if dead, not your turn, or sanctioned.
 */
void Player::tax() {
    if (!alive) throw std::logic_error("Dead player cannot tax.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (game->isSanctioned(this)) throw std::logic_error("You are sanctioned and cannot tax.");
    if (pendingAction != PendingAction::None) throw std::logic_error("You must resolve previous action (tax/bribe) before new action.");

    merchantBonus();
    int amount = 2;
    if (role == Role::Governor) amount = 3;
    coins += amount;
    std::cout << name << " taxed and got " << amount << " coins.\n";
    game->markTax(this);
    pendingAction = PendingAction::Tax;
    if (!extraAction) {
        endTurn();
    }
    
}

/**
 * @brief Allows the player to arrest another player, taking coins from them or causing penalties based on their role. Throws if not allowed.
 * @param target The player to arrest.
 * @throws std::logic_error if either player is dead, not your turn, or other arrest rules are violated.
 */
void Player::arrest(Player& target) {
    if (!alive || !target.isAlive()) throw logic_error("Both players must be alive.");
    if (!game->isPlayerTurn(this)) throw logic_error("Not your turn.");
    if (game->wasArrestedByMeLastTurn(this, &target))
        throw std::logic_error("Cannot arrest same player twice in a row.");
    // חסימת arrest ע"י Spy
    if (target.getRole() == Role::Spy && game->isArrestBlocked(&target)) 
        throw logic_error("You have been blocked from using arrest this turn.");
    game->markArrest(this, &target);
    if (target.getRole() == Role::General) {
        cout << target.getName() << " is a General and negated the arrest.\n";
        endTurn();
        return;
    }
    if (target.getRole() == Role::Merchant) {
        if (target.getCoins() < 2) throw logic_error("Merchant doesn't have enough to pay arrest penalty.");
        target.removeCoins(2);
        cout << target.getName() << " is a Merchant and paid 2 coins to bank (arrest).\n";
        endTurn();
        return;
    }
    if (target.getCoins() > 0) {
        target.removeCoins(1);
        addCoins(1);
    }
    cout << getName() << " arrested " << target.getName() << " and took 1 coin.\n";
    endTurn();
}

/**
 * @brief Adds a specified amount of coins to the player (utility function).
 * @param amount The number of coins to add.
 */
void Player::addCoins(int amount) {
    coins += amount;
}

/**
 * @brief Allows a Governor to block a tax action performed by another player this turn. Only possible for Governors, if both players are alive, and if the target used tax this turn.
 * @param target The player whose tax is being blocked.
 * @throws std::logic_error if not a Governor, either player is dead, or no tax to block.
 */
void Player::blockTax(Player& target) {
    if (!alive || !target.isAlive())
        throw std::logic_error("Both players must be alive.");
    if (role != Role::Governor)
        throw std::logic_error("Only Governor can block tax.");
    if (!game->wasTaxUsedBy(&target))
        throw std::logic_error("No tax to block.");
    if (target.pendingAction == PendingAction::Tax) {
        int amount = (target.role == Role::Governor) ? 3 : 2;
        target.removeCoins(amount);
        target.pendingAction = PendingAction::None;
        target.extraAction = false;
        game->cancelTax(&target);
        std::cout << name << " blocked tax by " << target.getName() << std::endl;
        target.endTurn();
    } else {
        throw std::logic_error("No pending tax to block.");
    }
}

/**
 * @brief Skips the player's turn, resetting their extra action status if active. Throws if dead or not their turn.
 * @throws std::logic_error if dead, not your turn.
 */
void Player::skipTurn() {
    if (!alive) throw std::logic_error("Dead player cannot skip turn.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (extraAction) {
        extraAction = false;
        return;
    }
    game->nextTurn();
}