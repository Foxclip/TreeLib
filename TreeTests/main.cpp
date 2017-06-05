#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "TreeLib/tree.h"

void check(bool condition, std::string fail_message) {
    if(condition) {
        throw std::exception(std::string("FAIL (" + fail_message + ")\n").c_str());
    }
}

tree_lib::treeNode *read_tree(std::string filename) {
    std::fstream stream(filename);
    tree_lib::treeNode *tree = tree_lib::createTree();
    int a;
    while(stream >> a) {
        tree = treeInsert(tree, a);
    }
    stream.close();
    return tree;
}

std::vector<int> *read_vector(std::string filename) {
    std::fstream stream(filename);
    std::vector<int> *vector = new std::vector<int>;
    int a;
    while(stream >> a) {
        vector->push_back(a);
    }
    return vector;
}

void print_vector(std::vector<int> *vector) {
    for(int number : *vector) {
        std::cout << number << " ";
    }
    std::cout << std::endl;
}

void creation_test() {
    printf("CREATION TEST: ");
    tree_lib::treeNode *tree = tree_lib::createTree();
    try {
        check(tree != nullptr, "root pointer is not NULL");
    } catch(std::exception e) {
        printf(e.what());
        return;
    }
    printf("OK\n");
}

void insertion_test_1() {
    printf("INSERTION TEST 1: ");
    tree_lib::treeNode *tree = tree_lib::createTree();
    try {
        tree = treeInsert(tree, 5);
        tree = treeInsert(tree, 1);
        tree = treeInsert(tree, 10);
        check(!tree, "root pointer is NULL");
        check(!tree->left, "left is NULL");
        check(!tree->right, "right is NULL");
        check(tree->key != 5, "key of root is not 5");
        check(tree->left->key != 1, "key of left is not 1");
        check(tree->right->key != 10, "key of right is not 10");
    } catch(std::exception e) {
        printf(e.what());
        return;
    }
    printf("OK\n");
}

void insertion_test_2() {
    printf("INSERTION TEST 2: ");
    try {
        tree_lib::treeNode *tree = read_tree("input.txt");
        std::vector<int> *tree_vector =  treeToVector(tree);
        std::vector<int> *sorted_vector = read_vector("sorted.txt");
        check(*tree_vector != *sorted_vector, "something went wrong");
    } catch(std::exception e) {
        printf(e.what());
        return;
    }
    printf("OK\n");
}

void remove_test_1() {
    printf("REMOVE TEST 1: ");
    tree_lib::treeNode *tree = tree_lib::createTree();
    try {
        tree = treeInsert(tree, 5);
        tree = treeInsert(tree, 1);
        tree = treeInsert(tree, 10);
        tree = treeRemove(tree, 5);
        check(tree->key != 1 && tree->key != 10, "invalid root key");
    } catch(std::exception e) {
        printf(e.what());
        return;
    }
    printf("OK\n");
}

void remove_test_2() {
    printf("REMOVE TEST 2: ");
    try {
        tree_lib::treeNode *tree = read_tree("input.txt");
        std::vector<int> *tree_before_remove = treeToVector(tree);
        std::vector<int> *remove_vector = read_vector("remove.txt");
        for(int number_to_remove : *remove_vector) {
            tree_lib::treeRemove(tree, number_to_remove);
        }
        std::vector<int> *tree_after_remove = treeToVector(tree);
        std::vector<int> *proper_vector = read_vector("after_remove.txt");
        check(*tree_after_remove != *proper_vector, "something went wrong");
    } catch(std::exception e) {
        printf(e.what());
        return;
    }
    printf("OK\n");
}

void find_test() {
    printf("FIND TEST: ");
    tree_lib::treeNode *tree = tree_lib::createTree();
    try {
        check(tree != nullptr, "root pointer is not NULL");
        tree_lib::treeNode *one = treeFind(tree, 1);
        check(one != nullptr, "1 found, but tree is empty");
        tree = treeInsert(tree, 1);
        one = treeFind(tree, 1);
        check(!one, "not found 1");
        check(one->key != 1, "found wrong node");
    } catch(std::exception e) {
        printf(e.what());
        return;
    }
    printf("OK\n");
}

int main() {

    creation_test();
    insertion_test_1();
    insertion_test_2();
    find_test();
    remove_test_1();
    remove_test_2();

    return 0;

}