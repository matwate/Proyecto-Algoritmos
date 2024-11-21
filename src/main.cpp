#include <raylib.h>

#include <string>

#include "common/constants.h"
#include "helpers/InputHelper.hpp"
#include "visualizers/GraphVisualizer.hpp"
#include "visualizers/SortVisualizer.hpp"
#include "visualizers/TreeVisualizer.hpp"

// Keep state of the app
enum AppState { MENU, SORT_VISUALIZER, TREE_VISUALIZER, GRAPH_VISUALIZER };

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Algorithms Visualizer");

    SortVisualizer visualizer;
    TreeVisualizer treeVisualizer;
    GraphVisualizer graphVisualizer;

    AppState currentState = MENU;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);

        // Handle state-specific logic
        switch (currentState) {
            case MENU:
                DrawText("MAIN MENU", 300, 100, 40, WHITE);
                DrawText("1. Sorting Algorithms", 300, 200, 30, LIGHTGRAY);
                DrawText("2. Trees", 300, 250, 30, LIGHTGRAY);
                DrawText("3. Graphs", 300, 300, 30, LIGHTGRAY);
                DrawText("Press Esc to Exit", 300, 400, 20, LIGHTGRAY);

                if (IsKeyPressed(KEY_ONE)) currentState = SORT_VISUALIZER;
                if (IsKeyPressed(KEY_TWO)) currentState = TREE_VISUALIZER;
                if (IsKeyPressed(KEY_THREE)) currentState = GRAPH_VISUALIZER;
                break;

            case SORT_VISUALIZER:
                DrawText("[Sort Visualizer] [SPACE] to start sorting", 10, 50,
                         20, WHITE);
                DrawText("[R]eset", 10, 130, 20, WHITE);
                DrawText("[S]huffle", 10, 150, 20, WHITE);
                DrawText("[B]ubble sort", 10, 170, 20, WHITE);
                DrawText("[L] Selection sort", 10, 190, 20, WHITE);
                DrawText("[I] Insertion sort", 10, 210, 20, WHITE);
                DrawText("[Q] Quick sort", 10, 230, 20, WHITE);
                DrawText("[M] Merge sort", 10, 250, 20, WHITE);
                DrawText("[H] Heap sort", 10, 270, 20, WHITE);
                DrawText("[D] Radix LSD sort", 10, 290, 20, WHITE);
                visualizer.Update();
                visualizer.Draw();
                DrawText("Press [M] to go back to Menu", 10, 20, 20, LIGHTGRAY);

                if (IsKeyPressed(KEY_M)) currentState = MENU;
                break;

            case TREE_VISUALIZER:
                DrawText("[Tree Visualizer]", 10, 50, 20, WHITE);
                DrawText("[T] Insert Random Node", 10, 100, 20, WHITE);
                DrawText("[I] In-Order Traversal", 10, 130, 20, WHITE);
                DrawText("[P] Pre-Order Traversal", 10, 160, 20, WHITE);
                DrawText("[O] Post-Order Traversal", 10, 190, 20, WHITE);
                DrawText("[B] Insert Balanced", 10, 220, 20, WHITE);
                DrawText("Press [M] to go back to Menu", 10, 10, 20, LIGHTGRAY);

                if (IsKeyPressed(KEY_T)) {
                    int randomValue = GetRandomValue(1, 100);
                    treeVisualizer.Insert(randomValue);
                }
                if (IsKeyPressed(KEY_I)) treeVisualizer.TraverseInOrder();
                if (IsKeyPressed(KEY_P)) treeVisualizer.TraversePreOrder();
                if (IsKeyPressed(KEY_O)) treeVisualizer.TraversePostOrder();
                if (IsKeyPressed(KEY_B)) treeVisualizer.InsertBalanced();

                treeVisualizer.Draw();

                if (IsKeyPressed(KEY_M)) currentState = MENU;
                break;

            case GRAPH_VISUALIZER:
                DrawText("[Graph Visualizer]", 10, 50, 20, WHITE);
                DrawText("[N] Add Node", 10, 100, 20, WHITE);
                DrawText("[E] Add Edge", 10, 130, 20, WHITE);
                DrawText("[1] Generate Random Graph", 10, 160, 20, WHITE);
                DrawText("[2] BFS Traversal", 10, 190, 20, WHITE);
                DrawText("[3] DFS Traversal", 10, 220, 20, WHITE);
                DrawText("[4] Djikstra", 10, 250, 20, WHITE);
                DrawText("Press [M] to go back to Menu", 10, 10, 20, LIGHTGRAY);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
                    IsKeyPressed(KEY_N)) {
                    int x = GetMouseX();
                    int y = GetMouseY();
                    std::string label = Prompt("Enter label for the node:");
                    graphVisualizer.AddNode(x, y, label);
                }
                if (IsKeyPressed(KEY_E)) {
                    std::string fromIdStr = Prompt("Enter from node ID:");
                    std::string toIdStr = Prompt("Enter to node ID:");
                    std::string weightStr = Prompt("Enter edge weight:");

                    int fromId = std::stoi(fromIdStr);
                    int toId = std::stoi(toIdStr);
                    float weight = std::stof(weightStr);

                    graphVisualizer.AddEdge(fromId, toId, weight);
                }
                if (IsKeyPressed(KEY_ONE)) {
                    std::string numberOfNodesStr =
                        Prompt("Enter number of nodes");

                    int numberOfNodes = std::stoi(numberOfNodesStr);
                    graphVisualizer.GenerateRandomGraph(numberOfNodes, 0.4,
                                                        100);
                }
                if (IsKeyPressed(KEY_TWO)) {
                    int initialNode =
                        std::stoi(Prompt("Enter Id of initial node"));

                    BeginDrawing();
                    ClearBackground(BLACK);
                    graphVisualizer.Draw();
                    EndDrawing();

                    graphVisualizer.BFS(initialNode);
                }
                if (IsKeyPressed(KEY_THREE)) {
                    int initialNode =
                        std::stoi(Prompt("Enter Id of initial node"));

                    BeginDrawing();
                    ClearBackground(BLACK);
                    graphVisualizer.Draw();
                    EndDrawing();

                    graphVisualizer.DFS(initialNode);
                }
                if (IsKeyPressed(KEY_FOUR)) {
                    std::string fromStr = Prompt("from");
                    std::string toStr = Prompt("to:");

                    int from = std::stoi(fromStr);
                    int to = std::stoi(toStr);

                    BeginDrawing();
                    ClearBackground(BLACK);
                    graphVisualizer.Draw();
                    EndDrawing();

                    graphVisualizer.Dijkstra(from, to);
                }

                graphVisualizer.Draw();

                if (IsKeyPressed(KEY_M)) currentState = MENU;
                break;
        }

        EndDrawing();
    }

    CloseWindow();
}
