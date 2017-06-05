#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"
 
namespace tree_lib {

    int height(treeNode* node) {
        if(node)
            return node->height;
        else
            return 0;
    }

    int balanceFactor(treeNode* node) {
        return (height(node->right) - height(node->left));
    }

    void fixheight(treeNode* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    treeNode* rotateRight(treeNode* node) {
        treeNode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    treeNode* rotateLeft(treeNode* node) {
        treeNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    treeNode* balance(treeNode* node) {
        fixheight(node);
        if(balanceFactor(node) == 2) {
            if(balanceFactor(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if(balanceFactor(node) == -2) {
            if(balanceFactor(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        return node;
    }

    treeNode* createTree() {
        treeNode* newTree = NULL;
        return newTree;
    }

    treeNode* createTreeNode(int key) {
        treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
        newNode->key = key;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    treeNode* treeInsert(treeNode* node, int key) {
        if(!node) {
            node = createTreeNode(key);
            return node;
        }
        if(key < node->key)
            node->left = treeInsert(node->left, key);
        else if(key > node->key)
            node->right = treeInsert(node->right, key);
        return balance(node);
    }

    treeNode* findMin(treeNode* node) {
        return node->left ? findMin(node->left) : node;
    }

    treeNode* removeMin(treeNode* node) {
        if(!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return balance(node);
    }

    treeNode* treeRemove(treeNode* node, int key) {
        if(!node)
            return NULL;
        if(key < node->key)
            node->left = treeRemove(node->left, key);
        else if(key > node->key)
            node->right = treeRemove(node->right, key);
        else {
            treeNode* leftNode = node->left;
            treeNode* rightNode = node->right;
            delete node;
            if(!rightNode)
                return leftNode;
            treeNode* min = findMin(rightNode);
            min->right = removeMin(rightNode);
            min->left = leftNode;
            return balance(min);
        }
        return balance(node);
    }

    void _tree_to_vector_(std::vector<int> *vector, treeNode* node) {
        if(node) {
            _tree_to_vector_(vector, node->left);
            vector->push_back(node->key);
            _tree_to_vector_(vector, node->right);
        }
    }

    std::vector<int> *treeToVector(treeNode *tree) {
        std::vector<int> *vector = new std::vector<int>;
        _tree_to_vector_(vector, tree);
        return vector;
    }

    treeNode* treeFind(treeNode* node, int key) {
        treeNode* currentNode = node;
        while(currentNode) {
            if(key == currentNode->key) {
                return currentNode;
            }
            else if(key > currentNode->key) {
                currentNode = currentNode->right;
            }
            else if(key < currentNode->key) {
                currentNode = currentNode->left;
            }
        }
        return NULL;
    }

}