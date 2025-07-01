#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "GameConstants.h"
#include "Player.h"
#include "MapManager.h"

class Renderer {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Font defaultFont;
    bool fontLoaded;
    
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
    void drawBotDemoUI(const Player& player, int currentStep, int totalSteps, bool botWon, bool demoFinished);
    
    // Helper methods
    void clear();
    void display();
    sf::Text createText(const std::string& string, unsigned int size, const sf::Color& color);

private:
    int calculateCellSize(const MapManager& mapManager) const;
    bool createDefaultFont();
}; 