#pragma once

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <optional>
#include <memory>

class MapManager;

struct Node {
    int x, y;
    int gCost, hCost, fCost;
    std::shared_ptr<Node> parent;
    char atmosphere;
    bool canBreakWall;
    int energy;

    Node(int x, int y);
};

struct Compare {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b);
};

class AIBot {
private:
    int heuristic(int x1, int y1, int x2, int y2);

public:
    AIBot();
    std::vector<std::pair<int, int>> findPath(int startX, int startY, int endX, int endY, 
                                             const MapManager& mapManager);
}; 