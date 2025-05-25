#pragma once

#include <string>
#include "Role.hpp"
class Game;

class Player {
private:
    std::string name;
    Role role;
    int coins;
    bool alive;
    Game* game;
    bool extraAction;

public:
    /**
     * @brief Constructs a new Player object, assigns a name, role, and game, and registers the player in the game.
     * @param name The player's name (unique identifier).
     * @param role The player's role (Governor, Spy, Baron, etc.).
     * @param game Pointer to the Game object this player belongs to.
     * @throws std::invalid_argument if the game pointer is null.
     */
    Player(const std::string& name, Role role, Game* game);

    /**
     * @brief Gets the player's name.
     * @return Reference to the player's name string.
     */
    const std::string& getName() const;
    /**
     * @brief Gets the player's role (Governor, Spy, etc.).
     * @return The player's role as an enum value.
     */
    Role getRole() const;
    /**
     * @brief Gets the current number of coins the player has.
     * @return The player's coin count.
     */
    int getCoins() const;
    /**
     * @brief Checks if the player is currently alive in the game.
     * @return True if alive, false if eliminated.
     */
    bool isAlive() const;

    /**
     * @brief Allows the player to gather 1 coin. Merchants may receive a bonus. Throws if dead, not your turn, or sanctioned.
     * @throws std::logic_error if dead, not your turn, or sanctioned.
     */
    void gather();
    /**
     * @brief Allows the player to tax, gaining 2 coins (or 3 if Governor). Merchants may receive a bonus. Throws if dead, not your turn, or sanctioned.
     * @throws std::logic_error if dead, not your turn, or sanctioned.
     */
    void tax();
    /**
     * @brief Allows the player to arrest another player, taking coins from them or causing penalties based on their role. Throws if not allowed.
     * @param target The player to arrest.
     * @throws std::logic_error if either player is dead, not your turn, or other arrest rules are violated.
     */
    void arrest(Player &target);
    /**
     * @brief Pays 4 coins to bribe and gain an extra action this turn. Only possible if alive, on turn, and has enough coins.
     *        Marks the bribe in the game so it can be tracked and possibly canceled by a Judge. Throws if not allowed.
     * @throws std::logic_error if player is dead, not their turn, or not enough coins.
     */
    void bribe();
    /**
     * @brief Performs a coup on another player, eliminating them from the game. Costs 7 coins. Throws if not allowed.
     *        If the coup is blocked by a General, coins are still lost but the coup fails. Only possible if both players are alive, on turn, and enough coins.
     * @param target The player to coup.
     * @throws std::logic_error if self-coup, either player is dead, not your turn, or not enough coins.
     */
    void coup(Player& target);
    /**
     * @brief Pays 3 coins to sanction another player, preventing them from acting on their next turn. Only possible if both players are alive, on turn, and enough coins.
     *        If the target is a Judge, 1 coin is added to the bank. Throws if not allowed.
     * @param target The player to sanction.
     * @throws std::logic_error if either player is dead, not your turn, or not enough coins.
     */
    void sanction(Player& target);
    /**
     * @brief Allows a Baron to invest 3 coins and gain 6 coins in return. Only possible for Barons, on their turn, and if alive and enough coins.
     * @throws std::logic_error if not a Baron, not enough coins, not your turn, or dead.
     */
    void invest(); //  Baron only
    /**
     * @brief Allows a Spy to spy on another player, revealing their coin count and blocking arrest on them for the next turn. Only possible for Spies, if both players are alive.
     * @param target The player to spy on.
     * @throws std::logic_error if not a Spy, either player is dead.
     */
    void spyOn(Player& target); //  Spy only
    /**
     * @brief Allows a General to prevent a coup against a target by paying 5 coins. Only possible for Generals, on their turn, if both players are alive, and enough coins.
     * @param target The player whose coup is being prevented.
     * @throws std::logic_error if not a General, not your turn, not enough coins, or either player is dead.
     */
    void preventCoup(Player& target); //  General only

    /**
     * @brief Adds a specified amount of coins to the player (utility function).
     * @param amount The number of coins to add.
     */
    void addCoins(int amount);
    /**
     * @brief Removes a specified amount of coins from the player. Throws if not enough coins.
     * @param amount The number of coins to remove.
     * @throws std::logic_error if not enough coins.
     */
    void removeCoins(int amount);
    /**
     * @brief Eliminates the player from the game, setting their alive status to false.
     */
    void eliminate();
    /**
     * @brief Ends the player's turn. If the player has an extra action (from bribe), consumes it instead of ending the turn. Otherwise, advances the game turn.
     */
    void endTurn();

    /**
     * @brief Allows a Judge to cancel a bribe used by another player this turn. Only possible for Judges, if both players are alive, and if the target used a bribe.
     * @param target The player whose bribe is being canceled.
     * @throws std::logic_error if not a Judge, either player is dead, or no bribe to cancel.
     */
    void judgeBribe(Player& target);

    /**
     * @brief Clears the player's extra action status (used when a bribe is canceled).
     */
    void clearExtraAction();

    /**
     * @brief Allows a General to block a coup attempt against them by paying 5 coins. The attacker is refunded the coup cost. Only possible for Generals, if a coup can be blocked, and enough coins.
     * @param attacker The player who attempted the coup.
     * @throws std::logic_error if not a General, no coup to block, or not enough coins.
     */
    void generalBlockCoup(Player& attacker);

    /**
     * @brief Grants a Merchant a bonus coin if they start their turn with 3 or more coins. Only applies to Merchants.
     */
    void merchantBonus();                     // Merchant gains bonus if 3+ coins

////
};
//