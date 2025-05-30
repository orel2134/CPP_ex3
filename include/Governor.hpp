// orel2744@gmail.com
// The Governor class represents a player who gains extra coins from tax and can block other players' tax actions.
// Has all standard actions and unique tax-related abilities.

#pragma once
#include "Player.hpp"

class Governor : public Player {
public:
    Governor(const std::string& name, Game* game);
    virtual ~Governor() = default;
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
