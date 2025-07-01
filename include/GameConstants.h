#pragma once

#include <SFML/Graphics.hpp>

// Game constants
const int CELL_SIZE = 40;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int UI_HEIGHT = 200;
const int maxEnergy = 5;
const float GAME_OVER_DELAY = 3.0f; // 3 seconds delay before returning to menu
const float BOT_DEMO_DELAY = 0.5f; // 0.5 seconds between bot moves

// Game state
enum class GameState {
    MENU,
    PLAYING,
    BOT_DEMO,
    GAME_OVER_SCREEN
};

// Modern color palette
const sf::Color WALL_COLOR(45, 45, 65);           // Dark blue-gray
const sf::Color FLOOR_COLOR(240, 240, 250);       // Light blue-white
const sf::Color PLAYER_COLOR(0, 150, 255);        // Bright blue
const sf::Color TANK_A_COLOR(255, 80, 80);        // Bright red
const sf::Color TANK_B_COLOR(80, 255, 80);        // Bright green
const sf::Color DOOR_CLOSED_COLOR(139, 69, 19);   // Brown
const sf::Color DOOR_OPEN_COLOR(255, 215, 0);     // Gold
const sf::Color FINISH_COLOR(255, 255, 0);        // Yellow
const sf::Color ENERGY_BAR_COLOR(30, 30, 40);     // Dark background
const sf::Color ENERGY_FILL_COLOR(0, 255, 150);   // Bright green
const sf::Color UI_BACKGROUND_COLOR(25, 25, 35);  // Dark blue-gray
const sf::Color TEXT_COLOR(220, 220, 255);        // Light blue-white
const sf::Color HIGHLIGHT_COLOR(255, 200, 0);     // Orange-yellow
const sf::Color SUCCESS_COLOR(0, 255, 150);       // Bright green
const sf::Color ERROR_COLOR(255, 100, 100);       // Bright red 