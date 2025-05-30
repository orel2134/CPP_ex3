// orel2744@gmail.com
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <stdexcept>
#include <algorithm>
#include <random>
#include <ctime>

Game::Game() : bank(50) {}
Game::~Game() = default;
Game::Game(const Game& other) = default;
Game& Game::operator=(const Game& other) = default;

/**
 * @brief Adds a player to the game.
 * @param p Pointer to the player to add.
 * @throws std::invalid_argument if p is null.
 */
void Game::addPlayer(Player* p) {
    if (!p) throw std::invalid_argument("Null player");
    players.push_back(p);
    if (currentTurnIndex == -1) currentTurnIndex = 0;
}

/**
 * @brief Returns the current player whose turn it is.
 * @return Pointer to the current Player.
 * @throws std::logic_error if there are no players or no active players.
 */
Player* Game::currentPlayer() const {
    if (players.empty()) throw std::logic_error("No players in game.");
    size_t count = 0;
    int idx = currentTurnIndex;
    while (!players[idx]->isAlive()) {
        idx = (idx + 1) % players.size();
        if (++count > players.size()) throw std::logic_error("No active players.");
    }
    return players[idx];
}

/**
 * @brief Gets the name of the player whose turn it is.
 * @return The name of the current player.
 */
std::string Game::turn() const {
    return currentPlayer()->getName();
}

/**
 * @brief Returns the names of all alive players in the game.
 * @return Vector of player names.
 */
std::vector<std::string> Game::playersNames() const {
    std::vector<std::string> names;
    for (auto* p : players) {
        if (p->isAlive()) names.push_back(p->getName());
    }
    return names;
}

/**
 * @brief Advances the turn to the next alive player and clears temporary effects.
 */
void Game::nextTurn() {
    if (players.empty()) return;
    // Remember the player whose turn just ended
    Player* prev = players[currentTurnIndex];
    // advance to next alive player
    do {
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
    } while (!players[currentTurnIndex]->isAlive());
    // Clear sanction for the player who just finished their turn
    sanctions.erase(prev);
    Player* next = players[currentTurnIndex];
    arrestBlocks.erase(next);
    coupBlocks.erase(next);
    clearCoupMarks();
    bribeLog.erase(next);  // clear bribe logs at new turn (now for next player)
    taxLog.erase(next);    // clear tax logs at new turn (now for next player)
}

/**
 * @brief Eliminates a player from the game.
 * @param p Pointer to the player to eliminate.
 * @throws std::logic_error if p is null or already eliminated.
 */
void Game::eliminate(Player* p) {
    if (!p || !p->isAlive()) throw std::logic_error("Cannot eliminate.");
    // If the eliminated player is the current player, advance the turn
    if (currentPlayer() == p) {
        nextTurn();
    }
    p->eliminate();
    // Remove from all logs and blocks
    sanctions.erase(p);
    arrestBlocks.erase(p);
    coupBlocks.erase(p);
    recentCoupTargets.erase(p);
    attemptedCoup.erase(p);
    arrestLog.erase(p);
    taxLog.erase(p);
    bribeLog.erase(p);
}

/**
 * @brief Returns the name of the winner if only one player is alive.
 * @return The winner's name.
 * @throws std::logic_error if the game is not over or no winner exists.
 */
std::string Game::winner() const {
    std::string winnerName = "";
    for (Player* p : players) {
        if (p->isAlive()) {
            if (!winnerName.empty()) throw std::logic_error("Game is not over yet.");
            winnerName = p->getName();
        }
    }
    if (winnerName.empty()) throw std::logic_error("No winner.");
    return winnerName;
}

/**
 * @brief Checks if it is the given player's turn.
 * @param p Pointer to the player.
 * @return True if it is p's turn, false otherwise.
 */
bool Game::isPlayerTurn(Player* p) const {
    return currentPlayer() == p;
}

/**
 * @brief Applies a sanction to a player for the current turn.
 * @param target Pointer to the player to sanction.
 * @throws std::logic_error if target is null or not alive.
 */
void Game::applySanction(Player* target) {
    if (!target || !target->isAlive()) throw std::logic_error("Invalid sanction target");
    sanctions[target] = currentTurnIndex;
}

/**
 * @brief Checks if a player is currently sanctioned.
 * @param p Pointer to the player.
 * @return True if sanctioned, false otherwise.
 */
bool Game::isSanctioned(Player* p) const {
    return sanctions.find(p) != sanctions.end();
}

/**
 * @brief Gets a player by name.
 * @param name The name of the player.
 * @return Pointer to the player.
 * @throws std::logic_error if player not found.
 */
Player* Game::getPlayer(const std::string& name) {
    for (Player* p : players) {
        if (p->getName() == name) return p;
    }
    throw std::logic_error("Player not found: " + name);
}

/**
 * @brief Blocks an arrest action on a player for the current turn.
 * @param target Pointer to the player to protect.
 */
void Game::blockArrest(Player* target) {
    arrestBlocks[target] = currentTurnIndex;
}

/**
 * @brief Checks if a player's arrest is currently blocked.
 * @param target Pointer to the player.
 * @return True if arrest is blocked, false otherwise.
 */
bool Game::isArrestBlocked(Player* target) const {
    return arrestBlocks.find(target) != arrestBlocks.end();
}

/**
 * @brief Blocks a coup action on a player for the current turn.
 * @param target Pointer to the player to protect.
 */
void Game::blockCoup(Player* target) {
    coupBlocks[target] = currentTurnIndex;
}

/**
 * @brief Checks if a player's coup is currently blocked.
 * @param target Pointer to the player.
 * @return True if coup is blocked, false otherwise.
 */
bool Game::isCoupBlocked(Player* target) const {
    return coupBlocks.find(target) != coupBlocks.end();
}

/**
 * @brief Marks that a player has used a bribe this turn.
 * @param p Pointer to the player.
 */
void Game::markBribe(Player* p) {
    bribeLog[p] = currentTurnIndex;
}

/**
 * @brief Checks if a player has used a bribe this turn.
 * @param p Pointer to the player.
 * @return True if bribe was used this turn, false otherwise.
 */
bool Game::wasBribeUsedBy(Player* p) const {
    auto it = bribeLog.find(p);
    if (it == bribeLog.end()) return false;
    return (it->second == currentTurnIndex);
}

/**
 * @brief Cancels a player's bribe and extra action for the turn.
 * @param p Pointer to the player.
 */
void Game::cancelBribe(Player* p) {
    bribeLog.erase(p);
    p->clearExtraAction();
}

/**
 * @brief Adds coins to the game's bank.
 * @param amount Number of coins to add.
 */
void Game::addCoinsToBank(int amount) {
    bank += amount;
}

/**
 * @brief Marks a player as having been targeted by a coup this turn.
 * @param target Pointer to the player.
 */
void Game::markCoupTarget(Player* target) {
    recentCoupTargets[target] = true;
}

/**
 * @brief Checks if a player was targeted by a coup this turn.
 * @param target Pointer to the player.
 * @return True if targeted, false otherwise.
 */
bool Game::wasCoupTargeted(Player* target) const {
    return recentCoupTargets.find(target) != recentCoupTargets.end();
}

/**
 * @brief Clears all coup target marks for the new turn.
 */
void Game::clearCoupMarks() {
    recentCoupTargets.clear();
}

/**
 * @brief Registers a coup attempt from one player to another.
 * @param attacker Pointer to the attacking player.
 * @param target Pointer to the target player.
 */
void Game::registerCoupAttempt(Player* attacker, Player* target) {
    attemptedCoup[target] = attacker;
}

/**
 * @brief Checks if a coup can be blocked for a player.
 * @param target Pointer to the player.
 * @return True if coup can be blocked, false otherwise.
 */
bool Game::canBlockCoup(Player* target) {
    return attemptedCoup.count(target) > 0;
}

/**
 * @brief Cancels a coup attempt on a player.
 * @param target Pointer to the player.
 */
void Game::cancelCoup(Player* target) {
    attemptedCoup.erase(target);
}

/**
 * @brief Marks that a player has attempted to arrest another player.
 * @param from Pointer to the player who arrested.
 * @param target Pointer to the player who was arrested.
 */
void Game::markArrest(Player* from, Player* target) {
    arrestLog[from] = target;
}

/**
 * @brief Checks if a player was arrested by another player last turn.
 * @param source Pointer to the arresting player.
 * @param target Pointer to the arrested player.
 * @return True if arrested last turn, false otherwise.
 */
bool Game::wasArrestedByMeLastTurn(Player* source, Player* target) const {
    auto it = arrestLog.find(source);
    return it != arrestLog.end() && it->second == target;
}

/**
 * @brief Marks that a player has used a tax this turn.
 * @param p Pointer to the player.
 */
void Game::markTax(Player* p) {
    taxLog[p] = currentTurnIndex;
}

/**
 * @brief Checks if a player has used a tax this turn.
 * @param p Pointer to the player.
 * @return True if tax was used this turn, false otherwise.
 */
bool Game::wasTaxUsedBy(Player* p) const {
    return taxLog.find(p) != taxLog.end();
}

/**
 * @brief Cancels a player's tax and removes the coins gained for the turn.
 * @param p Pointer to the player.
 */
void Game::cancelTax(Player* p) {
    // Remove the coins gained from tax this turn
    // Assumes only one tax per turn
    if (!wasTaxUsedBy(p)) return;
    int amount = (p->getRole() == Role::Governor) ? 3 : 2;
    p->removeCoins(amount);
    taxLog.erase(p);
}

/**
 * @brief Gets a random role for player assignment.
 * @return A randomly selected Role.
 */
Role Game::getRandomRole() {
    static std::vector<Role> roles = {
        Role::Governor, Role::Spy, Role::Baron, Role::General, Role::Judge, Role::Merchant
    };
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<size_t> dist(0, roles.size() - 1);
    return roles[dist(rng)];
}

/**
 * @brief Creates a player with the specified role.
 * @param name The name of the player.
 * @param game Pointer to the game instance.
 * @param role The role to assign to the player.
 * @return Pointer to the newly created Player.
 * @throws std::invalid_argument if the role is unknown.
 */
Player* Game::createPlayerWithRole(const std::string& name, Game* game, Role role) {
    switch (role) {
        case Role::Governor: return new Governor(name, game);
        case Role::Spy:      return new Spy(name, game);
        case Role::Baron:    return new Baron(name, game);
        case Role::General:  return new General(name, game);
        case Role::Judge:    return new Judge(name, game);
        case Role::Merchant: return new Merchant(name, game);
        default: throw std::invalid_argument("Unknown role for player creation");
    }
}
