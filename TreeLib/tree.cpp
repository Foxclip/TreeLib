#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"

namespace btree {

    int node_height(Node *node) {
        return node != nullptr ? node->height : 0;
    }

    int balanceFactor(Node *node) {
        return (node_height(node->right) - node_height(node->left));
    }

    void fixheight(Node *node) {
        node->height = std::max(node_height(node->left), node_height(node->right)) + 1;
    }

    Node *rotateRight(Node *node) {
        Node *newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    Node *rotateLeft(Node* node) {
        Node *newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    Node *balance(Node *node) {
        fixheight(node);
        if(balanceFactor(node) == 2) {
            if(balanceFactor(node->right) < 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        if(balanceFactor(node) == -2) {
            if(balanceFactor(node->left) > 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        return node;
    }

    
    template<typename T> Node *createTreeNode(T key) {
        Node *newNode = new Node;
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    Node *findMin(Node *node) {
        return node->left ? findMin(node->left) : node;
    }

    Node *removeMin(Node *node) {
        if(node->left == nullptr) return node->right;
        node->left = removeMin(node->left);
        return balance(node);
    }

    template<typename T> Tree<T>::Tree() {
        root = nullptr;
    }

    template<typename T> Tree<T>::~Tree() {
        clear();
    }

    template<typename T> int Tree<T>::getSize() {
        return size;
    }

    template<typename T> int Tree<T>::getHeight() {
        return node_height(root);
    }

    template<typename T> Node *_insert(Node *node, T key, int *size) {
        if(node == nullptr) {
            node = createTreeNode(key);
            (*size)++;
            return node;
        }
        if(key < node->key) node->left =  _insert(node->left,  key, size);
        if(key > node->key) node->right = _insert(node->right, key, size);
        return balance(node);
    }

    template<typename T> void Tree<T>::insert(T key) {
        root = _insert(root, key, &size);
    }

    template<typename T> Node *_remove(Node *node, T key, int *size) {
        if(node == nullptr) return nullptr;
        if(key == node->key) {
            Node *leftNode  = node->left;
            Node *rightNode = node->right;
            delete node;
            if(rightNode == nullptr) return leftNode;
            Node *min = findMin(rightNode);
            min->right = removeMin(rightNode);
            min->left = leftNode;
            (*size)--;
            return balance(min);
        }
        if(key < node->key) node->left =  _remove(node->left,  key, size);
        if(key > node->key) node->right = _remove(node->right, key, size);
        return balance(node);
    }

    template<typename T> void Tree<T>::remove(T key) {
        root = _remove(root, key, &size);
    }

    void _remove_one_node(Node *node) {
        delete node;
    }

    void _clear(Node *node) {
        if(node == nullptr) return;
        _clear(node->left);
        _clear(node->right);
        _remove_one_node(node);
    }

    template<typename T> void Tree<T>::clear() {
        _clear(root);
        root = nullptr;
    }

    template<typename T> void _tree_to_vector(std::vector<T> *vector, Node *node) {
        if(node == nullptr) return;
        _tree_to_vector(vector, node->left);
        vector->push_back(node->key);
        _tree_to_vector(vector, node->right);
    }

    template<typename T> std::vector<T> *Tree<T>::toVector() {
        std::vector<T> *vector = new std::vector<T>;
        _tree_to_vector(vector, root);
        return vector;
    }

    template<typename T> Node *_find(Node *node, T key) {
        if(node == nullptr) return nullptr;
        if(key == node->key) return node;
        if(key < node->key) return _find(node->left, key);
        if(key > node->key) return _find(node->right, key);
    }

    template<typename T> Node *Tree<T>::find(T key) {
        return _find(root, key);
    }

}