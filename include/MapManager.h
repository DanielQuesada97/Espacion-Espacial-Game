#pragma once

#include <vector>

class MapManager {
private:
    std::vector<std::vector<char>> map;
    int rows, cols;
    
    // Level maps
    static const char easyMap[10][10];
    static const char mediumMap[10][20];
    static const char hardMap[20][20];

public:
    MapManager();
    
    // Getters
    const std::vector<std::vector<char>>& getMap() const { return map; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    
    // Map operations
    void loadLevel(int difficulty);
    void setCell(int x, int y, char value);
    char getCell(int x, int y) const;
    
    // Helper methods
    bool isTank(char c) const;
    bool isWall(int x, int y) const;
    bool isValid(int x, int y) const;
    bool isValidForBot(int x, int y, char atmosphere, bool canBreak) const;
}; 