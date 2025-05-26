#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Player;

/**
 * @class Game
 * @brief Manages the state and rules of a Coup game session.
 *
 * The Game class is responsible for tracking players, turns, and all game-specific
 * mechanics such as sanctions, blocks, bribes, tax actions, coup attempts, and arrests.
 * It provides the main interface for player actions and enforces the rules and interactions
 * between different roles according to the game logic.
 */
class Game {
private:
    std::vector<Player*> players;                ///< List of players in the game
    int currentTurnIndex = -1;                   ///< Index of the player whose turn it is
    std::unordered_map<Player*, int> sanctions;  ///< Sanctions applied to players
    std::unordered_map<Player*, int> arrestBlocks; ///< Tracks arrest blocks (Spy role)
    std::unordered_map<Player*, int> coupBlocks;   ///< Tracks coup blocks (General role)
    int bank = 0;                               ///< Game bank for coins
    std::unordered_map<Player*, bool> recentCoupTargets; ///< Tracks recent coup targets
    std::unordered_map<Player*, Player*> attemptedCoup;  ///< Tracks coup attempts
    std::unordered_map<Player*, Player*> arrestLog;      ///< Tracks arrest actions
    std::unordered_map<Player*, int> taxLog;    ///< Tracks tax actions for blockTax

public:
    /**
     * @brief Adds a player to the game.
     * @param p Pointer to the player to add.
     */
    void addPlayer(Player* p);

    /**
     * @brief Returns the player whose turn it is.
     * @return Pointer to the current player.
     */
    Player* currentPlayer() const;

    /**
     * @brief Returns the name of the player whose turn it is.
     * @return Name of the current player.
     */
    std::string turn() const;

    /**
     * @brief Returns a vector of all player names in the game.
     * @return Vector of player names.
     */
    std::vector<std::string> playersNames() const;

    /**
     * @brief Advances the turn to the next player.
     */
    void nextTurn();

    /**
     * @brief Eliminates a player from the game.
     * @param p Pointer to the player to eliminate.
     */
    void eliminate(Player* p);

    /**
     * @brief Returns the name of the winning player, if the game is over.
     * @return Name of the winner.
     */
    std::string winner() const;

    /**
     * @brief Checks if it is the specified player's turn.
     * @param p Pointer to the player.
     * @return True if it is the player's turn, false otherwise.
     */
    bool isPlayerTurn(Player* p) const;

    /**
     * @brief Applies a sanction to a target player.
     * @param target Pointer to the player to sanction.
     */
    void applySanction(Player* target);

    /**
     * @brief Checks if a player is currently sanctioned.
     * @param p Pointer to the player.
     * @return True if sanctioned, false otherwise.
     */
    bool isSanctioned(Player* p) const;

    /**
     * @brief Returns a pointer to a player by name.
     * @param name Name of the player.
     * @return Pointer to the player, or nullptr if not found.
     */
    Player* getPlayer(const std::string& name);

    /**
     * @brief Blocks an arrest action against a target player (Spy role).
     * @param target Pointer to the player being protected.
     */
    void blockArrest(Player* target);

    /**
     * @brief Checks if an arrest is currently blocked for a player.
     * @param target Pointer to the player.
     * @return True if arrest is blocked, false otherwise.
     */
    bool isArrestBlocked(Player* target) const;

    /**
     * @brief Blocks a coup action against a target player (General role).
     * @param target Pointer to the player being protected.
     */
    void blockCoup(Player* target);

    /**
     * @brief Checks if a coup is currently blocked for a player.
     * @param target Pointer to the player.
     * @return True if coup is blocked, false otherwise.
     */
    bool isCoupBlocked(Player* target) const;

    std::unordered_map<Player*, int> bribeLog;  ///< Tracks bribe actions

    /**
     * @brief Marks that a player has used a bribe action.
     * @param p Pointer to the player.
     */
    void markBribe(Player* p);

    /**
     * @brief Checks if a player has used a bribe action.
     * @param p Pointer to the player.
     * @return True if bribe was used, false otherwise.
     */
    bool wasBribeUsedBy(Player* p) const;

    /**
     * @brief Cancels a bribe action for a player.
     * @param p Pointer to the player.
     */
    void cancelBribe(Player* p);

    /**
     * @brief Marks that a player has used a tax action.
     * @param p Pointer to the player.
     */
    void markTax(Player* p);

    /**
     * @brief Checks if a player has used a tax action.
     * @param p Pointer to the player.
     * @return True if tax was used, false otherwise.
     */
    bool wasTaxUsedBy(Player* p) const;

    /**
     * @brief Cancels a tax action for a player.
     * @param p Pointer to the player.
     */
    void cancelTax(Player* p);

    /**
     * @brief Adds coins to the game bank.
     * @param amount Number of coins to add.
     */
    void addCoinsToBank(int amount);

    /**
     * @brief Marks a player as having been targeted by a coup.
     * @param target Pointer to the player.
     */
    void markCoupTarget(Player* target);

    /**
     * @brief Checks if a player was recently targeted by a coup.
     * @param target Pointer to the player.
     * @return True if targeted, false otherwise.
     */
    bool wasCoupTargeted(Player* target) const;

    /**
     * @brief Clears all coup target marks.
     */
    void clearCoupMarks();

    /**
     * @brief Registers a coup attempt from one player to another.
     * @param attacker Pointer to the attacking player.
     * @param target Pointer to the target player.
     */
    void registerCoupAttempt(Player* attacker, Player* target);

    /**
     * @brief Checks if a coup can be blocked for a target player.
     * @param target Pointer to the player.
     * @return True if the coup can be blocked, false otherwise.
     */
    bool canBlockCoup(Player* target);

    /**
     * @brief Cancels a coup action for a target player.
     * @param target Pointer to the player.
     */
    void cancelCoup(Player* target);

    /**
     * @brief Marks an arrest action from one player to another.
     * @param from Pointer to the player performing the arrest.
     * @param target Pointer to the player being arrested.
     */
    void markArrest(Player *from, Player *target);

    /**
     * @brief Checks if a player was arrested by another player in the last turn.
     * @param source Pointer to the player who may have performed the arrest.
     * @param target Pointer to the player who may have been arrested.
     * @return True if the arrest occurred last turn, false otherwise.
     */
    bool wasArrestedByMeLastTurn(Player* source, Player* target) const;
};