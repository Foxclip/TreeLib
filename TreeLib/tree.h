#pragma once

namespace btree {

    typedef struct _node {
        int height;
        struct _node* left;
        struct _node* right;
        int key;
    } Node;

    class Tree {
    public:
        Tree();
        ~Tree();
        int getSize();
        int getHeight();
        void insert(int key);
        void remove(int key);
        void clear();
        Node *find(int key);
        std::vector<int> *toVector();

    private:
        int size;
        Node *root;

    };

    Node *createTreeNode(int key);
    int node_height(Node *node);
    Node *rotateRight(Node *node);
    Node *rotateLeft(Node *node);
    void fixheight(Node *node);
    int balanceFactor(Node *node);
    Node *findMin(Node *node);
    Node *removeMin(Node *node);

}