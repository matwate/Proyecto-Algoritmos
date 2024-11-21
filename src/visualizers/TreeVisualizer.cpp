#include "TreeVisualizer.hpp"

#include <raylib.h>

#include <algorithm>
#include <chrono>
#include <thread>

TreeVisualizer::TreeVisualizer() : root(nullptr) {}

TreeVisualizer::~TreeVisualizer() {
    // TODO: Add proper tree cleanup (deletion of nodes)
}

void TreeVisualizer::ClearTree(TreeNode*& node) {
    if (node == nullptr) return;
    ClearTree(node->left);
    ClearTree(node->right);
    delete node;
    node = nullptr;
}

TreeNode* TreeVisualizer::FindNode(TreeNode* node, int value) {
    if (node == nullptr) return nullptr;
    if (node->value == value) return node;
    if (value < node->value) return FindNode(node->left, value);
    return FindNode(node->right, value);
}

void TreeVisualizer::Insert(int value) {
    values.push_back(value);
    InsertNode(root, value);
}

void TreeVisualizer::InsertNode(TreeNode*& node, int value) {
    if (node == nullptr) {
        node = new TreeNode(value);
    } else if (value < node->value) {
        InsertNode(node->left, value);
    } else {
        InsertNode(node->right, value);
    }
}

// Build a balanced tree from the sorted values
TreeNode* TreeVisualizer::BuildBalancedTree(int left, int right) {
    if (left > right) return nullptr;

    int mid = left + (right - left) / 2;
    TreeNode* node = new TreeNode(values[mid]);

    node->left = BuildBalancedTree(left, mid - 1);
    node->right = BuildBalancedTree(mid + 1, right);

    return node;
}

void TreeVisualizer::InsertBalanced() {
    std::sort(values.begin(), values.end());
    ClearTree(root);
    root = nullptr;
    isSorting = true;  // Temporarily disable main loop drawing
    AnimateRebalance(0, values.size() - 1);
    isSorting = false;  // Re-enable main loop drawing
}

// Helper function to animate rebalancing of the tree
void TreeVisualizer::AnimateRebalance(int left, int right) {
    if (left > right) return;

    int mid = left + (right - left) / 2;
    TreeNode* node = FindNode(root, values[mid]);

    InsertNode(root, values[mid]);
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTree(root, GetScreenWidth() / 2, 50, 1);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Recurse for left and right halves
    AnimateRebalance(left, mid - 1);
    AnimateRebalance(mid + 1, right);
}

void TreeVisualizer::HighlightNode(TreeNode* node) {
    if (node == nullptr) return;
    DrawTree(root, GetScreenWidth() / 2, 50, 1);
    DrawCircle(node->x, node->y, 20, RED);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void TreeVisualizer::TraverseInOrder() {
    isSorting = true;
    InOrderTraversal(root);
    isSorting = false;
}

void TreeVisualizer::InOrderTraversal(TreeNode* node) {
    if (node == nullptr || !isSorting) return;
    InOrderTraversal(node->left);
    HighlightNode(node);
    InOrderTraversal(node->right);
}

void TreeVisualizer::TraversePreOrder() {
    isSorting = true;
    PreOrderTraversal(root);
    isSorting = false;
}

void TreeVisualizer::PreOrderTraversal(TreeNode* node) {
    if (node == nullptr || !isSorting) return;
    HighlightNode(node);
    PreOrderTraversal(node->left);
    PreOrderTraversal(node->right);
}

void TreeVisualizer::TraversePostOrder() {
    isSorting = true;
    PostOrderTraversal(root);
    isSorting = false;
}

void TreeVisualizer::PostOrderTraversal(TreeNode* node) {
    if (node == nullptr || !isSorting) return;
    PostOrderTraversal(node->left);
    PostOrderTraversal(node->right);
    HighlightNode(node);
}

void TreeVisualizer::Draw() {
    if (root != nullptr) {
        DrawTree(root, GetScreenWidth() / 2, 50, 1);
    }
}

void TreeVisualizer::DrawTree(TreeNode* node, int x, int y, int level) {
    if (node == nullptr) return;

    const int HORIZONTAL_OFFSET = 230;
    const int VERTICAL_OFFSET = 100;

    // Set the node's position
    node->x = x;
    node->y = y;

    // Draw current node
    DrawCircle(x, y, 22, LIGHTGRAY);
    DrawText(std::to_string(node->value).c_str(), x - 10, y - 10, 22, BLACK);

    // Draw left child
    if (node->left != nullptr) {
        DrawLine(x, y, x - HORIZONTAL_OFFSET / (level * 0.8),
                 y + VERTICAL_OFFSET, WHITE);
        DrawTree(node->left, x - HORIZONTAL_OFFSET / (level * 0.8),
                 y + VERTICAL_OFFSET, level + 1);
    }

    // Draw right child
    if (node->right != nullptr) {
        DrawLine(x, y, x + HORIZONTAL_OFFSET / (level * 0.8),
                 y + VERTICAL_OFFSET, WHITE);
        DrawTree(node->right, x + HORIZONTAL_OFFSET / (level * 0.8),
                 y + VERTICAL_OFFSET, level + 1);
    }
}
