#ifndef RECORD_BTREE_h
#define RECORD_BTREE_h

#include <iostream>
#include <vector>
#include <string>
#include "RECORD.h"

// A BTree Node structure
class BTreeNode {
public:
    std::vector<Record> records;     // List of records (keys with additional data)
    std::vector<BTreeNode*> children; // Child pointers
    bool isLeaf;                 // Is true if the node is a leaf
    int maxKeys;                 // Maximum number of keys in the node

    // Constructor
    BTreeNode(int maxKeys, bool isLeaf) {
        this->maxKeys = maxKeys;
        this->isLeaf = isLeaf;
    }

    // Traverse the tree and print records
    void traverse() {
        for (int i = 0; i < records.size(); i++) {
            if (!isLeaf)
                children[i]->traverse();
            std::cout << "ID: " << records[i].id << ", Name: " << records[i].name << ", Age: " << records[i].age << std::endl;
        }
        if (!isLeaf)
            children[records.size()]->traverse();
    }

    // Search for a record by ID in the subtree rooted with this node
    Record* search(int id) {
        int i = 0;
        while (i < records.size() && id > records[i].id)
            i++;

        if (i < records.size() && records[i].id == id)
            return &records[i];

        return isLeaf ? nullptr : children[i]->search(id);
    }

    // Remove a record by ID
    void remove(int id, int degree);
    void removeFromLeaf(int index);
    void removeFromNonLeaf(int index, int degree);
    Record getPredecessor(int index);
    Record getSuccessor(int index);
    void merge(int index);
    void fill(int index, int degree);
    void borrowFromPrev(int index);
    void borrowFromNext(int index);

    // Insert a record in a non-full node
    void insertNonFull(Record record);
    // Split a child
    void splitChild(int i, BTreeNode* child);
};

class BTree {
public:
    BTreeNode* root;
    int maxKeys;

    BTree(int degree) {
        root = nullptr;
        maxKeys = degree - 1;
    }

    void traverse() {
        if (root)
            root->traverse();
        std::cout << std::endl;
    }

    Record* search(int id) {
        return root ? root->search(id) : nullptr;
    }

    void insert(Record rec);
    void remove(int id) {
        if (!root)
            return;

        root->remove(id, maxKeys + 1);

        // If the root has no keys, make its first child the new root
        if (root->records.empty()) {
            BTreeNode* tmp = root;
            root = root->isLeaf ? nullptr : root->children[0];
            delete tmp;
        }
    }
};

void BTreeNode::remove(int id, int degree) {
    int index = 0;
    while (index < records.size() && records[index].id < id)
        index++;

    if (index < records.size() && records[index].id == id) {
        if (isLeaf) {
            removeFromLeaf(index);
        } else {
            removeFromNonLeaf(index, degree);
        }
    } else {
        if (isLeaf)
            return; // Key not found

        bool atLastChild = (index == records.size());

        if (children[index]->records.size() < degree / 2)
            fill(index, degree);

        if (atLastChild && index > records.size())
            children[index - 1]->remove(id, degree);
        else
            children[index]->remove(id, degree);
    }
}

void BTreeNode::removeFromLeaf(int index) {
    records.erase(records.begin() + index);
}

void BTreeNode::removeFromNonLeaf(int index, int degree) {
    int id = records[index].id;

    if (children[index]->records.size() >= degree / 2) {
        Record pred = getPredecessor(index);
        records[index] = pred;
        children[index]->remove(pred.id, degree);
    } else if (children[index + 1]->records.size() >= degree / 2) {
        Record succ = getSuccessor(index);
        records[index] = succ;
        children[index + 1]->remove(succ.id, degree);
    } else {
        merge(index);
        children[index]->remove(id, degree);
    }
}

Record BTreeNode::getPredecessor(int index) {
    BTreeNode* current = children[index];
    while (!current->isLeaf)
        current = current->children[current->records.size()];
    return current->records.back();
}

Record BTreeNode::getSuccessor(int index) {
    BTreeNode* current = children[index + 1];
    while (!current->isLeaf)
        current = current->children[0];
    return current->records.front();
}

void BTreeNode::insertNonFull(Record record) {
    int i = records.size() - 1;

    if (isLeaf) {
        records.push_back({0, "", 0}); // Extend size
        while (i >= 0 && records[i].id > record.id) {
            records[i + 1] = records[i];
            i--;
        }
        records[i + 1] = record;
    } else {
        while (i >= 0 && records[i].id > record.id) {
            i--;
        }

        if (children[i + 1]->records.size() == maxKeys) {
            splitChild(i + 1, children[i + 1]);
            if (records[i + 1].id < record.id) {
                i++;
            }
        }
        children[i + 1]->insertNonFull(record);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* child) {
    int mid = maxKeys / 2;
    BTreeNode* newChild = new BTreeNode(maxKeys, child->isLeaf);

    for (int j = mid + 1; j < child->records.size(); j++) {
        newChild->records.push_back(child->records[j]);
    }

    if (!child->isLeaf) {
        for (int j = mid + 1; j <= child->records.size(); j++) {
            newChild->children.push_back(child->children[j]);
        }
    }

    child->records.resize(mid);
    if (!child->isLeaf) {
        child->children.resize(mid + 1);
    }

    children.insert(children.begin() + i + 1, newChild);
    records.insert(records.begin() + i, child->records[mid]);
}


void BTreeNode::merge(int index) {
    BTreeNode* child = children[index];
    BTreeNode* sibling = children[index + 1];

    child->records.push_back(records[index]);

    for (auto& record : sibling->records)
        child->records.push_back(record);

    if (!sibling->isLeaf) {
        for (auto& childPtr : sibling->children)
            child->children.push_back(childPtr);
    }

    records.erase(records.begin() + index);
    children.erase(children.begin() + index + 1);

    delete sibling;
}

void BTreeNode::fill(int index, int degree) {
    if (index != 0 && children[index - 1]->records.size() >= degree / 2)
        borrowFromPrev(index);
    else if (index != records.size() && children[index + 1]->records.size() >= degree / 2)
        borrowFromNext(index);
    else {
        if (index != records.size())
            merge(index);
        else
            merge(index - 1);
    }
}

void BTreeNode::borrowFromPrev(int index) {
    BTreeNode* child = children[index];
    BTreeNode* sibling = children[index - 1];

    child->records.insert(child->records.begin(), records[index - 1]);
    records[index - 1] = sibling->records.back();
    sibling->records.pop_back();

    if (!child->isLeaf) {
        child->children.insert(child->children.begin(), sibling->children.back());
        sibling->children.pop_back();
    }
}

void BTreeNode::borrowFromNext(int index) {
    BTreeNode* child = children[index];
    BTreeNode* sibling = children[index + 1];

    child->records.push_back(records[index]);
    records[index] = sibling->records.front();
    sibling->records.erase(sibling->records.begin());

    if (!child->isLeaf) {
        child->children.push_back(sibling->children.front());
        sibling->children.erase(sibling->children.begin());
    }
}

void BTree::insert(Record record) {
    if (!root) {
        root = new BTreeNode(maxKeys, true);
        root->records.push_back(record);
    } else {
        if (root->records.size() == maxKeys) {
            BTreeNode* newRoot = new BTreeNode(maxKeys, false);
            newRoot->children.push_back(root);
            newRoot->splitChild(0, root);
            root = newRoot;
        }
        root->insertNonFull(record);
    }
}

#endif
