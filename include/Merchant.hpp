// orel2744@gmail.com
// The Merchant class represents a player who receives bonus coins when starting their turn with 3 or more coins.
// Has all standard actions and a unique merchantBonus ability.

#pragma once
#include "Player.hpp"

class Merchant : public Player {
public:
    Merchant(const std::string& name, Game* game);
    virtual ~Merchant() = default;
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
    void merchantBonus() override;
    void blockTax(Player& target) override;
    void skipTurn() override;
};
//
