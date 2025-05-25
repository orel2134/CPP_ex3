#include "Game.hpp"
#include "Player.hpp"
#include <stdexcept>
#include <algorithm>

void Game::addPlayer(Player* p) {
    if (!p) throw std::invalid_argument("Null player");
    players.push_back(p);
    if (currentTurnIndex == -1) currentTurnIndex = 0;
}

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

std::string Game::turn() const {
    return currentPlayer()->getName();
}

std::vector<std::string> Game::playersNames() const {
    std::vector<std::string> names;
    for (auto* p : players) {
        if (p->isAlive()) names.push_back(p->getName());
    }
    return names;
}

void Game::nextTurn() {
    if (players.empty()) return;
    do {
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
    } while (!players[currentTurnIndex]->isAlive());

    Player* current = players[currentTurnIndex];

    sanctions.erase(current);
    arrestBlocks.erase(current);
    coupBlocks.erase(current);
    clearCoupMarks();
    bribeLog.erase(current);  // clear bribe logs at new turn
}

void Game::eliminate(Player* p) {
    if (!p || !p->isAlive()) throw std::logic_error("Cannot eliminate.");
    p->eliminate();
}

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

bool Game::isPlayerTurn(Player* p) const {
    return currentPlayer() == p;
}

void Game::applySanction(Player* target) {
    if (!target || !target->isAlive()) throw std::logic_error("Invalid sanction target");
    sanctions[target] = currentTurnIndex;
}

bool Game::isSanctioned(Player* p) const {
    return sanctions.find(p) != sanctions.end();
}

Player* Game::getPlayer(const std::string& name) {
    for (Player* p : players) {
        if (p->getName() == name) return p;
    }
    throw std::logic_error("Player not found: " + name);
}

void Game::blockArrest(Player* target) {
    arrestBlocks[target] = currentTurnIndex;
}

bool Game::isArrestBlocked(Player* target) const {
    return arrestBlocks.find(target) != arrestBlocks.end();
}

void Game::blockCoup(Player* target) {
    coupBlocks[target] = currentTurnIndex;
}

bool Game::isCoupBlocked(Player* target) const {
    return coupBlocks.find(target) != coupBlocks.end();
}

void Game::markBribe(Player* p) {
    bribeLog[p] = currentTurnIndex;
}

bool Game::wasBribeUsedBy(Player* p) const {
    auto it = bribeLog.find(p);
    if (it == bribeLog.end()) return false;
    return (it->second == currentTurnIndex);
}

void Game::cancelBribe(Player* p) {
    bribeLog.erase(p);
    p->clearExtraAction();
}

void Game::addCoinsToBank(int amount) {
    bank += amount;
}

void Game::markCoupTarget(Player* target) {
    recentCoupTargets[target] = true;
}

bool Game::wasCoupTargeted(Player* target) const {
    return recentCoupTargets.find(target) != recentCoupTargets.end();
}

void Game::clearCoupMarks() {
    recentCoupTargets.clear();
}

void Game::registerCoupAttempt(Player* attacker, Player* target) {
    attemptedCoup[target] = attacker;
}

bool Game::canBlockCoup(Player* target) {
    return attemptedCoup.count(target) > 0;
}

void Game::cancelCoup(Player* target) {
    attemptedCoup.erase(target);
}

void Game::markArrest(Player* from, Player* target) {
    arrestLog[from] = target;
}

bool Game::wasArrestedByMeLastTurn(Player* source, Player* target) const {
    auto it = arrestLog.find(source);
    return it != arrestLog.end() && it->second == target;
}
///