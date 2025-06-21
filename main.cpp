#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <string>
#include <optional>
#include <variant>
using namespace std;

// Game constants
const int CELL_SIZE = 40;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int UI_HEIGHT = 200;

vector<vector<char>> map;
int ROWS = 0;
int COLS = 0;

char easyMap[10][10] = {
    {'#','#','#','#','#','#','#','#','#','#'},
    {'#','P','.','.','.','.','.','.','F','#'},
    {'#','#','#','#','#','.','#','#','.','#'},
    {'#','.','.','.','#','.','#','A','.','#'},
    {'#','.','#','.','#','.','#','#','.','#'},
    {'#','.','#','.','#','.','.','.','.','#'},
    {'#','.','#','.','#','#','#','#','.','#'},
    {'#','.','#','.','.','.','.','#','.','#'},
    {'#','.','.','.','#','#','.','#','.','#'},
    {'#','#','#','#','#','#','#','#','#','#'},
};

char mediumMap[10][20] = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','P','.','.','.','#','.','.','.','.','.','.','#','.','.','.','.','.','F','#'},
    {'#','.','#','#','.','#','.','#','#','#','#','.','#','.','#','#','#','#','.','#'},
    {'#','.','#','D','.','#','.','#','A','.','#','.','#','.','#','B','.','#','.','#'},
    {'#','.','#','#','.','#','.','#','#','.','#','.','#','.','#','#','.','#','.','#'},
    {'#','.','.','.','.','.','.','.','.','.','#','.','.','.','.','.','.','#','.','#'},
    {'#','#','#','#','#','#','#','#','#','.','#','#','#','#','#','#','.','#','.','#'},
    {'#','.','.','.','.','.','.','.','#','.','.','.','.','.','.','#','.','#','.','#'},
    {'#','.','#','#','#','#','#','.','#','#','#','#','#','#','.','#','.','#','.','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
};

char hardMap[20][20] = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','P','.','.','.','#','.','.','.','.','.','.','#','.','.','.','.','.','.','#'},
    {'#','.','#','#','.','#','.','#','#','#','#','.','#','.','#','#','#','#','.','#'},
    {'#','.','#','D','.','#','.','#','A','.','#','.','#','.','#','B','.','#','.','#'},
    {'#','.','#','#','.','#','.','#','#','.','#','.','#','.','#','#','.','#','.','#'},
    {'#','.','.','.','.','.','.','.','.','.','#','.','.','.','.','.','.','#','.','#'},
    {'#','#','#','#','#','#','#','#','#','.','#','#','#','#','#','#','.','#','.','#'},
    {'#','.','.','.','.','.','.','.','#','.','.','.','.','.','.','#','.','#','.','#'},
    {'#','.','#','#','#','#','#','.','#','#','#','#','#','#','.','#','.','#','.','#'},
    {'#','.','#','.','.','.','#','.','.','.','.','.','#','.','.','#','.','#','.','#'},
    {'#','.','#','.','#','.','#','#','#','#','#','.','#','#','#','#','.','#','.','#'},
    {'#','.','.','.','#','.','.','.','.','.','#','.','.','.','.','.','.','#','.','#'},
    {'#','#','#','.','#','#','#','#','#','.','#','#','#','#','#','#','#','#','.','#'},
    {'#','.','.','.','.','.','.','.','#','.','.','.','.','.','.','.','.','.','.','#'},
    {'#','.','#','#','#','#','#','.','#','#','#','#','#','#','#','#','#','#','.','#'},
    {'#','.','#','.','.','.','#','.','.','.','.','.','.','.','.','.','.','#','.','#'},
    {'#','.','#','.','#','.','#','#','#','#','#','#','#','#','#','#','#','#','.','#'},
    {'#','.','.','.','#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
};

int playerX, playerY;
int battery;
int energy = 0;
const int maxEnergy = 5;
bool canBreak = false;
int lastDX = 0;
int lastDY = 0;
bool gameWon = false;
char currentAtmosphere = '.';
char underPlayer = '.';
bool waitingForBreak = false;

// Game state
enum GameState {
    MENU,
    PLAYING,
    BOT_DEMO,
    GAME_OVER
};

GameState currentState = MENU;
int selectedOption = 0;
int botDemoLevel = 0;
bool botDemoActive = false;

// AI Bot structures and variables
struct Node {
    int x, y;
    int gCost, hCost, fCost;
    Node* parent;
    char atmosphere;
    bool canBreakWall;
    int energy; // Track energy like the player

    Node(int x, int y) : x(x), y(y), gCost(0), hCost(0), fCost(0), parent(nullptr), atmosphere('.'), canBreakWall(false), energy(0) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->fCost > b->fCost;
    }
};

// SFML objects
sf::RenderWindow window;
sf::Font font;
sf::RectangleShape cellShape;
sf::CircleShape playerShape;
sf::CircleShape tankShape;
sf::RectangleShape wallShape;
sf::RectangleShape doorShape;
sf::RectangleShape finishShape;
sf::RectangleShape energyBarShape;
sf::RectangleShape energyFillShape;

// Colors
sf::Color WALL_COLOR = sf::Color(64, 64, 64);
sf::Color FLOOR_COLOR = sf::Color(200, 200, 200);
sf::Color PLAYER_COLOR = sf::Color(0, 150, 255);
sf::Color TANK_A_COLOR = sf::Color(255, 100, 100);
sf::Color TANK_B_COLOR = sf::Color(100, 255, 100);
sf::Color DOOR_CLOSED_COLOR = sf::Color(139, 69, 19);
sf::Color DOOR_OPEN_COLOR = sf::Color(255, 215, 0);
sf::Color FINISH_COLOR = sf::Color(255, 255, 0);
sf::Color ENERGY_BAR_COLOR = sf::Color(50, 50, 50);
sf::Color ENERGY_FILL_COLOR = sf::Color(255, 255, 0);

bool isTank(char c) {
    return c == 'A' || c == 'B';
}

bool isWall(int x, int y) {
    return map[x][y] == '#';
}

bool isValid(int x, int y) {
    return !(isWall(x, y) || map[x][y] == 'D');
}

bool isValidForBot(int x, int y, char atmosphere, bool canBreak) {
    if (x < 0 || x >= ROWS || y < 0 || y >= COLS) return false;
    char cell = map[x][y];

    // Can always move to open spaces, tanks, and finish
    if (cell == '.' || cell == 'F' || isTank(cell)) return true;

    // Can move through open doors
    if (cell == 'O') return true;

    // Can move through doors if we have atmosphere A
    if (cell == 'D' && atmosphere == 'A') return true;

    // Walls are handled in the pathfinding algorithm based on energy
    if (cell == '#') return false;

    return false;
}

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

vector<pair<int, int>> findPath(int startX, int startY, int endX, int endY) {
    priority_queue<Node*, vector<Node*>, Compare> openSet;
    vector<vector<bool>> closedSet(ROWS, vector<bool>(COLS, false));
    vector<vector<Node*>> nodeMap(ROWS, vector<Node*>(COLS, nullptr));

    Node* startNode = new Node(startX, startY);
    startNode->hCost = heuristic(startX, startY, endX, endY);
    startNode->fCost = startNode->hCost;

    openSet.push(startNode);
    nodeMap[startX][startY] = startNode;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (closedSet[current->x][current->y]) continue;
        closedSet[current->x][current->y] = true;

        if (current->x == endX && current->y == endY) {
            // Reconstruct path
            vector<pair<int, int>> path;
            Node* node = current;
            while (node != nullptr) {
                path.push_back({node->x, node->y});
                node = node->parent;
            }
            reverse(path.begin(), path.end());

            // Clean up memory
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (nodeMap[i][j]) delete nodeMap[i][j];
                }
            }

            return path;
        }

        for (int i = 0; i < 4; i++) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (closedSet[newX][newY] || !isValidForBot(newX, newY, current->atmosphere, current->canBreakWall))
                continue;

            Node* neighbor = nodeMap[newX][newY];
            if (!neighbor) {
                neighbor = new Node(newX, newY);
                nodeMap[newX][newY] = neighbor;
            }

            int tentativeGCost = current->gCost + 1;

            // Update atmosphere if we hit a tank
            char newAtmosphere = current->atmosphere;
            if (isTank(map[newX][newY])) {
                newAtmosphere = map[newX][newY];
            }

            // Update energy and wall-breaking ability (like the player)
            int newEnergy = current->energy + 1;
            bool newCanBreak = (newEnergy >= maxEnergy);

            // If we're moving through a wall, we need to break it
            if (map[newX][newY] == '#') {
                if (!newCanBreak) {
                    continue; // Can't break wall without enough energy
                }
                newEnergy = 0; // Energy resets after breaking wall
                newCanBreak = false;
            }

            if (tentativeGCost < neighbor->gCost || neighbor->gCost == 0) {
                neighbor->parent = current;
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = heuristic(newX, newY, endX, endY);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->atmosphere = newAtmosphere;
                neighbor->canBreakWall = newCanBreak;
                neighbor->energy = newEnergy;

                openSet.push(neighbor);
            }
        }
    }

    // Clean up memory if no path found
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (nodeMap[i][j]) delete nodeMap[i][j];
        }
    }

    return {}; // No path found
}

void updateDoors() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (map[i][j] == 'D' || map[i][j] == 'O')
                map[i][j] = (currentAtmosphere == 'A') ? 'O' : 'D';
}

void movePlayer(int dx, int dy) {
    int nx = playerX + dx, ny = playerY + dy;
    lastDX = dx; lastDY = dy;
    if (nx < 0 || nx >= ROWS || ny < 0 || ny >= COLS) return;
    char cell = map[nx][ny];

    if (cell == 'F') {
        map[playerX][playerY] = underPlayer;
        underPlayer = 'F'; playerX = nx; playerY = ny;
        map[playerX][playerY] = 'P'; gameWon = true;
        return;
    }

    if (isTank(cell)) {
        currentAtmosphere = cell;
        underPlayer = '.';
        updateDoors();
    }

    if (isValid(nx, ny)) {
        map[playerX][playerY] = underPlayer;
        underPlayer = map[nx][ny];
        playerX = nx; playerY = ny;
        map[playerX][playerY] = 'P';
        battery--;
        energy++;
        if (energy >= maxEnergy) { energy = maxEnergy; canBreak = true; }
    }
}

void breakWall() {
    if (!canBreak) return;
    int tx = playerX + lastDX, ty = playerY + lastDY;
    if (isWall(tx, ty)) {
        map[tx][ty] = '.';
        canBreak = false; energy = 0;
    }
}

void loadLevel(int difficulty) {
    energy = 0; canBreak = false;
    currentAtmosphere = '.'; gameWon = false;
    waitingForBreak = false;

    if (difficulty == 1) {
        ROWS = 10; COLS = 10;
        map = vector<vector<char>>(ROWS, vector<char>(COLS));
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                map[i][j] = easyMap[i][j];
        playerX = 1; playerY = 1; battery = 50;
    } else if (difficulty == 2) {
        ROWS = 10; COLS = 20;
        map = vector<vector<char>>(ROWS, vector<char>(COLS));
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                map[i][j] = mediumMap[i][j];
        playerX = 1; playerY = 1; battery = 40;
    } else if (difficulty == 3) {
        ROWS = 20; COLS = 20;
        map = vector<vector<char>>(ROWS, vector<char>(COLS));
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                map[i][j] = hardMap[i][j];
        playerX = 1; playerY = 1; battery = 35;
    }

    updateDoors();
    underPlayer = map[playerX][playerY];
    map[playerX][playerY] = 'P';
}

void initializeGraphics() {
    // Create window
    window.create(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Estacion Espacial");
    window.setFramerateLimit(60);

    // Load font
    if (!font.openFromFile("/System/Library/Fonts/Arial.ttf")) {
        // Fallback font
        if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            // If both fonts fail, we'll continue without a font
            std::cerr << "Warning: Could not load any fonts" << std::endl;
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
}

void drawMap() {
    int offsetX = (WINDOW_WIDTH - COLS * CELL_SIZE) / 2;
    int offsetY = (WINDOW_HEIGHT - UI_HEIGHT - ROWS * CELL_SIZE) / 2;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            float x = offsetX + j * CELL_SIZE;
            float y = offsetY + i * CELL_SIZE;

            char cell = map[i][j];

            if (cell == '#') {
                wallShape.setPosition(sf::Vector2f(x, y));
                window.draw(wallShape);
            } else if (cell == '.' || cell == 'P') {
                cellShape.setPosition(sf::Vector2f(x, y));
                cellShape.setFillColor(FLOOR_COLOR);
                window.draw(cellShape);
            } else if (cell == 'A' || cell == 'B') {
                cellShape.setPosition(sf::Vector2f(x, y));
                cellShape.setFillColor(FLOOR_COLOR);
                window.draw(cellShape);

                tankShape.setPosition(sf::Vector2f(x + 2, y + 2));
                tankShape.setFillColor(cell == 'A' ? TANK_A_COLOR : TANK_B_COLOR);
                window.draw(tankShape);
            } else if (cell == 'D' || cell == 'O') {
                doorShape.setPosition(sf::Vector2f(x, y));
                doorShape.setFillColor(cell == 'O' ? DOOR_OPEN_COLOR : DOOR_CLOSED_COLOR);
                window.draw(doorShape);
            } else if (cell == 'F') {
                finishShape.setPosition(sf::Vector2f(x, y));
                window.draw(finishShape);
            }

            // Draw player
            if (i == playerX && j == playerY) {
                playerShape.setPosition(sf::Vector2f(x + 2, y + 2));
                window.draw(playerShape);
            }
        }
    }
}

void drawUI() {
    // Draw energy bar
    energyBarShape.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 150));
    window.draw(energyBarShape);

    energyFillShape.setPosition(sf::Vector2f(52, WINDOW_HEIGHT - 148));
    energyFillShape.setSize(sf::Vector2f((energy * 196) / maxEnergy, 16));
    window.draw(energyFillShape);

    // Draw text
    sf::Text text(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Battery
    text.setString("Bateria: " + to_string(battery));
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 120));
    window.draw(text);

    // Energy
    text.setString("Energia: " + to_string(energy) + " / " + to_string(maxEnergy));
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 90));
    window.draw(text);

    // Atmosphere
    string atmosphere = (currentAtmosphere == '.') ? "Normal" : string(1, currentAtmosphere);
    text.setString("Atmosfera: " + atmosphere);
    text.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 60));
    window.draw(text);

    // Wall breaking status
    if (canBreak) {
        text.setString("⚡ Puedes romper paredes (presiona direccion + e)");
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 120));
        window.draw(text);
    }

    if (waitingForBreak) {
        text.setString("Esperando 'e' para romper pared...");
        text.setPosition(sf::Vector2f(300, WINDOW_HEIGHT - 90));
        text.setFillColor(sf::Color::Yellow);
        window.draw(text);
    }

    // Game over or win
    if (gameWon) {
        text.setString("🎉 ¡Ganaste!");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Green);
        text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50));
        window.draw(text);
    } else if (battery <= 0) {
        text.setString("💀 ¡Bateria agotada!");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Red);
        text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50));
        window.draw(text);
    }
}

void drawMenu() {
    window.clear(sf::Color(30, 30, 30));

    sf::Text title(font);
    title.setString("ESTACION ESPACIAL");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 150, 100));
    window.draw(title);

    vector<string> options = {
        "1. Jugar - Facil",
        "2. Jugar - Medio",
        "3. Jugar - Dificil",
        "4. Bot Demo - Facil",
        "5. Bot Demo - Medio",
        "6. Bot Demo - Dificil",
        "7. Salir"
    };

    sf::Text optionText(font);
    optionText.setCharacterSize(24);

    for (int i = 0; i < options.size(); i++) {
        optionText.setString(options[i]);
        optionText.setPosition(sf::Vector2f(200, 200 + i * 50));

        if (i == selectedOption) {
            optionText.setFillColor(sf::Color::Yellow);
            optionText.setString("> " + options[i]);
        } else {
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

void handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event->is<sf::Event::KeyPressed>()) {
            const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (!keyEvent) continue;

            if (currentState == MENU) {
                if (keyEvent->code == sf::Keyboard::Key::Up) {
                    selectedOption = (selectedOption - 1 + 7) % 7;
                } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                    selectedOption = (selectedOption + 1) % 7;
                } else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    if (selectedOption == 6) { // Exit
                        window.close();
                    } else {
                        loadLevel(selectedOption + 1);
                        currentState = PLAYING;
                    }
                }
            } else if (currentState == PLAYING) {
                if (keyEvent->code == sf::Keyboard::Key::W) {
                    lastDX = -1; lastDY = 0;
                    if (canBreak && isWall(playerX + lastDX, playerY + lastDY)) {
                        waitingForBreak = true;
                    } else {
                        movePlayer(-1, 0);
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::S) {
                    lastDX = 1; lastDY = 0;
                    if (canBreak && isWall(playerX + lastDX, playerY + lastDY)) {
                        waitingForBreak = true;
                    } else {
                        movePlayer(1, 0);
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::A) {
                    lastDX = 0; lastDY = -1;
                    if (canBreak && isWall(playerX + lastDX, playerY + lastDY)) {
                        waitingForBreak = true;
                    } else {
                        movePlayer(0, -1);
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::D) {
                    lastDX = 0; lastDY = 1;
                    if (canBreak && isWall(playerX + lastDX, playerY + lastDY)) {
                        waitingForBreak = true;
                    } else {
                        movePlayer(0, 1);
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::E && waitingForBreak) {
                    breakWall();
                    waitingForBreak = false;
                } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    currentState = MENU;
                }
            }
        }
    }
}

int main() {
    initializeGraphics();

    while (window.isOpen()) {
        handleInput();

        if (currentState == MENU) {
            drawMenu();
        } else if (currentState == PLAYING) {
            window.clear(sf::Color(20, 20, 20));
            drawMap();
            drawUI();
        }

        window.display();
    }

    return 0;
}