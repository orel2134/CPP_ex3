// orel2744@gmail.com
// The Spy class represents a player with the unique ability to spy on other players and perform special actions.
// Responsibilities:
// - Can use spyOn to reveal information about other players.
// - Can perform all standard player actions (gather, tax, coup, etc.).
// - May have unique interactions with bribe, sanction, and other role-specific actions.
// - Implements all required overrides for the game's action system.
// The Spy's role is to gather intelligence and influence the game through information and special moves.

#pragma once
#include "Player.hpp"

class Spy : public Player {
public:
    Spy(const std::string& name, Game* game);
    virtual ~Spy() = default;
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
