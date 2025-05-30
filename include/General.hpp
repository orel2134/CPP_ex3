// orel2744@gmail.com
// The General class represents a player who can block coups and prevent coups against others by paying coins.
// Has all standard actions and unique coup-blocking abilities.

#pragma once
#include "Player.hpp"

class General : public Player {
public:
    General(const std::string& name, Game* game);
    virtual ~General() = default;
    void gather() override;
    void tax() override;
    void arrest(Player &target) override;
    void bribe() override;
    void coup(Player& target) override;
    void sanction(Player& target) override;
    void invest() override;
    void spyOn(Player& target) override;
    void preventCoup(Player& target) override;
    void judgeBribe(Player& target) override;
    void clearExtraAction() override;
    void generalBlockCoup(Player& attacker) override;
    void blockTax(Player& target) override;
    void skipTurn() override;
};
//
