// orel2744@gmail.com
// The Judge class represents a player who can cancel bribes and receives coins when sanctioned.
// Has all standard actions and unique bribe-blocking abilities.

#pragma once
#include "Player.hpp"

class Judge : public Player {
public:
    Judge(const std::string& name, Game* game);
    virtual ~Judge() = default;
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
