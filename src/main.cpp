// orel2744@gmail.com
#include <iostream>
#include <vector>
#include <memory>
#include "Game.hpp"
#include "Player.hpp"
#include "Role.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"

int main() {
    Game game;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::string> names = {"Avichay", "Shachar", "Dani"};
    for (const auto& name : names) {
        Role role = Game::getRandomRole();
        players.emplace_back(Game::createPlayerWithRole(name, &game, role));
        std::cout << name << " assigned role: " << roleToString(role) << std::endl;
    }
    std::cout << "\n--- Game Start ---\n";

    // Example: let the first player take a turn (demonstration only)
    std::cout << "\nTurn: " << game.turn() << std::endl;
    players[0]->gather();

    // Show who's still alive
    std::cout << "\n--- Remaining Players ---\n";
    for (const auto& name : game.playersNames()) {
        std::cout << "- " << name << std::endl;
    }

    return 0;
}
