#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Player;

class Game {
private:
    std::vector<Player*> players;
    int currentTurnIndex = -1;
    std::unordered_map<Player*, int> sanctions;
    std::unordered_map<Player*, int> arrestBlocks; // 🕵️ Spy blocks arrest
    std::unordered_map<Player*, int> coupBlocks;   // 🎖️ General blocks coup
    int bank = 0; // קופה של המשחק
    std::unordered_map<Player*, bool> recentCoupTargets; 
    std::unordered_map<Player*, Player*> attemptedCoup;
    std::unordered_map<Player*, Player*> arrestLog;


public:
    void addPlayer(Player* p);
    Player* currentPlayer() const;
    std::string turn() const;
    std::vector<std::string> playersNames() const;
    void nextTurn();
    void eliminate(Player* p);
    std::string winner() const;
    bool isPlayerTurn(Player* p) const;

    void applySanction(Player* target);
    bool isSanctioned(Player* p) const;

    Player* getPlayer(const std::string& name);

    void blockArrest(Player* target);       // 🕵️ Spy
    bool isArrestBlocked(Player* target) const;

    void blockCoup(Player* target);         // 🎖️ General
    bool isCoupBlocked(Player* target) const;

    std::unordered_map<Player*, int> bribeLog;  // לשמירת שוחד בוצע
    void markBribe(Player* p);      // לוג שוחד
    bool wasBribeUsedBy(Player* p) const;
    void cancelBribe(Player* p);    // מבטל תור נוסף

    void addCoinsToBank(int amount);

    void markCoupTarget(Player* target);
    bool wasCoupTargeted(Player* target) const;
    void clearCoupMarks();

    void registerCoupAttempt(Player* attacker, Player* target);
    bool canBlockCoup(Player* target);
    void cancelCoup(Player* target);
    void markArrest(Player *from, Player *target);

    bool wasArrestedByMeLastTurn(Player* source, Player* target) const;

};
///