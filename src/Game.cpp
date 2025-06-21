#include "../include/Game.h"

Game::Game() : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Estacion Espacial"),
               currentState(GameState::MENU), selectedOption(0), botDemoLevel(0), botDemoActive(false),
               gameWon(false), renderer(window), inputHandler(window) {
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
    }

    renderer.display();
}

void Game::handleMenuSelection() {
    // This is now handled in InputHandler
}

void Game::startBotDemo(int level) {
    botDemoLevel = level;
    botDemoActive = true;
    // Bot demo implementation can be added here
}

void Game::updateBotDemo() {
    // Bot demo update logic can be added here
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