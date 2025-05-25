#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Game.hpp"
#include "Player.hpp"
#include "Role.hpp"

int main() {
    Game game;
    std::vector<Player*> players;
    players.push_back(new Player("Orel", Role::Governor, &game));
    players.push_back(new Player("Avi", Role::Spy, &game));
    players.push_back(new Player("Alon", Role::General, &game));
    players.push_back(new Player("Maayan", Role::Merchant, &game));
    players.push_back(new Player("Avicii", Role::Merchant, &game));

    sf::RenderWindow window(sf::VideoMode(1100, 650), "Coup Game - GUI");
    sf::Font font;

    if (!font.loadFromFile("DejaVuSans-Bold.ttf")) {
        std::cerr << "❌ Failed to load font 'DejaVuSans-Bold.ttf'!" << std::endl;
        return 1;
    }

    // ✅ Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("backpicture.png")) {
        std::cerr << "❌ Failed to load background image 'backpicture.png'!" << std::endl;
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );

    sf::Text turnText("", font, 26); turnText.setPosition(50, 20); turnText.setFillColor(sf::Color::White);
    sf::Text roleText("", font, 20); roleText.setPosition(50, 55); roleText.setFillColor(sf::Color::Yellow);
    sf::Text resultText("", font, 22); resultText.setPosition(50, 90); resultText.setFillColor(sf::Color::Green);

    sf::RectangleShape gatherBtn({200, 50}), taxBtn({200, 50}), bribeBtn({200, 50}), coupBtn({200, 50});
    sf::RectangleShape sanctionBtn({200, 50}), investBtn({200, 50}), spyBtn({200, 50});
    gatherBtn.setPosition(50, 140); gatherBtn.setFillColor(sf::Color(100, 100, 250));
    taxBtn.setPosition(50, 210); taxBtn.setFillColor(sf::Color(100, 150, 100));
    bribeBtn.setPosition(50, 280); bribeBtn.setFillColor(sf::Color(200, 100, 100));
    coupBtn.setPosition(50, 350); coupBtn.setFillColor(sf::Color(250, 150, 50));
    sanctionBtn.setPosition(50, 420); sanctionBtn.setFillColor(sf::Color(150, 0, 150));
    investBtn.setPosition(50, 490); investBtn.setFillColor(sf::Color(0, 180, 180));
    spyBtn.setPosition(50, 560); spyBtn.setFillColor(sf::Color(90, 90, 90));

    sf::Text gatherText("Gather", font, 24), taxText("Tax", font, 24), bribeText("Bribe", font, 24), coupText("Coup", font, 24);
    sf::Text sanctionText("Sanction", font, 24), investText("Invest (Baron)", font, 20), spyText("Spy (Spy only)", font, 20);
    gatherText.setPosition(90, 150); taxText.setPosition(90, 220); bribeText.setPosition(90, 290); coupText.setPosition(90, 360);
    sanctionText.setPosition(80, 430); investText.setPosition(60, 500); spyText.setPosition(70, 570);

    for (auto* t : {&gatherText, &taxText, &bribeText, &coupText, &sanctionText, &investText, &spyText})
        t->setFillColor(sf::Color::White);

    std::vector<sf::RectangleShape> targetButtons;
    std::vector<sf::Text> targetTexts;
    bool choosingTarget = false, choosingSanction = false, choosingSpy = false;
    bool gameOver = false, mustCoup = false;
    std::string winnerName = "";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (gameOver) continue;

            Player* current = game.currentPlayer();
            mustCoup = current->getCoins() >= 10;
            sf::Vector2f mouse(sf::Mouse::getPosition(window));

            if (mustCoup && !choosingTarget) {
                choosingTarget = true;
                targetButtons.clear(); targetTexts.clear();
                int y = 470;
                for (Player* p : players) {
                    if (p->isAlive() && p != current) {
                        sf::RectangleShape btn({200, 40}); btn.setPosition(300, y); btn.setFillColor(sf::Color(60, 60, 60));
                        sf::Text txt(p->getName(), font, 20); txt.setPosition(310, y + 5); txt.setFillColor(sf::Color::White);
                        targetButtons.push_back(btn); targetTexts.push_back(txt); y += 50;
                    }
                }
                resultText.setString("You have 10+ coins. Must coup!");
                resultText.setFillColor(sf::Color::Red);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (choosingTarget || choosingSanction || choosingSpy) {
                    for (size_t i = 0; i < targetButtons.size(); ++i) {
                        if (targetButtons[i].getGlobalBounds().contains(mouse)) {
                            Player* target = game.getPlayer(targetTexts[i].getString());
                            try {
                                if (choosingTarget) current->coup(*target);
                                else if (choosingSanction) current->sanction(*target);
                                else if (choosingSpy) current->spyOn(*target);
                                resultText.setString("Action successful.");
                                resultText.setFillColor(sf::Color::Green);
                            } catch (const std::exception& e) {
                                resultText.setString(e.what());
                                resultText.setFillColor(sf::Color::Red);
                            }
                            choosingTarget = choosingSanction = choosingSpy = false;
                            break;
                        }
                    }
                } else if (gatherBtn.getGlobalBounds().contains(mouse)) {
                    try { current->gather(); resultText.setString(current->getName() + " gathered 1 coin."); }
                    catch (const std::exception& e) { resultText.setString(e.what()); resultText.setFillColor(sf::Color::Red); }
                } else if (taxBtn.getGlobalBounds().contains(mouse)) {
                    try { current->tax(); resultText.setString(current->getName() + " taxed."); }
                    catch (const std::exception& e) { resultText.setString(e.what()); resultText.setFillColor(sf::Color::Red); }
                } else if (bribeBtn.getGlobalBounds().contains(mouse)) {
                    try { current->bribe(); resultText.setString(current->getName() + " bribed."); }
                    catch (const std::exception& e) { resultText.setString(e.what()); resultText.setFillColor(sf::Color::Red); }
                } else if (coupBtn.getGlobalBounds().contains(mouse) && current->getCoins() >= 7) {
                    choosingTarget = true;
                    targetButtons.clear(); targetTexts.clear();
                    int y = 470;
                    for (Player* p : players) {
                        if (p->isAlive() && p != current) {
                            sf::RectangleShape btn({200, 40}); btn.setPosition(300, y); btn.setFillColor(sf::Color(60, 60, 60));
                            sf::Text txt(p->getName(), font, 20); txt.setPosition(310, y + 5); txt.setFillColor(sf::Color::White);
                            targetButtons.push_back(btn); targetTexts.push_back(txt); y += 50;
                        }
                    }
                    resultText.setString("Choose player to coup");
                } else if (sanctionBtn.getGlobalBounds().contains(mouse) && current->getCoins() >= 3) {
                    choosingSanction = true;
                    targetButtons.clear(); targetTexts.clear();
                    int y = 470;
                    for (Player* p : players) {
                        if (p->isAlive() && p != current) {
                            sf::RectangleShape btn({200, 40}); btn.setPosition(550, y); btn.setFillColor(sf::Color(120, 0, 120));
                            sf::Text txt(p->getName(), font, 20); txt.setPosition(560, y + 5); txt.setFillColor(sf::Color::White);
                            targetButtons.push_back(btn); targetTexts.push_back(txt); y += 50;
                        }
                    }
                    resultText.setString("Choose player to sanction");
                } else if (spyBtn.getGlobalBounds().contains(mouse) && current->getRole() == Role::Spy) {
                    choosingSpy = true;
                    targetButtons.clear(); targetTexts.clear();
                    int y = 470;
                    for (Player* p : players) {
                        if (p->isAlive() && p != current) {
                            sf::RectangleShape btn({200, 40}); btn.setPosition(800, y); btn.setFillColor(sf::Color(80, 80, 80));
                            sf::Text txt(p->getName(), font, 20); txt.setPosition(810, y + 5); txt.setFillColor(sf::Color::White);
                            targetButtons.push_back(btn); targetTexts.push_back(txt); y += 50;
                        }
                    }
                    resultText.setString("Choose player to spy on");
                }
            }
        }

        window.clear();
        Player* current = nullptr;
        if (!gameOver) {
            try { winnerName = game.winner(); gameOver = true; }
            catch (...) { current = game.currentPlayer(); mustCoup = current->getCoins() >= 10; }
        }

        if (gameOver) {
            window.draw(backgroundSprite);
            sf::Text winText("🏆 Winner: " + winnerName + " 🏆", font, 36);
            winText.setFillColor(sf::Color::Cyan);
            winText.setPosition(250, 250);
            window.draw(winText);
        } else {
            window.draw(backgroundSprite); //  Draw background
            window.draw(turnText);
            window.draw(roleText);
            window.draw(resultText);

            if (!mustCoup && !choosingTarget && !choosingSanction && !choosingSpy) {
                window.draw(gatherBtn); window.draw(gatherText);
                window.draw(taxBtn); window.draw(taxText);
                window.draw(bribeBtn); window.draw(bribeText);
                window.draw(coupBtn); window.draw(coupText);
                window.draw(sanctionBtn); window.draw(sanctionText);
                if (current->getRole() == Role::Baron) { window.draw(investBtn); window.draw(investText); }
                if (current->getRole() == Role::Spy) { window.draw(spyBtn); window.draw(spyText); }
            }

            for (size_t i = 0; i < targetButtons.size(); ++i) {
                window.draw(targetButtons[i]);
                window.draw(targetTexts[i]);
            }

            sf::Text playerListTitle("Players:", font, 20);
            playerListTitle.setPosition(850, 30);
            playerListTitle.setFillColor(sf::Color::White);
            window.draw(playerListTitle);

            int py = 60;
            for (Player* p : players) {
                if (!p->isAlive()) continue;
                sf::Text pText(p->getName() + " - " + std::to_string(p->getCoins()) + " coins", font, 18);
                pText.setPosition(850, py); pText.setFillColor(sf::Color::White);
                window.draw(pText); py += 30;
            }
        }

        window.display();
    }

    return 0;
}
//