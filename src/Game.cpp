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
        float deltaTime = deltaClock.restart().asSeconds();
        
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
    switch (currentState) {
        case GameState::PLAYING:
            checkGameOver();
            break;
            
        case GameState::GAME_OVER_SCREEN:
            if (gameOverTimer.getElapsedTime().asSeconds() >= GAME_OVER_DELAY) {
                currentState = GameState::MENU;
                selectedOption = 0;
            }
            break;
            
        case GameState::BOT_DEMO:
            if (!botDemoActive) {
                int level = (selectedOption >= 3) ? selectedOption - 2 : 1;
                startBotDemo(level);
            } else {
                updateBotDemo();
            }
            break;
            
        default:
            break;
    }
}

void Game::render() {
    switch (currentState) {
        case GameState::MENU:
            renderer.drawMenu(selectedOption);
            break;
            
        case GameState::PLAYING:
        case GameState::GAME_OVER_SCREEN:
            renderer.clear();
            renderer.drawMap(mapManager, player);
            renderer.drawUI(player);
            if (currentState == GameState::GAME_OVER_SCREEN) {
                renderer.drawGameOverScreen(gameWon);
            }
            break;
            
        case GameState::BOT_DEMO:
            renderer.clear();
            renderer.drawMap(mapManager, player);
            renderer.drawBotDemoUI(player, currentBotStep, botPath.size(), botDemoWon, botDemoFinished);
            break;
    }
    
    renderer.display();
}

void Game::startBotDemo(int level) {
    botDemoLevel = level;
    botDemoActive = true;
    currentBotStep = 0;
    botDemoWon = false;
    botDemoFinished = false;
    botPath.clear();
    
    mapManager.loadLevel(level);
    
    // Set position and battery based on difficulty
    player.setPosition(1, 1);
    // Set what's under the player at the starting position
    player.setUnderPlayer(mapManager.getCell(1, 1));
    // Place player on the map
    mapManager.setCell(1, 1, 'P');
    
    const int batteries[] = {50, 40, 35};
    player.battery = batteries[level - 1];
    player.reset();
    
    findBotPath();
    
    if (!botPath.empty()) {
        currentState = GameState::BOT_DEMO;
        botDemoTimer.restart();
    } else {
        // No path, return to menu
        currentState = GameState::MENU;
        botDemoActive = false;
    }
}

void Game::findBotPath() {
    // Find finish position
    int finishX = -1, finishY = -1;
    for (int i = 0; i < mapManager.getRows() && finishX == -1; i++) {
        for (int j = 0; j < mapManager.getCols(); j++) {
            if (mapManager.getCell(i, j) == 'F') {
                finishX = i;
                finishY = j;
                break;
            }
        }
    }
    
    if (finishX != -1) {
        botPath = aiBot.findPath(player.getX(), player.getY(), finishX, finishY, mapManager);
    }
}

void Game::updateBotDemo() {
    if (botDemoFinished) {
        if (botDemoTimer.getElapsedTime().asSeconds() >= 3.0f) {
            currentState = GameState::MENU;
            selectedOption = 0;
            botDemoActive = false;
            botDemoFinished = false;
            botDemoWon = false;
            currentBotStep = 0;
            botPath.clear();
        }
        return;
    }
    
    if (botDemoTimer.getElapsedTime().asSeconds() >= BOT_DEMO_DELAY) {
        executeBotStep();
        botDemoTimer.restart();
    }
}

void Game::executeBotStep() {
    if (currentBotStep >= botPath.size() - 1 || player.battery <= 0) {
        botDemoFinished = true;
        return;
    }
    
    int currentX = botPath[currentBotStep].first;
    int currentY = botPath[currentBotStep].second;
    int nextX = botPath[currentBotStep + 1].first;
    int nextY = botPath[currentBotStep + 1].second;
    
    char nextCell = mapManager.getCell(nextX, nextY);
    
    // Handle wall breaking
    if (nextCell == '#') {
        if (player.canBreak) {
            mapManager.setCell(nextX, nextY, '.');
            player.canBreak = false;
            player.energy = 0;
            nextCell = '.';
        } else {
            botDemoFinished = true;
            return;
        }
    }
    
    // Update old position
    mapManager.setCell(currentX, currentY, player.underPlayer);
    
    // Handle special cells
    if (nextCell == 'F') {
        player.underPlayer = 'F';
        player.setPosition(nextX, nextY);
        mapManager.setCell(player.getX(), player.getY(), 'P');
        botDemoWon = true;
        botDemoFinished = true;
    } else {
        if (mapManager.isTank(nextCell)) {
            player.currentAtmosphere = nextCell;
            player.updateDoors(const_cast<std::vector<std::vector<char>>&>(mapManager.getMap()), 
                             mapManager.getRows(), mapManager.getCols());
        }
        
        player.setUnderPlayer(nextCell);
        player.setPosition(nextX, nextY);
        mapManager.setCell(player.getX(), player.getY(), 'P');
    }
    
    // Update battery and energy
    player.battery--;
    player.energy++;
    if (player.energy >= maxEnergy) {
        player.energy = maxEnergy;
        player.canBreak = true;
    }
    
    currentBotStep++;
    
    if (player.battery <= 0) {
        botDemoFinished = true;
    }
}

void Game::checkGameOver() {
    if (player.gameWon) {
        startGameOverScreen(true);
    } else if (player.battery <= 0) {
        startGameOverScreen(false);
    }
}

void Game::startGameOverScreen(bool won) {
    gameWon = won;
    currentState = GameState::GAME_OVER_SCREEN;
    gameOverTimer.restart();
} 