#pragma once

#include <vector>
#include <string>

class Player {
public:
    // Core player state - made public for direct access
    int x, y;
    int battery;
    int energy;
    bool canBreak;
    char currentAtmosphere;
    char underPlayer;
    bool gameWon;
    bool waitingForBreak;

private:
    int lastDX, lastDY;

public:
    Player();
    
    // Essential getters (only for complex logic or when needed externally)
    int getX() const { return x; }
    int getY() const { return y; }
    int getBattery() const { return battery; }
    int getEnergy() const { return energy; }
    bool getCanBreak() const { return canBreak; }
    char getCurrentAtmosphere() const { return currentAtmosphere; }
    bool getWaitingForBreak() const { return waitingForBreak; }
    bool getGameWon() const { return gameWon; }
    char getUnderPlayer() const { return underPlayer; }
    
    // Essential setters (only for complex logic)
    void setPosition(int newX, int newY);
    void setLastDirection(int dx, int dy);
    void setUnderPlayer(char under);
    
    // Game logic
    void movePlayer(int dx, int dy, std::vector<std::vector<char>>& map, int rows, int cols);
    void breakWall(std::vector<std::vector<char>>& map);
    void updateDoors(std::vector<std::vector<char>>& map, int rows, int cols);
    void reset();
    
    // Helper methods
    bool isValid(int nx, int ny, const std::vector<std::vector<char>>& map, int rows, int cols);
    bool isTank(char c);
    bool isWall(int nx, int ny, const std::vector<std::vector<char>>& map);
}; 