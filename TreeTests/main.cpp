#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include "TreeLib/tree.h"

typedef struct _test {
    std::function<void()> f;
    std::string name;
} Test;

void check(bool condition, std::string fail_message) {
    if(condition) {
        throw std::exception(std::string("FAIL (" + fail_message + ")").c_str());
    }
}

btree::Tree *read_tree(std::string filename) {
    std::fstream stream(filename);
    btree::Tree *tree = new btree::Tree();
    int a;
    while(stream >> a) {
        tree->insert(a);
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

void insertion_test() {
    btree::Tree *tree = read_tree("input.txt");
    std::vector<int> *tree_vector = tree->toVector();
    std::vector<int> *sorted_vector = read_vector("sorted.txt");
    check(*tree_vector != *sorted_vector, "something went wrong");
}

void remove_test() {
    btree::Tree *tree = read_tree("input.txt");
    std::vector<int> *tree_before_remove = tree->toVector();
    std::vector<int> *remove_vector = read_vector("remove.txt");
    for(int number_to_remove : *remove_vector) {
        tree->remove(number_to_remove);
    }
    std::vector<int> *tree_after_remove = tree->toVector();
    std::vector<int> *proper_vector = read_vector("after_remove.txt");
    check(*tree_after_remove != *proper_vector, "something went wrong");
}

void find_test() {
    btree::Tree *tree = new btree::Tree();
    check(tree->find(1) != nullptr, "1 found, but tree is empty");
    tree->insert(1);
    check(tree->find(1) == nullptr, "not found 1");
    check(tree->find(1)->key != 1, "found wrong node");
    check(tree->find(5) != nullptr, "5 found, but is not there");
    tree->insert(5);
    check(tree->find(5) == nullptr, "not found 5");
}

void clear_test() {
    btree::Tree *tree = new btree::Tree();
    tree->clear();
    check(tree->getHeight() != 0, "empty tree height is not zero");
    tree = read_tree("input.txt");
    tree->clear();
    check(tree->getHeight() != 0, "tree height is not zero");
}

bool run_test(Test test) {
    std::cout << test.name << ": ";
    try {
        test.f();
    } catch(std::exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    std::cout << "OK" << std::endl;
    return true;
}

int main() {

    Test tests[] = { {find_test, "FIND TEST"},  {insertion_test, "INSERTION TEST"}, {remove_test, "REMOVE TEST"}, {clear_test, "CLEAR_TEST"} };

    int failed_count = 0;
    for(Test test : tests) {
        if(!run_test(test)) {
            failed_count++;
        }
    }
    std::cout << std::endl;
    if(failed_count > 0) {
        std::cout << failed_count << " tests failed" << std::endl;
    } else {
        std::cout << "All tests passed" << std::endl;
    }
    std::cout << std::endl;

    return 0;

}