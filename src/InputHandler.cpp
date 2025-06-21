#include "../include/InputHandler.h"

InputHandler::InputHandler(sf::RenderWindow& window) : window(window) {}

bool InputHandler::handleInput(GameState& currentState, int& selectedOption, 
                              Player& player, MapManager& mapManager) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            return false;
        }

        if (event->is<sf::Event::KeyPressed>()) {
            const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (!keyEvent) continue;

            if (currentState == GameState::MENU) {
                if (keyEvent->code == sf::Keyboard::Key::Up) {
                    selectedOption = (selectedOption - 1 + 7) % 7;
                } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                    selectedOption = (selectedOption + 1) % 7;
                } else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    if (selectedOption == 6) { // Exit
                        return false;
                    } else {
                        int difficulty = selectedOption + 1;
                        bool isBotDemo = false;
                        
                        if (selectedOption >= 3) {
                            difficulty = selectedOption - 2;
                            isBotDemo = true;
                        }
                        
                        mapManager.loadLevel(difficulty);
                        
                        // Player position and battery
                        player.setPosition(1, 1);
                        if (difficulty == 1) {
                            player.setBattery(50);
                        } else if (difficulty == 2) {
                            player.setBattery(40);
                        } else if (difficulty == 3) {
                            player.setBattery(35);
                        }
                        player.reset();
                        
                        if (isBotDemo) {
                            // Signal to start bot demo
                            currentState = GameState::BOT_DEMO;
                        } else {
                            currentState = GameState::PLAYING;
                        }
                    }
                }
            } else if (currentState == GameState::PLAYING) {
                if (keyEvent->code == sf::Keyboard::Key::W) {
                    player.setLastDirection(-1, 0);
                    if (player.getCanBreak() && mapManager.isWall(player.getX() - 1, player.getY())) {
                        player.setWaitingForBreak(true);
                    } else {
                        player.movePlayer(-1, 0, const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                                        mapManager.getRows(), mapManager.getCols());
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::S) {
                    player.setLastDirection(1, 0);
                    if (player.getCanBreak() && mapManager.isWall(player.getX() + 1, player.getY())) {
                        player.setWaitingForBreak(true);
                    } else {
                        player.movePlayer(1, 0, const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                                        mapManager.getRows(), mapManager.getCols());
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::A) {
                    player.setLastDirection(0, -1);
                    if (player.getCanBreak() && mapManager.isWall(player.getX(), player.getY() - 1)) {
                        player.setWaitingForBreak(true);
                    } else {
                        player.movePlayer(0, -1, const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                                        mapManager.getRows(), mapManager.getCols());
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::D) {
                    player.setLastDirection(0, 1);
                    if (player.getCanBreak() && mapManager.isWall(player.getX(), player.getY() + 1)) {
                        player.setWaitingForBreak(true);
                    } else {
                        player.movePlayer(0, 1, const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                                        mapManager.getRows(), mapManager.getCols());
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::E && player.getWaitingForBreak()) {
                    player.breakWall(const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()));
                    player.setWaitingForBreak(false);
                } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    currentState = GameState::MENU;
                }
            }
        }
    }
    return true; // Continue running
}

bool InputHandler::isWindowClosed() const {
    return !window.isOpen();
} 