#ifndef RECORD_BST_H
#define RECORD_BST_H

#include <iostream>
#include "RECORD.h"

// BSTNode class representing a node in the Binary Search Tree
class BSTNode {
public:
    Record rec;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Record _rec = Record(), BSTNode* _left = nullptr, BSTNode* _right = nullptr)
        : rec(_rec), left(_left), right(_right) {}
};

// BST class encapsulating the Binary Search Tree
class BST {
private:
    BSTNode* root;

    // Helper function for insertion
    BSTNode* insertNode(BSTNode* tree, Record rec) {
        if (tree == nullptr)
            return new BSTNode(rec);

        if (tree->rec.id == rec.id)
            return tree;

        if (rec.id < tree->rec.id)
            tree->left = insertNode(tree->left, rec);
        else
            tree->right = insertNode(tree->right, rec);

        return tree;
    }

    // Helper function for finding a node
    BSTNode* findNode(BSTNode* tree, int ID) {
        if (tree == nullptr || tree->rec.id == ID)
            return tree;

        if (ID < tree->rec.id)
            return findNode(tree->left, ID);
        else
            return findNode(tree->right, ID);
    }

    // Helper functions for traversal
    void preOrderTraversal(BSTNode* tree) {
        if (tree != nullptr) {
            std::cout << "ID: " << tree->rec.id << ", Name: " << tree->rec.name << ", Age: " << tree->rec.age << std::endl;
            preOrderTraversal(tree->left);
            preOrderTraversal(tree->right);
        }
    }

    void inOrderTraversal(BSTNode* tree) {
        if (tree != nullptr) {
            inOrderTraversal(tree->left);
            std::cout << "ID: " << tree->rec.id << ", Name: " << tree->rec.name << ", Age: " << tree->rec.age << std::endl;
            inOrderTraversal(tree->right);
        }
    }

    void postOrderTraversal(BSTNode* tree) {
        if (tree != nullptr) {
            postOrderTraversal(tree->left);
            postOrderTraversal(tree->right);
            std::cout << "ID: " << tree->rec.id << ", Name: " << tree->rec.name << ", Age: " << tree->rec.age << std::endl;
        }
    }

    void inOrderDescendingTraversal(BSTNode* tree) {
        if (tree != nullptr) {
            inOrderDescendingTraversal(tree->right);
            std::cout << "ID: " << tree->rec.id << ", Name: " << tree->rec.name << ", Age: " << tree->rec.age << std::endl;
            inOrderDescendingTraversal(tree->left);
        }
    }

    // Helper function to find the minimum value node
    int getMinValue(BSTNode* tree) {
        if (tree->left == nullptr)
            return tree->rec.id;
        return getMinValue(tree->left);
    }

    // Helper function for node deletion
    BSTNode* deleteNode(BSTNode* tree, int ID) {
        if (tree == nullptr)
            return tree;

        if (ID < tree->rec.id) {
            tree->left = deleteNode(tree->left, ID);
        } else if (ID > tree->rec.id) {
            tree->right = deleteNode(tree->right, ID);
        } else {
            if (tree->left == nullptr && tree->right == nullptr) {
                delete tree;
                return nullptr;
            }

            if (tree->left == nullptr) {
                BSTNode* temp = tree->right;
                delete tree;
                return temp;
            }

            if (tree->right == nullptr) {
                BSTNode* temp = tree->left;
                delete tree;
                return temp;
            }

            int minValue = getMinValue(tree->right);
            BSTNode* minNode = findNode(tree, minValue);
            tree->rec = minNode->rec;
            tree->right = deleteNode(tree->right, minValue);
        }

        return tree;
    }

    // Helper functions to find in-order predecessor and successor
    int inOrderPredecessor(BSTNode* tree, int ID) {
        int predecessor = -1;
        while (tree != nullptr) {
            if (tree->rec.id < ID) {
                predecessor = tree->rec.id;
                tree = tree->right;
            } else {
                tree = tree->left;
            }
        }
        return predecessor;
    }

    int inOrderSuccessor(BSTNode* tree, int ID) {
        int successor = -1;
        while (tree != nullptr) {
            if (tree->rec.id > ID) {
                successor = tree->rec.id;
                tree = tree->left;
            } else {
                tree = tree->right;
            }
        }
        return successor;
    }

public:
    BST() : root(nullptr) {}

    void insert(Record rec) {
        root = insertNode(root, rec);
    }

    Record* search(int ID) {
        BSTNode* node = findNode(root, ID);
        return node ? &node->rec : nullptr;
    }

    void remove(int ID) {
        root = deleteNode(root, ID);
    }

    void preOrder() {
        preOrderTraversal(root);
    }

    void inOrder() {
        inOrderTraversal(root);
    }

    void postOrder() {
        postOrderTraversal(root);
    }

    void inOrderDescending() {
        inOrderDescendingTraversal(root);
    }

    int getInOrderPredecessor(int ID) {
        return inOrderPredecessor(root, ID);
    }

    int getInOrderSuccessor(int ID) {
        return inOrderSuccessor(root, ID);
    }
};

#endif
