#include "../include/Game.h"

Game::Game() : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Estacion Espacial"),
               currentState(GameState::MENU), selectedOption(0), botDemoLevel(0), botDemoActive(false),
               gameWon(false), currentBotStep(0), botDemoWon(false), botDemoFinished(false),
               renderer(window), inputHandler(window) {
    window.setFramerateLimit(60);
    renderer.initializeGraphics();
}

Game::~Game() {
    if (window.isOpen()) {
        window.close();
    }
}

void Game::run() {
    while (window.isOpen()) {
        // Handle input
        if (!inputHandler.handleInput(currentState, selectedOption, player, mapManager)) {
            window.close();
            break;
        }

        // Update game state
        update();

        // Render
        render();
    }
}

void Game::update() {
    if (currentState == GameState::PLAYING) {
        // Check for game over conditions
        checkGameOver();
    } else if (currentState == GameState::GAME_OVER_SCREEN) {
        // Check if it's time to return to menu
        if (gameOverTimer.getElapsedTime().asSeconds() >= GAME_OVER_DELAY) {
            currentState = GameState::MENU;
            selectedOption = 0; // Reset menu selection
        }
    } else if (currentState == GameState::BOT_DEMO) {
        // Check if we need to start the bot demo
        if (!botDemoActive) {
            // Determine which bot demo level was selected
            int level = 1;
            if (selectedOption == 3) level = 1; // Bot Demo - Fácil
            else if (selectedOption == 4) level = 2; // Bot Demo - Medio
            else if (selectedOption == 5) level = 3; // Bot Demo - Difícil
            
            startBotDemo(level);
        } else {
            // Update bot demo
            updateBotDemo();
        }
    }
}

void Game::render() {
    if (currentState == GameState::MENU) {
        renderer.drawMenu(selectedOption);
    } else if (currentState == GameState::PLAYING) {
        renderer.clear();
        renderer.drawMap(mapManager, player);
        renderer.drawUI(player);
    } else if (currentState == GameState::GAME_OVER_SCREEN) {
        renderer.clear();
        renderer.drawMap(mapManager, player);
        renderer.drawUI(player);
        renderer.drawGameOverScreen(gameWon);
    } else if (currentState == GameState::BOT_DEMO) {
        renderer.clear();
        renderer.drawMap(mapManager, player);
        renderer.drawBotDemoUI(player, currentBotStep, botPath.size(), botDemoWon, botDemoFinished);
    }

    renderer.display();
}

void Game::handleMenuSelection() {}

void Game::startBotDemo(int level) {
    botDemoLevel = level;
    botDemoActive = true;
    currentBotStep = 0;
    botDemoWon = false;
    botDemoFinished = false;
    botPath.clear(); // Clear any previous path
    
    mapManager.loadLevel(level);
    
    // Set player position and battery based on difficulty
    player.setPosition(1, 1);
    if (level == 1) {
        player.setBattery(50);
    } else if (level == 2) {
        player.setBattery(40);
    } else if (level == 3) {
        player.setBattery(35);
    }
    player.reset();
    
    // Find the path
    findBotPath();
    
    if (!botPath.empty()) {
        currentState = GameState::BOT_DEMO;
        botDemoTimer.restart();
    } else {
        // No path found, return to menu
        currentState = GameState::MENU;
        botDemoActive = false; // Reset state
    }
}

void Game::findBotPath() {
    // Find finish position
    int finishX = -1, finishY = -1;
    for (int i = 0; i < mapManager.getRows(); i++) {
        for (int j = 0; j < mapManager.getCols(); j++) {
            if (mapManager.getCell(i, j) == 'F') {
                finishX = i;
                finishY = j;
                break;
            }
        }
        if (finishX != -1) break;
    }
    
    if (finishX != -1) {
        // Use the current map state (which may have broken walls) for pathfinding
        botPath = aiBot.findPath(player.getX(), player.getY(), finishX, finishY, mapManager);
    }
}

void Game::updateBotDemo() {
    if (botDemoFinished) {
        // Demo finished, wait a bit then return to menu
        if (botDemoTimer.getElapsedTime().asSeconds() >= 3.0f) {
            currentState = GameState::MENU;
            selectedOption = 0;
            // Reset bot demo state for next run
            botDemoActive = false;
            botDemoFinished = false;
            botDemoWon = false;
            currentBotStep = 0;
            botPath.clear();
        }
        return;
    }
    
    // moves bot with pace
    if (botDemoTimer.getElapsedTime().asSeconds() >= BOT_DEMO_DELAY) {
        executeBotStep();
        botDemoTimer.restart();
    }
}

void Game::executeBotStep() {
    if (currentBotStep >= botPath.size() - 1 || player.getBattery() <= 0) {
        botDemoFinished = true;
        return;
    }
    
    int currentX = botPath[currentBotStep].first;
    int currentY = botPath[currentBotStep].second;
    int nextX = botPath[currentBotStep + 1].first;
    int nextY = botPath[currentBotStep + 1].second;
    
    // Get what's currently at the next position
    char nextCell = mapManager.getCell(nextX, nextY);
    
    // Handle wall breaking BEFORE moving
    if (nextCell == '#') {
        // Bot is breaking a wall
        if (player.getCanBreak()) {
            mapManager.setCell(nextX, nextY, '.'); // Break the wall
            player.setCanBreak(false);
            player.setEnergy(0);
            nextCell = '.'; // Update nextCell to reflect the broken wall
        } else {
            // Shouldn't happen if pathfinding is correct, but just in case
            botDemoFinished = true;
            return;
        }
    }
    
    // Update the old position with what was actually there
    mapManager.setCell(currentX, currentY, player.getUnderPlayer());
    
    // Handle special cells
    if (nextCell == 'F') {
        player.setUnderPlayer('F');
        player.setPosition(nextX, nextY);
        mapManager.setCell(player.getX(), player.getY(), 'P');
        botDemoWon = true;
        botDemoFinished = true;
    } else {
        if (mapManager.isTank(nextCell)) {
            player.setCurrentAtmosphere(nextCell);
            player.updateDoors(const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                             mapManager.getRows(), mapManager.getCols());
        }
        
        player.setUnderPlayer(nextCell);
        player.setPosition(nextX, nextY);
        mapManager.setCell(player.getX(), player.getY(), 'P');
    }
    
    // Update battery and energy
    player.setBattery(player.getBattery() - 1);
    player.setEnergy(player.getEnergy() + 1);
    if (player.getEnergy() >= maxEnergy) {
        player.setEnergy(maxEnergy);
        player.setCanBreak(true);
    }
    
    currentBotStep++;
    
    if (player.getBattery() <= 0) {
        botDemoFinished = true;
    }
}

void Game::checkGameOver() {
    // Check if player won or lost
    if (player.getGameWon()) {
        startGameOverScreen(true);
    } else if (player.getBattery() <= 0) {
        startGameOverScreen(false);
    }
}

void Game::startGameOverScreen(bool won) {
    gameWon = won;
    currentState = GameState::GAME_OVER_SCREEN;
    gameOverTimer.restart();
} 