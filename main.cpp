#include <iostream>
#include "Game.hpp"
#include "Player.hpp"
#include "Role.hpp"

int main() {
    Game game; // Assumes you have defined a default constructor for Game, or do so in Game.hpp/cpp

    // Players: Avichay is Governor, Hadar is Spy, Dani is Baron
    Player avichay("Avichay", Role::Governor, &game);
    Player hadar("Hadar", Role::Spy, &game);
    Player dani("Dani", Role::Baron, &game);

    std::cout << "\n--- Game Start ---\n";

    // Turn 1: Avichay does tax (expect 3 coins because he's Governor)
    std::cout << "\nTurn: " << game.turn() << std::endl;
    avichay.tax();

    // Turn 2: Hadar (Spy) spies on Dani and then gathers
    std::cout << "\nTurn: " << game.turn() << std::endl;
    hadar.spyOn(dani);  // should block Dani's arrest
    hadar.gather();     // completes turn

    // Turn 3: Dani tries to arrest Hadar → should throw due to Spy block
    std::cout << "\nTurn: " << game.turn() << std::endl;
    try {
        dani.arrest(hadar);
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Caught expected arrest block error: " << e.what() << std::endl;
    }

    // Advance turn after failed arrest attempt
    game.nextTurn();

    // Still Turn 3: Dani adds coins and sanctions Avichay
    dani.addCoins(3);
    dani.sanction(avichay);

    // Turn 4: Avichay tries to tax but is sanctioned → should throw
    std::cout << "\nTurn: " << game.turn() << std::endl;
    try {
        avichay.tax();  // blocked due to sanction
    } catch (const std::exception& e) {
        std::cout << "Caught expected sanction error: " << e.what() << std::endl;
    }

    // Still Turn 4: Avichay adds coins, does bribe + coup on Hadar
    avichay.addCoins(11);  // give enough for bribe + coup
    avichay.bribe();       // no turn end
    avichay.coup(hadar);   // should eliminate Hadar

    std::cout << "\nHadar is " << (hadar.isAlive() ? "still in the game" : "eliminated") << ".\n";

    // Turn 5: Dani gathers normally
    std::cout << "\nTurn: " << game.turn() << std::endl;
    dani.gather();

    // Show who's still alive
    std::cout << "\n--- Remaining Players ---\n";
    for (const auto& name : game.playersNames()) {
        std::cout << "- " << name << std::endl;
    }

    return 0;
}