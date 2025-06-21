# Estacion Espacial v3 - Modular Structure

This project has been reorganized into a modular structure for better maintainability and code organization.

## Project Structure

```
estacion_espacial_v3/
├── include/                 # Header files
│   ├── GameConstants.h     # Game constants, enums, and colors
│   ├── Player.h           # Player class for game logic
│   ├── MapManager.h       # Map and level management
│   ├── AIBot.h           # AI pathfinding and bot logic
│   ├── Renderer.h        # Graphics and rendering
│   ├── InputHandler.h    # Input processing
│   └── Game.h           # Main game coordination
├── src/                   # Source files
│   ├── main.cpp         # Entry point
│   ├── Game.cpp         # Main game implementation
│   ├── Player.cpp       # Player logic implementation
│   ├── MapManager.cpp   # Map management implementation
│   ├── AIBot.cpp        # AI implementation
│   ├── Renderer.cpp     # Rendering implementation
│   ├── InputHandler.cpp # Input handling implementation
│   └── GameConstants.cpp # Constants implementation
├── CMakeLists.txt        # Build configuration
└── README.md            # This file
```

## Class Responsibilities

### GameConstants
- Defines all game constants (window size, cell size, etc.)
- Contains color definitions
- Defines game state enums

### Player
- Manages player state (position, battery, energy, etc.)
- Handles player movement logic
- Manages wall breaking mechanics
- Updates doors based on atmosphere

### MapManager
- Manages the game map and level data
- Handles level loading for different difficulties
- Provides map validation methods
- Contains all level map definitions

### AIBot
- Implements A* pathfinding algorithm
- Handles bot movement logic
- Manages bot state (energy, atmosphere, etc.)

### Renderer
- Handles all graphics rendering
- Manages SFML shapes and fonts
- Draws the game map, UI, and menu
- Initializes graphics resources

### InputHandler
- Processes all user input
- Handles keyboard events
- Manages game state transitions
- Coordinates between input and game logic

### Game
- Main game coordinator
- Manages the game loop
- Coordinates between all components
- Handles window management

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Key Improvements

1. **Separation of Concerns**: Each class has a specific responsibility
2. **Modularity**: Components can be easily modified or replaced
3. **Maintainability**: Code is organized and easier to understand
4. **Extensibility**: New features can be added without affecting existing code
5. **Testability**: Individual components can be tested in isolation

## Game Features

- Three difficulty levels (Easy, Medium, Hard)
- Energy-based wall breaking mechanics
- Atmosphere system affecting door states
- AI bot with pathfinding capabilities
- Modern SFML-based graphics
- Intuitive controls (WASD + E for wall breaking) 