#ifndef GRAPHVISUALIZER_H
#define GRAPHVISUALIZER_H

#include <raylib.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ (hash2 << 1);  // Combine hashes
    }
};

// Basic structure for a graph node with a label
struct GraphNode {
    int id;
    int x, y;
    std::string label;

    // Default constructor
    GraphNode() : id(-1), x(0), y(0), label("") {}

    // Constructor with parameters
    GraphNode(int id, int x, int y, const std::string& label)
        : id(id), x(x), y(y), label(label) {}
};

// Basic structure for a directed graph edge with weight
struct GraphEdge {
    int from;
    int to;
    float weight;
    GraphEdge(int from, int to, float weight)
        : from(from), to(to), weight(weight) {}
};

class GraphVisualizer {
   private:
    std::unordered_map<int, GraphNode> nodes;
    std::vector<GraphEdge> edges;
    int nextNodeId;

    // Helper functions
    void DrawNode(const GraphNode& node);
    void DrawEdge(const GraphEdge& edge, Color color);
    void WaitForAnimationStep(int milliseconds);
    void HighlightNode(int nodeId, Color color);
    void HighlightEdge(const GraphEdge& edge, Color color);
    void DrawWithHighlights(
        const std::unordered_map<int, bool>& visited,
        const std::unordered_set<std::pair<int, int>, PairHash>& visitedEdges);

   public:
    GraphVisualizer();

    // Public interface
    void GenerateRandomGraph(int numNodes, float edgeProbability,
                             int maxWeight);
    void AddNode(int x, int y, const std::string& label = "");
    void AddEdge(int fromId, int toId, float weight = 1.0f,
                 bool directed = true);
    void BFS(int startNode);
    void DFS(int startNode);
    void Dijkstra(int from, int to);
    void HighlightShortestPath(int from, int to,
                               const std::unordered_map<int, int>& previous);
    void DrawWithDistances(const std::unordered_map<int, float>& distances,
                           const std::unordered_set<int>& visited);

    void Draw();
};

#endif
