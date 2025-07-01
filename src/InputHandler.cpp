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
                        
                        player.setPosition(1, 1);
                        player.setUnderPlayer(mapManager.getCell(1, 1));
                        mapManager.setCell(1, 1, 'P');
                        
                        if (difficulty == 1) {
                            player.battery = 50;
                        } else if (difficulty == 2) {
                            player.battery = 40;
                        } else if (difficulty == 3) {
                            player.battery = 35;
                        }
                        player.reset();
                        
                        if (isBotDemo) {
                            currentState = GameState::BOT_DEMO;
                        } else {
                            currentState = GameState::PLAYING;
                        }
                    }
                }
            } else if (currentState == GameState::PLAYING) {
                handlePlayerMovement(keyEvent, player, mapManager);
                
                if (keyEvent->code == sf::Keyboard::Key::E && player.getWaitingForBreak()) {
                    player.breakWall(const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()));
                    player.waitingForBreak = false;
                } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    currentState = GameState::MENU;
                }
            }
        }
    }
    return true;

void InputHandler::handlePlayerMovement(const sf::Event::KeyPressed* keyEvent, 
                                       Player& player, MapManager& mapManager) {
    struct Direction {
        int dx, dy;
        sf::Keyboard::Key key;
    };
    
    static const Direction directions[] = {
        {-1, 0, sf::Keyboard::Key::W}, 
        {1, 0, sf::Keyboard::Key::S}, 
        {0, -1, sf::Keyboard::Key::A}, 
        {0, 1, sf::Keyboard::Key::D}
    };
    
    for (const auto& dir : directions) {
        if (keyEvent->code == dir.key) {
            player.setLastDirection(dir.dx, dir.dy);
            
            int newX = player.getX() + dir.dx;
            int newY = player.getY() + dir.dy;
            
            if (player.getCanBreak() && mapManager.isWall(newX, newY)) {
                player.waitingForBreak = true;
            } else {
                player.movePlayer(dir.dx, dir.dy, 
                                const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                                mapManager.getRows(), mapManager.getCols());
            }
            break;
        }
    }
}

bool InputHandler::isWindowClosed() const {
    return !window.isOpen();
} 
