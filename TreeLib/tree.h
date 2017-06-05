#pragma once

namespace tree_lib {

    typedef struct _treeNode {
        struct _treeNode* left;
        struct _treeNode* right;
        int key;
        int height;
    } treeNode;

    int height(treeNode* node);
    treeNode* createTree();
    treeNode* treeInsert(treeNode* node, int key);
    treeNode* treeRemove(treeNode* node, int key);
    treeNode* treeFind(treeNode* node, int key);
    std::vector<int> *treeToVector(treeNode* node);

}