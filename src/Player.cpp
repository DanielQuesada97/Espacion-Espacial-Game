#include "../include/Player.h"
#include "../include/GameConstants.h"

Player::Player() : x(0), y(0), battery(0), energy(0), canBreak(false), 
                   lastDX(0), lastDY(0), currentAtmosphere('.'), underPlayer('.'), 
                   waitingForBreak(false), gameWon(false) {}

void Player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::setBattery(int newBattery) {
    battery = newBattery;
}

void Player::setEnergy(int newEnergy) {
    energy = newEnergy;
}

void Player::setCanBreak(bool canBreak) {
    this->canBreak = canBreak;
}

void Player::setCurrentAtmosphere(char atmosphere) {
    currentAtmosphere = atmosphere;
}

void Player::setWaitingForBreak(bool waiting) {
    waitingForBreak = waiting;
}

void Player::setGameWon(bool won) {
    gameWon = won;
}

void Player::setLastDirection(int dx, int dy) {
    lastDX = dx;
    lastDY = dy;
}

bool Player::isTank(char c) {
    return c == 'A' || c == 'B';
}

bool Player::isWall(int nx, int ny, const std::vector<std::vector<char>>& map) {
    return map[nx][ny] == '#';
}

bool Player::isValid(int nx, int ny, const std::vector<std::vector<char>>& map, int rows, int cols) {
    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) return false;
    return !(isWall(nx, ny, map) || map[nx][ny] == 'D');
}

void Player::movePlayer(int dx, int dy, std::vector<std::vector<char>>& map, int rows, int cols) {
    int nx = x + dx, ny = y + dy;
    // Don't override lastDX and lastDY here since they're set by input handler
    
    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) return;
    char cell = map[nx][ny];

    if (cell == 'F') {
        map[x][y] = underPlayer;
        underPlayer = 'F'; x = nx; y = ny;
        map[x][y] = 'P'; gameWon = true;
        return;
    }

    if (isTank(cell)) {
        currentAtmosphere = cell;
        underPlayer = '.';
        updateDoors(map, rows, cols);
    }

    if (isValid(nx, ny, map, rows, cols)) {
        map[x][y] = underPlayer;
        underPlayer = map[nx][ny];
        x = nx; y = ny;
        map[x][y] = 'P';
        battery--;
        energy++;
        if (energy >= maxEnergy) { energy = maxEnergy; canBreak = true; }
    }
}

void Player::breakWall(std::vector<std::vector<char>>& map) {
    if (!canBreak) return;
    int tx = x + lastDX, ty = y + lastDY;
    if (isWall(tx, ty, map)) {
        map[tx][ty] = '.';
        canBreak = false; energy = 0;
    }
}

void Player::updateDoors(std::vector<std::vector<char>>& map, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (map[i][j] == 'D' || map[i][j] == 'O')
                map[i][j] = (currentAtmosphere == 'A') ? 'O' : 'D';
}

void Player::reset() {
    energy = 0;
    canBreak = false;
    currentAtmosphere = '.';
    gameWon = false;
    waitingForBreak = false;
} 