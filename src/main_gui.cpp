/*
 * main_gui.cpp
 * -----------------------------------------------------------------------------
 * Graphical User Interface for the Coup Game using SFML
 * -----------------------------------------------------------------------------
 * This file implements a simple GUI for the Coup game, allowing players to
 * interact with the game logic visually. The interface displays player info,
 * available actions, and handles user input for all main game mechanics.
 *
 * Features:
 * - Displays player list, coins, and current turn
 * - Allows actions: Gather, Tax, Bribe, Coup, Sanction, Invest (Baron), Spy (Spy)
 * - Handles target selection for actions that require it
 * - Shows game result and error messages
 * - Uses SFML for rendering and event handling
 *
 * -----------------------------------------------------------------------------
 */
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Game.hpp"
#include "Player.hpp"
#include "Role.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "General.hpp"
#include "Merchant.hpp"

int main() {
    Game game;
    std::vector<Player*> players;
    players.push_back(new Governor("Orel", &game));
    players.push_back(new Spy("Avi", &game));
    players.push_back(new General("Alon", &game));
    players.push_back(new Merchant("Shachar", &game));
    players.push_back(new Merchant("Avicii", &game));

    sf::RenderWindow window(sf::VideoMode(1100, 650), "Coup Game - GUI");
    sf::Font font;

    if (!font.loadFromFile("assets/DejaVuSans-Bold.ttf")) {
        std::cerr << " Failed to load font 'assets/DejaVuSans-Bold.ttf'!" << std::endl;
        return 1;
    }

    //  Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/backpicture.png")) {
        std::cerr << " Failed to load background image 'assets/backpicture.png'!" << std::endl;
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );
    // Make background brighter by drawing a semi-transparent white rectangle over it
    sf::RectangleShape brightOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    brightOverlay.setFillColor(sf::Color(255, 255, 255, 80)); // 80/255 alpha for brightness

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

    // --- Professional color palette for Coup-like game ---
    sf::Color bgOverlayColor(255, 255, 255, 60); // Slightly brighter, but less white
    sf::Color mainBtnColor(60, 60, 120);        // Deep blue for main actions
    sf::Color taxBtnColor(180, 140, 40);        // Gold for tax
    sf::Color bribeBtnColor(120, 60, 20);       // Brown for bribe
    sf::Color coupBtnColor(180, 30, 30);        // Red for coup
    sf::Color sanctionBtnColor(90, 0, 90);      // Purple for sanction
    sf::Color investBtnColor(30, 120, 60);      // Green for invest
    sf::Color spyBtnColor(40, 40, 40);          // Dark gray for spy
    sf::Color textColor(230, 230, 230);         // Light gray for text
    sf::Color highlightColor(255, 215, 0);      // Gold for highlights
    sf::Color winnerColor(0, 180, 60);          // Green for winner
    sf::Color playerListBg(30, 30, 30, 200);    // Semi-transparent dark for player list

    // Update overlay for background
    brightOverlay.setFillColor(bgOverlayColor);

    // --- Button colors ---
    gatherBtn.setFillColor(mainBtnColor);
    taxBtn.setFillColor(taxBtnColor);
    bribeBtn.setFillColor(bribeBtnColor);
    coupBtn.setFillColor(coupBtnColor);
    sanctionBtn.setFillColor(sanctionBtnColor);
    investBtn.setFillColor(investBtnColor);
    spyBtn.setFillColor(spyBtnColor);

    // --- Text colors ---
    turnText.setFillColor(highlightColor);
    turnText.setOutlineColor(sf::Color::Black);
    turnText.setOutlineThickness(2);
    roleText.setFillColor(textColor);
    resultText.setFillColor(sf::Color::Green);
    for (auto* t : {&gatherText, &taxText, &bribeText, &coupText, &sanctionText, &investText, &spyText})
        t->setFillColor(textColor);

    // --- Restart button ---
    sf::Text restartText("Restart Game", font, 32);
    restartText.setFillColor(sf::Color(60, 60, 0));
    // Center the text inside the button
    sf::FloatRect rt = restartText.getLocalBounds();
    float restartBtnWidth = rt.width + 40;
    float restartBtnHeight = rt.height + 28;
    float restartBtnX = (window.getSize().x - restartBtnWidth) / 2.f;
    float restartBtnY = 400;
    sf::RectangleShape restartBtn(sf::Vector2f(restartBtnWidth, restartBtnHeight));
    restartBtn.setPosition(restartBtnX, restartBtnY);
    restartBtn.setFillColor(sf::Color(255, 230, 80));
    restartBtn.setOutlineColor(sf::Color(180, 150, 0));
    restartBtn.setOutlineThickness(3);
    restartText.setPosition(restartBtnX + (restartBtnWidth - rt.width) / 2.f - rt.left,
                           restartBtnY + (restartBtnHeight - rt.height) / 2.f - rt.top);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (gameOver) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mouse(sf::Mouse::getPosition(window));
                    if (restartBtn.getGlobalBounds().contains(mouse)) {
                        // Reset game state
                        for (Player* p : players) delete p;
                        players.clear();
                        game = Game();
                        players.push_back(new Governor("Orel", &game));
                        players.push_back(new Spy("Avi", &game));
                        players.push_back(new General("Alon", &game));
                        players.push_back(new Merchant("Shachar", &game));
                        players.push_back(new Merchant("Avicii", &game));
                        winnerName = "";
                        gameOver = false;
                        mustCoup = false;
                        continue;
                    }
                }
                continue;
            }

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
            window.draw(brightOverlay); // Draw overlay to brighten background
            sf::Text winText("Winner: " + winnerName, font, 54); // Larger font size
            winText.setFillColor(winnerColor); // Professional green for winner
            winText.setOutlineColor(highlightColor);
            winText.setOutlineThickness(4);
            // Center horizontally, move higher vertically
            sf::FloatRect textRect = winText.getLocalBounds();
            float x = (window.getSize().x - textRect.width) / 2.f;
            float y = 80; // Higher up
            winText.setPosition(x, y);
            window.draw(winText);
            // Draw restart button
            window.draw(restartBtn);
            window.draw(restartText);
        } else {
            window.draw(backgroundSprite);
            window.draw(brightOverlay);
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

            // Player list background
            sf::RectangleShape playerListRect(sf::Vector2f(230, 300));
            playerListRect.setPosition(840, 20);
            playerListRect.setFillColor(playerListBg);
            window.draw(playerListRect);

            sf::Text playerListTitle("Players:", font, 20);
            playerListTitle.setPosition(850, 30);
            playerListTitle.setFillColor(highlightColor);
            window.draw(playerListTitle);

            int py = 60;
            for (Player* p : players) {
                if (!p->isAlive()) continue;
                sf::Text pText(p->getName() + " - " + std::to_string(p->getCoins()) + " coins", font, 18);
                pText.setPosition(850, py); pText.setFillColor(textColor);
                // Highlight current player's name with yellow background
                if (!gameOver && p == current) {
                    sf::RectangleShape nameBg(sf::Vector2f(220, 28));
                    nameBg.setPosition(845, py - 2);
                    nameBg.setFillColor(sf::Color(255, 255, 120, 220)); // Soft yellow
                    nameBg.setOutlineColor(sf::Color(200, 180, 0));
                    nameBg.setOutlineThickness(2);
                    window.draw(nameBg);
                    pText.setFillColor(sf::Color(60, 60, 0)); // Darker text for contrast
                }
                window.draw(pText); py += 30;
            }
        }

        window.display();
    }

    return 0;
}