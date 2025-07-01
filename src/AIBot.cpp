#include "../include/AIBot.h"
#include "../include/MapManager.h"
#include "../include/GameConstants.h"
#include <queue>

Node::Node(int x, int y) : x(x), y(y), gCost(0), hCost(0), fCost(0), parent(nullptr), 
                           atmosphere('.'), canBreakWall(false), energy(0) {}

bool Compare::operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
    return a->fCost > b->fCost;
}

AIBot::AIBot() {}

int AIBot::heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

std::vector<std::pair<int, int>> AIBot::findPath(int startX, int startY, int endX, int endY, const MapManager& mapManager) {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Compare> openSet;
    std::vector closedSet(mapManager.getRows(),std::vector(mapManager.getCols(), false));
    std::vector nodeMap(mapManager.getRows(),std::vector<std::shared_ptr<Node>>(mapManager.getCols(), nullptr));

    auto startNode = std::make_shared<Node>(startX, startY);
    startNode->hCost = heuristic(startX, startY, endX, endY);
    startNode->fCost = startNode->hCost;

    openSet.push(startNode);
    nodeMap[startX][startY] = startNode;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        if (closedSet[current->x][current->y]) continue;
        closedSet[current->x][current->y] = true;

        if (current->x == endX && current->y == endY) {
            // Reconstruct path
            std::vector<std::pair<int, int>> path;
            auto node = current;
            while (node != nullptr) {
                path.push_back({node->x, node->y});
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());

            return path;
        }

        for (int i = 0; i < 4; i++) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (closedSet[newX][newY] || !mapManager.isValidForBot(newX, newY, current->atmosphere, current->canBreakWall))
                continue;

            auto& neighbor = nodeMap[newX][newY];
            if (!neighbor) {
                neighbor = std::make_shared<Node>(newX, newY);
            }

            int tentativeGCost = current->gCost + 1;

            // Update atmosphere if we hit a tank
            char newAtmosphere = current->atmosphere;
            if (mapManager.isTank(mapManager.getCell(newX, newY))) {
                newAtmosphere = mapManager.getCell(newX, newY);
            }

            // Update energy and wall-breaking ability (like the player)
            int newEnergy = current->energy + 1;
            bool newCanBreak = (newEnergy >= maxEnergy);

            // If we're moving through a wall, we need to break it
            if (mapManager.getCell(newX, newY) == '#') {
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

    return {}; // No path found
} 