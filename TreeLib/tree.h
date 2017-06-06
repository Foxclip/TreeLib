#pragma once

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <memory>

#define Vector std::vector<T>
#define pVector std::shared_ptr<Vector>
#define _pNode std::shared_ptr<struct _node>
#define pNode std::shared_ptr<Node>

namespace btree {

    typedef struct _node {
        int height;
        _pNode left;
        _pNode right;
        int key;
    } Node;

    template<typename T> class Tree {
    public:
        Tree();
        ~Tree();
        int getSize();
        int getHeight();
        void insert(T key);
        void remove(T key);
        void clear();
        pNode find(T key);
        pVector toVector();

    private:
        int size;
        pNode root;

    };

    template<typename T> pNode createTreeNode(T key);
    int node_height(pNode node);
    pNode rotateRight(pNode node);
    pNode rotateLeft(pNode node);
    void fixheight(pNode node);
    int balanceFactor(pNode node);
    pNode findMin(pNode node);
    pNode removeMin(pNode node);

}



namespace btree {

    int node_height(pNode node) {
        return node != nullptr ? node->height : 0;
    }

    int balanceFactor(pNode node) {
        return (node_height(node->right) - node_height(node->left));
    }

    void fixheight(pNode node) {
        node->height = std::max(node_height(node->left), node_height(node->right)) + 1;
    }

    pNode rotateRight(pNode node) {
        pNode newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    pNode rotateLeft(pNode node) {
        pNode newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    pNode balance(pNode node) {
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

    
    template<typename T> pNode createTreeNode(T key) {
        pNode newNode(new Node);
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    pNode findMin(pNode node) {
        return node->left ? findMin(node->left) : node;
    }

    pNode removeMin(pNode node) {
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

    template<typename T> pNode _insert(pNode node, T key, int *size) {
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

    template<typename T> pNode _remove(pNode node, T key, int *size) {
        if(node == nullptr) return nullptr;
        if(key == node->key) {
            pNode leftNode  = node->left;
            pNode rightNode = node->right;
            node.reset();
            if(rightNode == nullptr) return leftNode;
            pNode min = findMin(rightNode);
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

    void _remove_one_node(pNode node) {
        node.reset();
    }

    void _clear(pNode node) {
        if(node == nullptr) return;
        _clear(node->left);
        _clear(node->right);
        _remove_one_node(node);
    }

    template<typename T> void Tree<T>::clear() {
        _clear(root);
        root = nullptr;
    }

    template<typename T> void _tree_to_vector(pVector vector, pNode node) {
        if(node == nullptr) return;
        _tree_to_vector(vector, node->left);
        vector->push_back(node->key);
        _tree_to_vector(vector, node->right);
    }

    template<typename T> pVector Tree<T>::toVector() {
        pVector vector(new Vector);
        _tree_to_vector(vector, root);
        return vector;
    }

    template<typename T> pNode _find(pNode node, T key) {
        if(node == nullptr) return nullptr;
        if(key == node->key) return node;
        if(key < node->key) return _find(node->left, key);
        if(key > node->key) return _find(node->right, key);
    }

    template<typename T> pNode Tree<T>::find(T key) {
        return _find(root, key);
    }

}