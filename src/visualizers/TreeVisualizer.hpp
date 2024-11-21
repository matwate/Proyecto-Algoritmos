#ifndef TREEVISUALIZER_H
#define TREEVISUALIZER_H

#include <raylib.h>

#include <vector>

// Basic structure for a binary tree node
struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;

    // Visualization coordinates
    int x, y;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class TreeVisualizer {
   private:
    TreeNode* root;
    std::vector<int> traversalResult;
    std::vector<int> values;  // Store values for balanced insertion
    bool isSorting;

    void InsertNode(TreeNode*& node, int value);
    TreeNode* BuildBalancedTree(int left, int right);
    void AnimateRebalance(int left, int right);
    void InOrderTraversal(TreeNode* node);
    void PreOrderTraversal(TreeNode* node);
    void PostOrderTraversal(TreeNode* node);
    void DrawTree(TreeNode* node, int x, int y, int level);

    void HighlightNode(TreeNode* mode);

   public:
    TreeVisualizer();
    ~TreeVisualizer();

    // Public interface
    void ClearTree(TreeNode*& node);
    TreeNode* FindNode(TreeNode* node, int value);
    void Insert(int value);
    void InsertBalanced();
    void TraverseInOrder();
    void TraversePreOrder();
    void TraversePostOrder();
    void Draw();
};

#endif
