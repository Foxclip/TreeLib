#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include <memory>
#include "TreeLib/tree.h"

class Word {
public:
    Word(std::string str): data(str) {}
    const bool operator>(const Word& w) const {
        return strcmp(data.c_str(), w.data.c_str()) > 0;
    }
    const bool operator<(const Word& w) const {
        return strcmp(data.c_str(), w.data.c_str()) < 0;
    }
    std::string getData() {
        return data;
    }

private:
    std::string data;

};

typedef std::shared_ptr<Word> pWord;
typedef btree::Tree<Word> TreeWrd;
typedef std::shared_ptr<TreeWrd> pTreeWrd;
typedef std::vector<Word> VectorWrd;
typedef std::shared_ptr<VectorWrd> pVectorWrd;

typedef struct _test {
    std::function<void()> f;
    std::string name;
} Test;

void check(bool condition, std::string fail_message) {
    if(condition) {
        throw std::exception(std::string("FAIL (" + fail_message + ")").c_str());
    }
}

typedef btree::Tree<int> TreeInt;
typedef std::unique_ptr<TreeInt> pTreeInt;
typedef std::vector<int> VectorInt;
typedef std::shared_ptr<VectorInt> pVectorInt;

pTreeInt read_tree_int(std::string filename) {
    std::fstream stream(filename);
    pTreeInt tree(new TreeInt);
    int a;
    while(stream >> a) {
        tree->insert(a);
    }
    stream.close();
    return tree;
}

pTreeWrd read_tree_wrd(std::string filename) {
    std::fstream stream(filename);
    pTreeWrd tree(new TreeWrd);
    std::string a;
    while(stream >> a) {
        Word w(a);
        tree->insert(w);
    }
    stream.close();
    return tree;
}

pVectorInt read_vector_int(std::string filename) {
    std::fstream stream(filename);
    pVectorInt vector(new VectorInt);
    int a;
    while(stream >> a) {
        vector->push_back(a);
    }
    return vector;
}

pVectorWrd read_vector_wrd(std::string filename) {
    std::fstream stream(filename);
    pVectorWrd vector(new pVectorWrd);
    std::string a;
    while(stream >> a) {
        Word w(a);
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

void insertion_test_1() {
    pTreeInt tree = read_tree_int("input.txt");
    pVectorInt tree_vector = tree->toVector();
    pVectorInt sorted_vector = read_vector_int("sorted.txt");
    check(*tree_vector != *sorted_vector, "something went wrong");
}

void insertion_test_2() {
    pTreeWrd tree = read_tree_wrd("words.txt");
    pVectorWrd tree_vector = tree->toVector();
    pVectorWrd sorted_vector = read_vector_wrd("sorted.txt");
    check(*tree_vector != *sorted_vector, "something went wrong");
}

void remove_test() {
    pTreeInt tree = read_tree_int("input.txt");
    pVectorInt tree_before_remove = tree->toVector();
    pVectorInt remove_vector = read_vector_int("remove.txt");
    for(int number_to_remove : *remove_vector) {
        tree->remove(number_to_remove);
    }
    pVectorInt tree_after_remove = tree->toVector();
    pVectorInt proper_vector = read_vector_int("after_remove.txt");
    check(*tree_after_remove != *proper_vector, "something went wrong");
}

void find_test() {
    btree::Tree<int> *tree = new btree::Tree<int>();
    check(tree->find(5) != nullptr, "1 found, but tree is empty");
    tree->insert(1);
    check(tree->find(1) == nullptr, "not found 1");
    check(tree->find(1)->key != 1, "found wrong node");
    check(tree->find(5) != nullptr, "5 found, but is not there");
    tree->insert(5);
    check(tree->find(5) == nullptr, "not found 5");
}

void clear_test() {
    pTreeInt tree(new btree::Tree<int>());
    tree->clear();
    check(tree->getHeight() != 0, "empty tree height is not zero");
    tree = read_tree_int("input.txt");
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

    Test tests[] = { {find_test, "FIND TEST"},  {insertion_test_1, "INSERTION TEST"}, {remove_test, "REMOVE TEST"}, {clear_test, "CLEAR TEST"} };

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