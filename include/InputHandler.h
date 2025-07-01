#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "GameConstants.h"
#include "Player.h"
#include "MapManager.h"

class InputHandler {
private:
    sf::RenderWindow& window;
    void handlePlayerMovement(const sf::Event::KeyPressed* keyEvent, 
                             Player& player, MapManager& mapManager);

public:
    InputHandler(sf::RenderWindow& window);
    
    // Input processing
    bool handleInput(GameState& currentState, int& selectedOption, 
                    Player& player, MapManager& mapManager);
    
    // Helper methods
    bool isWindowClosed() const;
}; 