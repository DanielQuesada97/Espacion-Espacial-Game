#include "../include/Renderer.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow &window) : window(window) {}

bool Renderer::initializeGraphics()
{
    // Load font
    if (!font.openFromFile("/System/Library/Fonts/Arial.ttf"))
    {
        if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc"))
        {
            std::cerr << "Warning: Could not load any fonts" << std::endl;
            return false;
        }
    }

    // Initialize shapes
    cellShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cellShape.setOutlineThickness(1);
    cellShape.setOutlineColor(sf::Color::Black);

    playerShape.setRadius(CELL_SIZE / 2 - 2);
    playerShape.setFillColor(PLAYER_COLOR);
    playerShape.setOutlineThickness(2);
    playerShape.setOutlineColor(sf::Color::White);

    tankShape.setRadius(CELL_SIZE / 2 - 2);
    tankShape.setOutlineThickness(2);
    tankShape.setOutlineColor(sf::Color::White);

    wallShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    wallShape.setFillColor(WALL_COLOR);
    wallShape.setOutlineThickness(1);
    wallShape.setOutlineColor(sf::Color::Black);

    doorShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    doorShape.setOutlineThickness(1);
    doorShape.setOutlineColor(sf::Color::Black);

    finishShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    finishShape.setFillColor(FINISH_COLOR);
    finishShape.setOutlineThickness(2);
    finishShape.setOutlineColor(sf::Color::Black);

    energyBarShape.setSize(sf::Vector2f(200, 20));
    energyBarShape.setFillColor(ENERGY_BAR_COLOR);
    energyBarShape.setOutlineThickness(2);
    energyBarShape.setOutlineColor(sf::Color::White);

    energyFillShape.setSize(sf::Vector2f(0, 16));
    energyFillShape.setFillColor(ENERGY_FILL_COLOR);

    return true;
}

void Renderer::drawMap(const MapManager &mapManager, const Player &player)
{
    int offsetX = (WINDOW_WIDTH - mapManager.getCols() * CELL_SIZE) / 2;
    int offsetY = (WINDOW_HEIGHT - UI_HEIGHT - mapManager.getRows() * CELL_SIZE) / 2;

    for (int i = 0; i < mapManager.getRows(); i++)
    {
        for (int j = 0; j < mapManager.getCols(); j++)
        {
            float x = offsetX + j * CELL_SIZE;
            float y = offsetY + i * CELL_SIZE;

            char cell = mapManager.getCell(i, j);

            if (cell == '#')
            {
                wallShape.setPosition(sf::Vector2f(x, y));
                window.draw(wallShape);
            }
            else if (cell == '.' || cell == 'P')
            {
                cellShape.setPosition(sf::Vector2f(x, y));
                cellShape.setFillColor(FLOOR_COLOR);
                window.draw(cellShape);
            }
            else if (cell == 'A' || cell == 'B')
            {
                cellShape.setPosition(sf::Vector2f(x, y));
                cellShape.setFillColor(FLOOR_COLOR);
                window.draw(cellShape);

                tankShape.setPosition(sf::Vector2f(x + 2, y + 2));
                tankShape.setFillColor(cell == 'A' ? TANK_A_COLOR : TANK_B_COLOR);
                window.draw(tankShape);
            }
            else if (cell == 'D' || cell == 'O')
            {
                doorShape.setPosition(sf::Vector2f(x, y));
                doorShape.setFillColor(cell == 'O' ? DOOR_OPEN_COLOR : DOOR_CLOSED_COLOR);
                window.draw(doorShape);
            }
            else if (cell == 'F')
            {
                finishShape.setPosition(sf::Vector2f(x, y));
                window.draw(finishShape);
            }

            // Draw player
            if (i == player.getX() && j == player.getY())
            {
                playerShape.setPosition(sf::Vector2f(x + 2, y + 2));
                window.draw(playerShape);
            }
        }
    }
}

void Renderer::drawUI(const Player &player)
{
    // Draw energy bar
    energyBarShape.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 150));
    window.draw(energyBarShape);

    energyFillShape.setPosition(sf::Vector2f(52, WINDOW_HEIGHT - 148));
    energyFillShape.setSize(sf::Vector2f((player.getEnergy() * 196) / maxEnergy, 16));
    window.draw(energyFillShape);

    // Draw text
    sf::Text text(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Battery
    text.setString("Bateria: " + std::to_string(player.getBattery()));
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 120));
    window.draw(text);

    // Energy
    text.setString("Energia: " + std::to_string(player.getEnergy()) + " / " + std::to_string(maxEnergy));
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 90));
    window.draw(text);

    // Atmosphere
    std::string atmosphere = (player.getCurrentAtmosphere() == '.') ? "Normal" : std::string(1, player.getCurrentAtmosphere());
    text.setString("Atmosfera: " + atmosphere);
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 60));
    window.draw(text);

    // Wall breaking status
    if (player.getCanBreak())
    {
        text.setString("Puedes romper paredes (presiona direccion + e)");
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 120));
        window.draw(text);
    }

    if (player.getWaitingForBreak())
    {
        text.setString("Esperando 'e' para romper pared...");
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 90));
        text.setFillColor(sf::Color::Yellow);
        window.draw(text);
    }
}

void Renderer::drawMenu(int selectedOption)
{
    window.clear(sf::Color(30, 30, 30));

    sf::Text title(font);
    title.setString("ESTACION ESPACIAL");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 150, 100));
    window.draw(title);

    std::vector<std::string> options = {
        "1. Jugar - Facil",
        "2. Jugar - Medio",
        "3. Jugar - Dificil",
        "4. Bot Demo - Facil",
        "5. Bot Demo - Medio",
        "6. Bot Demo - Dificil",
        "7. Salir"};

    sf::Text optionText(font);
    optionText.setCharacterSize(24);

    for (int i = 0; i < options.size(); i++)
    {
        optionText.setString(options[i]);
        optionText.setPosition(sf::Vector2f(200, 200 + i * 50));

        if (i == selectedOption)
        {
            optionText.setFillColor(sf::Color::Yellow);
            optionText.setString("> " + options[i]);
        }
        else
        {
            optionText.setFillColor(sf::Color::White);
        }

        window.draw(optionText);
    }

    // Instructions
    sf::Text instructions(font);
    instructions.setString("Usa W/A/S/D para mover, direccion + E para romper paredes");
    instructions.setCharacterSize(16);
    instructions.setFillColor(sf::Color(128, 128, 128));
    instructions.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 50));
    window.draw(instructions);
}

void Renderer::clear()
{
    window.clear(sf::Color(20, 20, 20));
}

void Renderer::display()
{
    window.display();
}

void Renderer::drawGameOverScreen(bool gameWon)
{
    // Create a semi-transparent overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black
    overlay.setPosition(sf::Vector2f(0, 0));
    window.draw(overlay);

    // Draw the game over message
    sf::Text gameOverText(font);
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(gameWon ? sf::Color::Green : sf::Color::Red);

    if (gameWon)
    {
        gameOverText.setString("Ganaste");
    }
    else
    {
        gameOverText.setString("Perdiste");
    }

    // Position text without using setOrigin (simpler approach)
    gameOverText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT / 2.0f - 50));
    window.draw(gameOverText);

    // Draw return to menu message
    sf::Text returnText(font);
    returnText.setString("Regresando al menu en 3 segundos...");
    returnText.setCharacterSize(24);
    returnText.setFillColor(sf::Color::White);

    // Position return text
    returnText.setPosition(sf::Vector2f(50, WINDOW_HEIGHT / 2.0f + 50));
    window.draw(returnText);
}

void Renderer::drawBotDemoUI(const Player &player, int currentStep, int totalSteps, bool botWon, bool demoFinished)
{
    // Draw energy bar
    energyBarShape.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 150));
    window.draw(energyBarShape);

    energyFillShape.setPosition(sf::Vector2f(52, WINDOW_HEIGHT - 148));
    energyFillShape.setSize(sf::Vector2f((player.getEnergy() * 196) / maxEnergy, 16));
    window.draw(energyFillShape);

    // Draw text
    sf::Text text(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Battery
    text.setString("Bateria: " + std::to_string(player.getBattery()));
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 120));
    window.draw(text);

    // Energy
    text.setString("Energia: " + std::to_string(player.getEnergy()) + " / " + std::to_string(maxEnergy));
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 90));
    window.draw(text);

    // Atmosphere
    std::string atmosphere = (player.getCurrentAtmosphere() == '.') ? "Normal" : std::string(1, player.getCurrentAtmosphere());
    text.setString("Atmosfera: " + atmosphere);
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 60));
    window.draw(text);

    // Bot demo specific info
    text.setString("Bot Demo - Paso " + std::to_string(currentStep) + "/" + std::to_string(totalSteps));
    text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 120));
    text.setFillColor(sf::Color::Yellow);
    window.draw(text);

    if (demoFinished)
    {
        if (botWon)
        {
            text.setString("El bot completo el nivel");
            text.setFillColor(sf::Color::Green);
        }
        else
        {
            text.setString("El bot se quedo sin bateria");
            text.setFillColor(sf::Color::Red);
        }
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 90));
        window.draw(text);

        text.setString("Regresando al menu en 3 segundos...");
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 60));
        window.draw(text);
    }
    else
    {
        text.setString("Eficiencia: " + std::to_string(totalSteps) + " pasos");
        text.setFillColor(sf::Color::Cyan);
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 90));
        window.draw(text);
    }
}