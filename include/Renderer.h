#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "GameConstants.h"
#include "Player.h"
#include "MapManager.h"

class Renderer {
private:
    sf::RenderWindow& window;
    sf::Font font;
    
    // Shapes
    sf::RectangleShape cellShape;
    sf::CircleShape playerShape;
    sf::CircleShape tankShape;
    sf::RectangleShape wallShape;
    sf::RectangleShape doorShape;
    sf::RectangleShape finishShape;
    sf::RectangleShape energyBarShape;
    sf::RectangleShape energyFillShape;

public:
    Renderer(sf::RenderWindow& window);
    
    // Initialization
    bool initializeGraphics();
    
    // Drawing methods
    void drawMap(const MapManager& mapManager, const Player& player);
    void drawUI(const Player& player);
    void drawMenu(int selectedOption);
    void drawGameOverScreen(bool gameWon);
    
    // Helper methods
    void clear();
    void display();
}; 