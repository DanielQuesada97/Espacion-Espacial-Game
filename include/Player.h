#pragma once

#include <vector>
#include <string>

class Player {
private:
    int x, y;
    int battery;
    int energy;
    bool canBreak;
    int lastDX, lastDY;
    char currentAtmosphere;
    char underPlayer;
    bool waitingForBreak;
    bool gameWon;

public:
    Player();
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getBattery() const { return battery; }
    int getEnergy() const { return energy; }
    bool getCanBreak() const { return canBreak; }
    char getCurrentAtmosphere() const { return currentAtmosphere; }
    bool getWaitingForBreak() const { return waitingForBreak; }
    bool getGameWon() const { return gameWon; }
    
    // Setters
    void setPosition(int newX, int newY);
    void setBattery(int newBattery);
    void setEnergy(int newEnergy);
    void setCanBreak(bool canBreak);
    void setCurrentAtmosphere(char atmosphere);
    void setWaitingForBreak(bool waiting);
    void setGameWon(bool won);
    void setLastDirection(int dx, int dy);
    
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