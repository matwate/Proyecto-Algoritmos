#include "GraphVisualizer.hpp"

#include <raylib.h>

#include <cmath>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>

const int ANIMATION_WAIT = 2000;

float CalculateDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

GraphVisualizer::GraphVisualizer() : nextNodeId(0) {}

void GraphVisualizer::AddNode(int x, int y, const std::string& label) {
    nodes[nextNodeId] = GraphNode(nextNodeId, x, y, label);
    std::cout << "Added node with ID: " << nextNodeId << " at (" << x << ", "
              << y << ") with label: " << label << std::endl;
    nextNodeId++;
}

void GraphVisualizer::AddEdge(int fromId, int toId, float weight,
                              bool directed) {
    if (nodes.find(fromId) != nodes.end() && nodes.find(toId) != nodes.end()) {
        edges.emplace_back(fromId, toId, weight);
        std::cout << "Added edge from node " << fromId << " to node " << toId
                  << " with weight " << weight << std::endl;
        if (!directed) {
            edges.emplace_back(toId, fromId, weight);
            std::cout << "Added reverse edge from node " << toId << " to node "
                      << fromId << " with weight " << weight << std::endl;
        }
    } else {
        std::cout << "Invalid node IDs for edge: " << fromId << " -> " << toId
                  << std::endl;
    }
}

void GraphVisualizer::GenerateRandomGraph(int numNodes, float edgeProbability,
                                          int maxWeight) {
    // Clear existing graph
    nodes.clear();
    edges.clear();
    nextNodeId = 0;

    // Minimum distance between nodes
    const int MIN_DISTANCE = 150;

    // Add nodes at random positions
    for (int i = 0; i < numNodes; i++) {
        bool validPosition = false;
        int x, y;

        while (!validPosition) {
            x = GetRandomValue(100, GetScreenWidth() - 100);
            y = GetRandomValue(100, GetScreenHeight() - 100);

            validPosition = true;
            for (const auto& [id, node] : nodes) {
                if (CalculateDistance(x, y, node.x, node.y) < MIN_DISTANCE) {
                    validPosition = false;
                    break;
                }
            }
        }

        AddNode(x, y, "Node" + std::to_string(i));
    }

    for (int i = 0; i < numNodes; i++) {
        for (int j = i + 1; j < numNodes; j++) {
            float probability =
                static_cast<float>(GetRandomValue(0, 100)) / 100.0f;
            if (probability < edgeProbability) {
                int weight = GetRandomValue(1, maxWeight);
                AddEdge(i, j, weight, true);
            }
        }
    }

    std::cout << "Generated random graph with " << numNodes
              << " nodes and edge probability " << edgeProbability << std::endl;
}

void GraphVisualizer::BFS(int startNode) {
    if (nodes.find(startNode) == nodes.end()) {
        std::cout << "Invalid start node for BFS: " << startNode << std::endl;
        return;
    }

    std::queue<int> nodeQueue;              // BFS queue
    std::unordered_map<int, bool> visited;  // Track visited nodes
    std::unordered_set<std::pair<int, int>, PairHash>
        visitedEdges;  // Track visited edges

    nodeQueue.push(startNode);
    visited[startNode] = true;

    while (!nodeQueue.empty()) {
        int currentNode = nodeQueue.front();
        nodeQueue.pop();

        // Highlight the current node as active
        HighlightNode(currentNode, YELLOW);
        std::cout << "Visiting node: " << currentNode << std::endl;

        // Traverse edges and visit neighbors
        for (const auto& edge : edges) {
            if (edge.from == currentNode && !visited[edge.to]) {
                // Highlight edge and add to visited edges
                visitedEdges.insert({edge.from, edge.to});
                HighlightEdge(edge, YELLOW);

                // Mark neighbor as visited and add to queue
                visited[edge.to] = true;
                nodeQueue.push(edge.to);

                // Animation step for edge
                BeginDrawing();
                DrawWithHighlights(visited, visitedEdges);
                EndDrawing();
                WaitForAnimationStep(ANIMATION_WAIT);
            }
        }

        HighlightNode(currentNode, BLUE);

        BeginDrawing();
        DrawWithHighlights(visited, visitedEdges);
        EndDrawing();
        WaitForAnimationStep(ANIMATION_WAIT);
    }
}

void GraphVisualizer::DFS(int startNode) {
    if (nodes.find(startNode) == nodes.end()) {
        std::cout << "Invalid start node for DFS: " << startNode << std::endl;
        return;
    }

    // Stack now tracks edges as (from, to)
    std::stack<std::pair<int, int>> nodeStack;

    // Track visited nodes
    std::unordered_map<int, bool> visited;

    // Track visited edges
    std::unordered_set<std::pair<int, int>, PairHash> visitedEdges;

    // Start with (-1, startNode) to represent the root
    nodeStack.push({-1, startNode});

    while (!nodeStack.empty()) {
        auto [fromNode, currentNode] = nodeStack.top();
        nodeStack.pop();

        // Highlight the edge leading to this node (if it exists)
        if (fromNode != -1 &&
            visitedEdges.count({fromNode, currentNode}) == 0) {
            visitedEdges.insert({fromNode, currentNode});
            HighlightEdge({fromNode, currentNode, 0}, YELLOW);

            // Animation step for edge
            BeginDrawing();
            DrawWithHighlights(visited, visitedEdges);
            EndDrawing();
            WaitForAnimationStep(500);
        }

        if (!visited[currentNode]) {
            // Highlight the current node as active
            HighlightNode(currentNode, YELLOW);
            std::cout << "Visiting node: " << currentNode << std::endl;
            visited[currentNode] = true;

            // Animation step for node
            BeginDrawing();
            DrawWithHighlights(visited, visitedEdges);
            EndDrawing();
            WaitForAnimationStep(500);  // Adjust delay as needed
        }

        // Push unvisited neighbors onto the stack
        for (const auto& edge : edges) {
            if (edge.from == currentNode && !visited[edge.to]) {
                nodeStack.push({currentNode, edge.to});
            }
        }

        // Mark the current node as visited (blue) after processing neighbors
        HighlightNode(currentNode, BLUE);
    }

    // Final render
    BeginDrawing();
    DrawWithHighlights(visited, visitedEdges);
    EndDrawing();
}

struct DijkstraComparator {
    bool operator()(const std::pair<int, float>& a,
                    const std::pair<int, float>& b) {
        return a.second > b.second;  // Min-heap based on distance
    }
};

void GraphVisualizer::Dijkstra(int from, int to) {
    if (nodes.find(from) == nodes.end() || nodes.find(to) == nodes.end()) {
        std::cout << "Invalid source or destination node for Dijkstra: " << from
                  << " -> " << to << std::endl;
        return;
    }

    // Initialize distances and previous nodes for path reconstruction
    std::unordered_map<int, float> distances;
    std::unordered_map<int, int> previous;
    std::unordered_set<int> visited;
    for (const auto& [id, node] : nodes) {
        distances[id] = std::numeric_limits<float>::infinity();
    }
    distances[from] = 0.0f;

    // Priority queue for nodes to process
    std::priority_queue<std::pair<int, float>,
                        std::vector<std::pair<int, float>>, DijkstraComparator>
        pq;
    pq.push({from, 0.0f});

    while (!pq.empty()) {
        auto [currentNode, currentDistance] = pq.top();
        pq.pop();

        // Stop if we reach the destination
        if (currentNode == to) break;

        // Skip if already visited
        if (visited.count(currentNode)) continue;

        // Mark as visited
        visited.insert(currentNode);
        HighlightNode(currentNode, YELLOW);

        // Animation step for node
        BeginDrawing();
        ClearBackground(BLACK);
        DrawWithDistances(distances, visited);
        EndDrawing();
        WaitForAnimationStep(1000);

        // Process all neighbors
        for (const auto& edge : edges) {
            if (edge.from == currentNode && !visited.count(edge.to)) {
                float newDistance = distances[currentNode] + edge.weight;
                if (newDistance < distances[edge.to]) {
                    distances[edge.to] = newDistance;
                    previous[edge.to] = currentNode;
                    pq.push({edge.to, newDistance});

                    // Highlight the edge being relaxed
                    HighlightEdge(edge, YELLOW);

                    // Animation step for edge
                    BeginDrawing();
                    DrawWithDistances(distances, visited);
                    EndDrawing();
                    WaitForAnimationStep(500);
                }
            }
        }

        // Mark the current node as finalized (blue)
        HighlightNode(currentNode, BLUE);
    }

    // Highlight the shortest path
    HighlightShortestPath(from, to, previous);
}

void GraphVisualizer::DrawWithDistances(
    const std::unordered_map<int, float>& distances,
    const std::unordered_set<int>& visited) {
    // Draw edges
    for (const auto& edge : edges) {
        HighlightEdge(edge, LIGHTGRAY);  // Default color for edges
    }

    // Draw nodes
    for (const auto& [id, node] : nodes) {
        if (visited.count(id)) {
            HighlightNode(id, BLUE);  // Finalized nodes in blue
        } else {
            HighlightNode(id, LIGHTGRAY);  // Unvisited nodes in gray
        }

        // Display distance
        if (distances.count(id) &&
            distances.at(id) < std::numeric_limits<float>::infinity()) {
            DrawText(std::to_string(static_cast<int>(distances.at(id))).c_str(),
                     node.x - 15, node.y - 40, 20, RED);
        }
    }
}

void GraphVisualizer::HighlightShortestPath(
    int from, int to, const std::unordered_map<int, int>& previous) {
    int currentNode = to;

    // Check if the destination is reachable
    if (previous.find(to) == previous.end()) {
        std::cout << "No path exists from " << from << " to " << to
                  << std::endl;
        return;
    }

    // Highlight the shortest path
    while (currentNode != from) {
        int previousNode = previous.at(currentNode);

        // Highlight the edge in green
        HighlightEdge({previousNode, currentNode, 0}, GREEN);

        // Highlight the node in green
        HighlightNode(currentNode, GREEN);

        currentNode = previousNode;
    }

    // Highlight the source node
    HighlightNode(from, GREEN);

    // Final render
    BeginDrawing();
    Draw();
    EndDrawing();
}

void GraphVisualizer::WaitForAnimationStep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void GraphVisualizer::Draw() {
    for (const auto& [id, node] : nodes) {
        DrawNode(node);
    }

    for (const auto& edge : edges) {
        DrawEdge(edge, LIGHTGRAY);
    }
}

void GraphVisualizer::DrawNode(const GraphNode& node) {
    DrawCircle(node.x, node.y, 20, LIGHTGRAY);
    DrawText(std::to_string(node.id).c_str(), node.x - 10, node.y - 10, 20,
             BLACK);

    if (!node.label.empty()) {
        DrawText(node.label.c_str(), node.x - 20, node.y + 25, 20, GREEN);
    }
}

void GraphVisualizer::DrawEdge(const GraphEdge& edge, Color color) {
    const GraphNode& fromNode = nodes[edge.from];
    const GraphNode& toNode = nodes[edge.to];

    Vector2 startPos = {static_cast<float>(fromNode.x),
                        static_cast<float>(fromNode.y)};
    Vector2 endPos = {static_cast<float>(toNode.x),
                      static_cast<float>(toNode.y)};
    DrawLineEx(startPos, endPos, 2.0f, color);

    // Calculate the position for the weight
    int midX = (startPos.x + endPos.x) / 2;
    int midY = (startPos.y + endPos.y) / 2;

    // Display the weight
    DrawText(std::to_string(static_cast<int>(edge.weight)).c_str(), midX - 10,
             midY - 10, 20, RED);

    // Draw a circle as the direction indicator
    float fraction = 0.95f;
    float dx = endPos.x - startPos.x;
    float dy = endPos.y - startPos.y;
    Vector2 indicatorPos = {startPos.x + dx * fraction,
                            startPos.y + dy * fraction};
    DrawCircle(static_cast<int>(indicatorPos.x),
               static_cast<int>(indicatorPos.y), 5, YELLOW);
}

void GraphVisualizer::HighlightNode(int nodeId, Color color) {
    if (nodes.find(nodeId) != nodes.end()) {
        const GraphNode& node = nodes[nodeId];
        DrawCircle(node.x, node.y, 25, color);
        DrawText(std::to_string(node.id).c_str(), node.x - 10, node.y - 10, 20,
                 BLACK);
    }
}

void GraphVisualizer::HighlightEdge(const GraphEdge& edge, Color color) {
    const GraphNode& fromNode = nodes[edge.from];
    const GraphNode& toNode = nodes[edge.to];

    Vector2 startPos = {static_cast<float>(fromNode.x),
                        static_cast<float>(fromNode.y)};
    Vector2 endPos = {static_cast<float>(toNode.x),
                      static_cast<float>(toNode.y)};
    DrawLineEx(startPos, endPos, 2.0f, color);
}

void GraphVisualizer::DrawWithHighlights(
    const std::unordered_map<int, bool>& visited,
    const std::unordered_set<std::pair<int, int>, PairHash>& visitedEdges) {
    for (const auto& edge : edges) {
        if (visitedEdges.count({edge.from, edge.to})) {
            DrawEdge(edge, RED);  // Visited edges are red
        } else {
            DrawEdge(edge, LIGHTGRAY);  // Unvisited edges are gray
        }
    }

    for (const auto& [id, node] : nodes) {
        if (visited.count(id) && visited.at(id)) {
            HighlightNode(id, BLUE);  // Visited nodes are blue
        } else {
            HighlightNode(id, LIGHTGRAY);  // Unvisited nodes are gray
        }
    }
}
