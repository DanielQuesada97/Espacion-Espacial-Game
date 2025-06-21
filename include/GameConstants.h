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

// Colors
extern sf::Color WALL_COLOR;
extern sf::Color FLOOR_COLOR;
extern sf::Color PLAYER_COLOR;
extern sf::Color TANK_A_COLOR;
extern sf::Color TANK_B_COLOR;
extern sf::Color DOOR_CLOSED_COLOR;
extern sf::Color DOOR_OPEN_COLOR;
extern sf::Color FINISH_COLOR;
extern sf::Color ENERGY_BAR_COLOR;
extern sf::Color ENERGY_FILL_COLOR; 