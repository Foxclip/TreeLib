#pragma once

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <memory>

#define Vector std::vector<T>
#define pVector std::shared_ptr<Vector>
#define pNode std::shared_ptr<Node<T> >

namespace btree {

    template<typename T> struct Node {
        int height;
        pNode left;
        pNode right;
        T key;
    };

    template<typename T> class Tree {
    public:
        Tree();
        ~Tree();
        int getSize();
        int getHeight();
        void insert(const T& key);
        void remove(const T& key);
        void clear();
        pNode find(const T& key);
        pVector toVector();

    private:
        int size;
        pNode root;

    };

    template<typename T> pNode createTreeNode(const T& key);
    template<typename T> int node_height(pNode node);
    template<typename T> pNode rotateRight(pNode node);
    template<typename T> pNode rotateLeft(pNode node);
    template<typename T> void fixheight(pNode node);
    template<typename T> int balanceFactor(pNode node);
    template<typename T> pNode findMin(pNode node);
    template<typename T> pNode removeMin(pNode node);

}



namespace btree {

    template<typename T>
    int node_height(pNode node) {
        return node != nullptr ? node->height : 0;
    }

    template<typename T>
    int balanceFactor(pNode node) {
        return (node_height(node->right) - node_height(node->left));
    }

    template<typename T>
    void fixheight(pNode node) {
        node->height = std::max(node_height(node->left), node_height(node->right)) + 1;
    }

    template<typename T>
    pNode rotateRight(pNode node) {
        pNode newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    template<typename T>
    pNode rotateLeft(pNode node) {
        pNode newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        fixheight(node);
        fixheight(newRoot);
        return newRoot;
    }

    template<typename T>
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

    
    template<typename T> pNode createTreeNode(const T& key) {
        pNode newNode(new Node<T>);
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    template<typename T>
    pNode findMin(pNode node) {
        return node->left ? findMin(node->left) : node;
    }

    template<typename T>
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

    template<typename T> pNode _insert(pNode node, const T& key, int *size) {
        if(node == nullptr) {
            node = createTreeNode(key);
            (*size)++;
            return node;
        }
        if(key < node->key) node->left = _insert(node->left,  key, size);
        if(key > node->key) node->right = _insert(node->right, key, size);
        return balance(node);
    }

    template<typename T> void Tree<T>::insert(const T& key) {
        root = _insert(root, key, &size);
    }

    template<typename T> pNode _remove(pNode node, const T& key, int *size) {
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

    template<typename T> void Tree<T>::remove(const T& key) {
        root = _remove(root, key, &size);
    }

    template<typename T>
    void _remove_one_node(pNode node) {
        node.reset();
    }

    template<typename T>
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

    template<typename T> pNode _find(pNode node, const T& key) {
        if(node == nullptr) return nullptr;
        if(key == node->key) return node;
        if(key < node->key) return _find(node->left, key);
        if(key > node->key) return _find(node->right, key);
    }

    template<typename T> pNode Tree<T>::find(const T& key) {
        return _find(root, key);
    }

}