#ifndef RECORD_AVL_H
#define RECORD_AVL_H

#include <iostream>
#include "RECORD.h"
#include <iostream>
#include <string>


// AVLNode class representing a single node in the AVL tree
class AVLNode {
public:
    Record rec;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Record _rec = Record()) {
        rec = _rec;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

// AVL class encapsulating the AVL tree
class AVL {
private:
    AVLNode* root;

    // Helper function to calculate height
    int height(AVLNode* node) {
        return node == nullptr ? 0 : node->height;
    }

    // Helper function to get the balance factor of a node
    int getBalanceFactor(AVLNode* node) {
        return node == nullptr ? 0 : height(node->left) - height(node->right);
    }

    // Helper function to perform right rotation
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + std::max(height(y->left), height(y->right));
        x->height = 1 + std::max(height(x->left), height(x->right));

        return x;
    }

    // Helper function to perform left rotation
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + std::max(height(x->left), height(x->right));
        y->height = 1 + std::max(height(y->left), height(y->right));

        return y;
    }

    // Balance the AVL tree at the given node
    AVLNode* balanceAVL(AVLNode* node) {
        int balanceFactor = getBalanceFactor(node);

        // Left-heavy case
        if (balanceFactor > 1) {
            if (getBalanceFactor(node->left) >= 0) {
                return rightRotate(node); // Left-Left Case
            } else {
                node->left = leftRotate(node->left); // Left-Right Case
                return rightRotate(node);
            }
        }

        // Right-heavy case
        if (balanceFactor < -1) {
            if (getBalanceFactor(node->right) <= 0) {
                return leftRotate(node); // Right-Right Case
            } else {
                node->right = rightRotate(node->right); // Right-Left Case
                return leftRotate(node);
            }
        }

        return node; // Balanced
    }

    // Helper function to insert a node
    AVLNode* insertAVLNode(AVLNode* node, Record rec) {
        if (node == nullptr) return new AVLNode(rec);

        if (rec.id < node->rec.id)
            node->left = insertAVLNode(node->left, rec);
        else if (rec.id > node->rec.id)
            node->right = insertAVLNode(node->right, rec);
        else
            return node; // Duplicate ID, no insertion

        // Update height and balance
        node->height = 1 + std::max(height(node->left), height(node->right));
        return balanceAVL(node);
    }

    // Helper function to find the node with minimum value
    AVLNode* AVLNodeWithMinimumValue(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    // Helper function to delete a node
    AVLNode* deleteAVLNode(AVLNode* node, int ID) {
        if (node == nullptr) return node;

        if (ID < node->rec.id) {
            node->left = deleteAVLNode(node->left, ID);
        } else if (ID > node->rec.id) {
            node->right = deleteAVLNode(node->right, ID);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                AVLNode* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }

            AVLNode* temp = AVLNodeWithMinimumValue(node->right);
            node->rec = temp->rec;
            node->right = deleteAVLNode(node->right, temp->rec.id);
        }

        // Update height and balance
        node->height = 1 + std::max(height(node->left), height(node->right));
        return balanceAVL(node);
    }

    // Helper function to search for a node
    AVLNode* searchAVLNode(AVLNode* node, int ID) {
        if (node == nullptr || node->rec.id == ID)
            return node;

        if (ID < node->rec.id)
            return searchAVLNode(node->left, ID);
        else
            return searchAVLNode(node->right, ID);
    }

    // Helper function to print the tree in-order
    void printTreeInOrder(AVLNode* node) {
        if (node) {
            printTreeInOrder(node->left);
            std::cout << "ID: " << node->rec.id << ", Name: " << node->rec.name << ", Age: " << node->rec.age << std::endl;
            printTreeInOrder(node->right);
        }
    }

public:
    AVL() : root(nullptr) {}

    void insert(Record rec) {
        root = insertAVLNode(root, rec);
    }

    void remove(int ID) {
        root = deleteAVLNode(root, ID);
    }

    Record* search(int ID) {
        AVLNode* result = searchAVLNode(root, ID);
        return result ? &result->rec : nullptr;
    }

    void print() {
        printTreeInOrder(root);
    }
};

#endif