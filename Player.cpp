// Player.cpp – כולל Baron, Spy, General

#include "Player.hpp"
#include "Game.hpp"

#include <iostream>
#include <stdexcept>
using namespace std;

Player::Player(const std::string& name, Role role, Game* game)
    : name(name), role(role), coins(0), alive(true), game(game), extraAction(false) {
    if (!game) throw std::invalid_argument("Player must be assigned to a game.");
    game->addPlayer(this);
}

const std::string& Player::getName() const { return name; }
Role Player::getRole() const { return role; }
int Player::getCoins() const { return coins; }
bool Player::isAlive() const { return alive; }

void Player::bribe() {
    if (!alive) throw std::logic_error("Dead player cannot bribe.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (coins < 4) throw std::logic_error("Not enough coins to bribe");

    coins -= 4;
    extraAction = true;
    game->markBribe(this);
    std::cout << name << " paid 4 coins to bribe and earned an extra action.\n";
}

void Player::sanction(Player& target) {
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

void Player::coup(Player& target) {
    if (this == &target) throw std::logic_error("Cannot coup yourself");
    if (!alive || !target.isAlive()) throw std::logic_error("Both players must be alive");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn");
    if (coins < 7) throw std::logic_error("Not enough coins to perform a coup");

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

void Player::invest() {
    if (!alive) throw std::logic_error("Dead player cannot invest");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn");
    if (role != Role::Baron) throw std::logic_error("Only a Baron can invest");
    if (coins < 3) throw std::logic_error("Not enough coins to invest");
    coins -= 3; coins += 6;
    std::cout << name << " invested and gained 6 coins" << std::endl;
    endTurn();
}

void Player::spyOn(Player& target) {
    if (!alive || !target.isAlive()) throw std::logic_error("Both players must be alive");
    if (role != Role::Spy) throw std::logic_error("Only Spy can spy.");
    std::cout << name << " spies on " << target.getName() << ": " << target.getCoins() << " coins." << std::endl;
    game->blockArrest(&target);
    endTurn();
}

void Player::preventCoup(Player& target) {
    if (!alive || !target.isAlive()) throw std::logic_error("Both players must be alive.");
    if (role != Role::General) throw std::logic_error("Only General can prevent coup.");
    if (coins < 5) throw std::logic_error("Not enough coins to block coup.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn");

    coins -= 5;
    game->blockCoup(&target);
    std::cout << name << " (General) blocked coup against " << target.getName() << "." << std::endl;
    endTurn();
}

void Player::removeCoins(int amount) {
    if (coins < amount) throw std::logic_error("Not enough coins.");
    coins -= amount;
}

void Player::eliminate() {
    alive = false;
    std::cout << name << " has been eliminated." << std::endl;
}

void Player::endTurn() {
    if (extraAction) {
        extraAction = false;
        return;
    }
    game->clearCoupMarks();
    game->nextTurn();
}


void Player::judgeBribe(Player& target) {
    if (!alive || !target.isAlive())
        throw std::logic_error("Both players must be alive.");
    if (role != Role::Judge)
        throw std::logic_error("Only Judge can cancel bribes.");
    if (!game->wasBribeUsedBy(&target))
        throw std::logic_error("No bribe to cancel.");

    game->cancelBribe(&target);
    std::cout << name << " canceled bribe by " << target.getName() << std::endl;
}

void Player::clearExtraAction() {
    extraAction = false;
}

void Player::generalBlockCoup(Player& attacker) {
    if (this->role != Role::General) throw std::logic_error("Only General can block coups.");
    if (!game->canBlockCoup(this)) throw std::logic_error("No coup to block.");
    if (coins < 5) throw std::logic_error("General needs 5 coins to block coup.");

    coins -= 5;
    attacker.addCoins(7);
    game->cancelCoup(this);

    std::cout << name << " blocked the coup by " << attacker.getName() << " and paid 5 coins.\n";
}

void Player::merchantBonus() {
    if (role != Role::Merchant) return;
    if (coins >= 3) {
        coins += 1;
        std::cout << name << " (Merchant) gained 1 bonus coin for starting with 3+.\n";
    }
}

void Player::gather() {
    if (!alive) throw std::logic_error("Dead player cannot gather.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (game->isSanctioned(this)) throw std::logic_error("You are sanctioned and cannot gather.");

    merchantBonus();
    coins += 1;
    std::cout << name << " gathered 1 coin.\n";
    endTurn();
}

void Player::tax() {
    if (!alive) throw std::logic_error("Dead player cannot tax.");
    if (!game->isPlayerTurn(this)) throw std::logic_error("Not your turn.");
    if (game->isSanctioned(this)) throw std::logic_error("You are sanctioned and cannot tax.");

    merchantBonus();
    int amount = 2;
    if (role == Role::Governor) amount = 3;
    coins += amount;
    std::cout << name << " taxed and got " << amount << " coins.\n";
    endTurn();
}

void Player::arrest(Player& target) {
    if (!alive || !target.isAlive()) throw logic_error("Both players must be alive.");
    if (!game->isPlayerTurn(this)) throw logic_error("Not your turn.");
    if (game->wasArrestedByMeLastTurn(this, &target))
        throw std::logic_error("Cannot arrest same player twice in a row.");

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

void Player::addCoins(int amount) {
    coins += amount;
}
///