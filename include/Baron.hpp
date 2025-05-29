#pragma once
#include "Player.hpp"

class Baron : public Player {
public:
    Baron(const std::string& name, Game* game);
    virtual ~Baron() = default;
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