#include "../include/Renderer.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow& window) : window(window), fontLoaded(false) {}

int Renderer::calculateCellSize(const MapManager& mapManager) const {
    int availableWidth = WINDOW_WIDTH;
    int availableHeight = WINDOW_HEIGHT - UI_HEIGHT;
    
    int cellSizeWidth = availableWidth / mapManager.getCols();
    int cellSizeHeight = availableHeight / mapManager.getRows();
    
    int cellSize = std::min(cellSizeWidth, cellSizeHeight);
    
    cellSize = std::max(cellSize, 20);
    
    cellSize = std::min(cellSize, CELL_SIZE);
    
    return cellSize;
}

bool Renderer::createDefaultFont() {
    std::vector<std::string> fallbackPaths = {
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/Arial.ttf",
        "/Library/Fonts/Arial.ttf"
    };
    
    for (const auto& path : fallbackPaths) {
        if (defaultFont.openFromFile(path)) {
            return true;
        }
    }
    return false;
}

sf::Text Renderer::createText(const std::string& string, unsigned int size, const sf::Color& color) {
    const sf::Font& textFont = fontLoaded ? font : defaultFont;
    sf::Text text(textFont, string, size);
    text.setFillColor(color);
    return text;
}



bool Renderer::initializeGraphics() {
    // Load font
    std::vector<std::string> fontPaths = {
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/Arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "/System/Library/Fonts/STHeiti Light.ttc",
        "/System/Library/Fonts/STHeiti Medium.ttc",
        "/System/Library/Fonts/PingFang.ttc"
    };
    
    fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (font.openFromFile(path)) {
            fontLoaded = true;
            std::cout << "Font loaded successfully from: " << path << std::endl;
            break;
        }
    }
    


    // Initialize shapes
    cellShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cellShape.setOutlineThickness(1);
    cellShape.setOutlineColor(sf::Color(60, 60, 80));

    playerShape.setRadius(CELL_SIZE / 2 - 3);
    playerShape.setFillColor(PLAYER_COLOR);
    playerShape.setOutlineThickness(3);
    playerShape.setOutlineColor(sf::Color::White);

    tankShape.setRadius(CELL_SIZE / 2 - 3);
    tankShape.setOutlineThickness(3);
    tankShape.setOutlineColor(sf::Color::White);

    wallShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    wallShape.setFillColor(WALL_COLOR);
    wallShape.setOutlineThickness(1);
    wallShape.setOutlineColor(sf::Color(30, 30, 50));

    doorShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    doorShape.setOutlineThickness(2);
    doorShape.setOutlineColor(sf::Color(100, 80, 40));

    finishShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    finishShape.setFillColor(FINISH_COLOR);
    finishShape.setOutlineThickness(3);
    finishShape.setOutlineColor(sf::Color::White);

    // energy bar
    energyBarShape.setSize(sf::Vector2f(250, 25));
    energyBarShape.setFillColor(ENERGY_BAR_COLOR);
    energyBarShape.setOutlineThickness(2);
    energyBarShape.setOutlineColor(sf::Color(60, 60, 80));

    energyFillShape.setSize(sf::Vector2f(0, 21));
    energyFillShape.setFillColor(ENERGY_FILL_COLOR);
    
    return true;
}

void Renderer::drawMap(const MapManager& mapManager, const Player& player) {
    int cellSize = calculateCellSize(mapManager);
    int offsetX = (WINDOW_WIDTH - mapManager.getCols() * cellSize) / 2;
    int offsetY = (WINDOW_HEIGHT - UI_HEIGHT - mapManager.getRows() * cellSize) / 2;

    // Draw background
    for (int i = 0; i < mapManager.getRows(); i++) {
        for (int j = 0; j < mapManager.getCols(); j++) {
            float x = offsetX + j * cellSize;
            float y = offsetY + i * cellSize;

            char cell = mapManager.getCell(i, j);

            if (cell == '#') {
                wallShape.setSize(sf::Vector2f(cellSize, cellSize));
                wallShape.setPosition(sf::Vector2f(x, y));
                window.draw(wallShape);
            } else if (cell == '.' || cell == 'P') {
                cellShape.setSize(sf::Vector2f(cellSize, cellSize));
                cellShape.setPosition(sf::Vector2f(x, y));
                cellShape.setFillColor(FLOOR_COLOR);
                window.draw(cellShape);
            } else if (cell == 'A' || cell == 'B') {
                cellShape.setSize(sf::Vector2f(cellSize, cellSize));
                cellShape.setPosition(sf::Vector2f(x, y));
                cellShape.setFillColor(FLOOR_COLOR);
                window.draw(cellShape);

                tankShape.setRadius(cellSize / 2 - 3);
                tankShape.setPosition(sf::Vector2f(x + 3, y + 3));
                tankShape.setFillColor(cell == 'A' ? TANK_A_COLOR : TANK_B_COLOR);
                window.draw(tankShape);
            } else if (cell == 'D' || cell == 'O') {
                doorShape.setSize(sf::Vector2f(cellSize, cellSize));
                doorShape.setPosition(sf::Vector2f(x, y));
                doorShape.setFillColor(cell == 'O' ? DOOR_OPEN_COLOR : DOOR_CLOSED_COLOR);
                window.draw(doorShape);
            } else if (cell == 'F') {
                finishShape.setSize(sf::Vector2f(cellSize, cellSize));
                finishShape.setPosition(sf::Vector2f(x, y));
                window.draw(finishShape);
            }

            // Draw player
            if (i == player.getX() && j == player.getY()) {
                sf::CircleShape glow;
                glow.setRadius(cellSize / 2);
                glow.setPosition(sf::Vector2f(x, y));
                glow.setFillColor(sf::Color(0, 150, 255, 50));
                window.draw(glow);
                
                playerShape.setRadius(cellSize / 2 - 3);
                playerShape.setPosition(sf::Vector2f(x + 3, y + 3));
                window.draw(playerShape);
            }
        }
    }
}

void Renderer::drawUI(const Player& player) {
    // Draw UI background
    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(WINDOW_WIDTH, UI_HEIGHT));
    uiBackground.setPosition(sf::Vector2f(0, WINDOW_HEIGHT - UI_HEIGHT));
    uiBackground.setFillColor(UI_BACKGROUND_COLOR);
    uiBackground.setOutlineThickness(2);
    uiBackground.setOutlineColor(sf::Color(60, 60, 80));
    window.draw(uiBackground);

    // Draw energy bar
    energyBarShape.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 150));
    window.draw(energyBarShape);

    energyFillShape.setPosition(sf::Vector2f(52, WINDOW_HEIGHT - 148));
    energyFillShape.setSize(sf::Vector2f((player.getEnergy() * 246) / maxEnergy, 21));
    window.draw(energyFillShape);

    // Draw text
    auto batteryText = createText("Bateria: " + std::to_string(player.getBattery()), 22, TEXT_COLOR);
    batteryText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 120));
    window.draw(batteryText);

    auto energyText = createText("Energia: " + std::to_string(player.getEnergy()) + " / " + std::to_string(maxEnergy), 22, TEXT_COLOR);
    energyText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 90));
    window.draw(energyText);

    // Atmosphere
    std::string atmosphere = (player.getCurrentAtmosphere() == '.') ? "Normal" : std::string(1, player.getCurrentAtmosphere());
    auto atmosphereText = createText("Atmosfera: " + atmosphere, 22, TEXT_COLOR);
    atmosphereText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 60));
    window.draw(atmosphereText);

    // Wall breaking status
    if (player.getCanBreak()) {
        auto breakText = createText("Puedes romper paredes (presiona direccion + e)", 22, HIGHLIGHT_COLOR);
        breakText.setPosition(sf::Vector2f(350, WINDOW_HEIGHT - 120));
        window.draw(breakText);
    }

    if (player.getWaitingForBreak()) {
        auto waitText = createText("Esperando 'e' para romper pared...", 22, HIGHLIGHT_COLOR);
        waitText.setPosition(sf::Vector2f(350, WINDOW_HEIGHT - 90));
        window.draw(waitText);
    }
}

void Renderer::drawMenu(int selectedOption) {
    window.clear(UI_BACKGROUND_COLOR);

    // Draw title
    auto title = createText("ESTACION ESPACIAL", 48, HIGHLIGHT_COLOR);
    title.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 200, 80));
    window.draw(title);

    std::vector<std::string> options = {
        "Jugar - Facil",
        "Jugar - Medio", 
        "Jugar - Dificil",
        "Bot Demo - Facil",
        "Bot Demo - Medio",
        "Bot Demo - Dificil",
        "Salir"
    };

    for (int i = 0; i < options.size(); i++) {
        sf::Color textColor = (i == selectedOption) ? HIGHLIGHT_COLOR : TEXT_COLOR;
        std::string optionString = options[i];
        
        if (i == selectedOption) {
            optionString = "> " + options[i];
            
            // Draw selection highlight
            sf::RectangleShape highlight;
            highlight.setSize(sf::Vector2f(400, 40));
            highlight.setPosition(sf::Vector2f(180, 195 + i * 55));
            highlight.setFillColor(sf::Color(60, 60, 80, 100));
            highlight.setOutlineThickness(2);
            highlight.setOutlineColor(HIGHLIGHT_COLOR);
            window.draw(highlight);
        }
        
        auto optionText = createText(optionString, 26, textColor);
        optionText.setPosition(sf::Vector2f(200, 200 + i * 55));
        window.draw(optionText);
    }

    // Enhanced instructions
    auto instructions = createText("Usa W/A/S/D para mover, direccion + E para romper paredes", 18, sf::Color(150, 150, 170));
    instructions.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 50));
    window.draw(instructions);
}

void Renderer::clear() {
    window.clear(UI_BACKGROUND_COLOR);
}

void Renderer::display() {
    window.display();
}

void Renderer::drawGameOverScreen(bool gameWon) {
    // Create overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    overlay.setPosition(sf::Vector2f(0, 0));
    window.draw(overlay);

    // Draw the game over
    std::string gameOverString = gameWon ? "Ganaste" : "Perdiste";
    auto gameOverText = createText(gameOverString, 72, gameWon ? SUCCESS_COLOR : ERROR_COLOR);
    gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100));
    window.draw(gameOverText);

    // Draw return to menu message
    auto returnText = createText("Regresando al menu en 3 segundos...", 28, TEXT_COLOR);
    returnText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 50));
    window.draw(returnText);
}

void Renderer::drawBotDemoUI(const Player& player, int currentStep, int totalSteps, bool botWon, bool demoFinished) {
    // Draw UI background
    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(WINDOW_WIDTH, UI_HEIGHT));
    uiBackground.setPosition(sf::Vector2f(0, WINDOW_HEIGHT - UI_HEIGHT));
    uiBackground.setFillColor(UI_BACKGROUND_COLOR);
    uiBackground.setOutlineThickness(2);
    uiBackground.setOutlineColor(sf::Color(60, 60, 80));
    window.draw(uiBackground);

    // Draw energy bar
    energyBarShape.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 150));
    window.draw(energyBarShape);

    energyFillShape.setPosition(sf::Vector2f(52, WINDOW_HEIGHT - 148));
    energyFillShape.setSize(sf::Vector2f((player.getEnergy() * 246) / maxEnergy, 21));
    window.draw(energyFillShape);

    // Draw text
    auto batteryText = createText("Bateria: " + std::to_string(player.getBattery()), 22, TEXT_COLOR);
    batteryText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 120));
    window.draw(batteryText);

    auto energyText = createText("Energia: " + std::to_string(player.getEnergy()) + " / " + std::to_string(maxEnergy), 22, TEXT_COLOR);
    energyText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 90));
    window.draw(energyText);

    // Atmosphere
    std::string atmosphere = (player.getCurrentAtmosphere() == '.') ? "Normal" : std::string(1, player.getCurrentAtmosphere());
    auto atmosphereText = createText("Atmosfera: " + atmosphere, 22, TEXT_COLOR);
    atmosphereText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 60));
    window.draw(atmosphereText);

    auto demoText = createText("Bot Demo - Paso " + std::to_string(currentStep) + "/" + std::to_string(totalSteps - 1), 22, HIGHLIGHT_COLOR);
    demoText.setPosition(sf::Vector2f(350, WINDOW_HEIGHT - 120));
    window.draw(demoText);

    if (demoFinished) {
        std::string resultString = botWon ? "El bot completo el nivel" : "El bot se quedo sin bateria";
        sf::Color resultColor = botWon ? SUCCESS_COLOR : ERROR_COLOR;
        auto resultText = createText(resultString, 22, resultColor);
        resultText.setPosition(sf::Vector2f(350, WINDOW_HEIGHT - 90));
        window.draw(resultText);
        
        auto returnText = createText("Regresando al menu en 3 segundos...", 22, TEXT_COLOR);
        returnText.setPosition(sf::Vector2f(350, WINDOW_HEIGHT - 60));
        window.draw(returnText);
    } else {
        auto efficiencyText = createText("Eficiencia: " + std::to_string(totalSteps - 1) + " pasos", 22, sf::Color(0, 200, 255));
        efficiencyText.setPosition(sf::Vector2f(350, WINDOW_HEIGHT - 90));
        window.draw(efficiencyText);
    }
} 
