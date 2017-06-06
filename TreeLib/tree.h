#pragma once

namespace btree {

    typedef struct _node {
        int height;
        struct _node* left;
        struct _node* right;
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
        Node *find(T key);
        std::vector<T> *toVector();

    private:
        int size;
        Node *root;

    };

    template<typename T> Node *createTreeNode(T key);
    int node_height(Node *node);
    Node *rotateRight(Node *node);
    Node *rotateLeft(Node *node);
    void fixheight(Node *node);
    int balanceFactor(Node *node);
    Node *findMin(Node *node);
    Node *removeMin(Node *node);

}