#pragma once

#include <SFML/Graphics.hpp>
#include "GameConstants.h"
#include "Player.h"
#include "MapManager.h"
#include "AIBot.h"
#include "Renderer.h"
#include "InputHandler.h"

class Game {
private:
    sf::RenderWindow window;
    GameState currentState;
    int selectedOption;
    int botDemoLevel;
    bool botDemoActive;
    
    // Game over timer
    sf::Clock gameOverTimer;
    bool gameWon;
    
    // Game components
    Player player;
    MapManager mapManager;
    AIBot aiBot;
    Renderer renderer;
    InputHandler inputHandler;

public:
    Game();
    ~Game();
    
    // Game loop
    void run();
    
    // Game state management
    void update();
    void render();
    
    // Menu handling
    void handleMenuSelection();
    
    // Bot demo
    void startBotDemo(int level);
    void updateBotDemo();
    
    // Game over handling
    void checkGameOver();
    void startGameOverScreen(bool won);
}; 