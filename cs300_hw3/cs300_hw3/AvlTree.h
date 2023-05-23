//
//  cs300_hw3
//
//  Created by Doruk Dulger
//

#ifndef AvlTree_h
#define AvlTree_h

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

template <class Comparable>
struct AvlNode {
    AvlNode *left;
    AvlNode *right;
    AvlNode *parent;
    
    const Comparable value;
    int count;  // how many nodes are there in this subtree
    int height;
    
    AvlNode(Comparable value);
    void updateValues();
    int balanceFactor();
    AvlNode* left_rotate();
    AvlNode* right_rotate();
};

template <class Comparable>
class AVLTree {
    int _size;
    AvlNode<Comparable> *root;
    void balance(vector<AvlNode<Comparable>**> path);
    
    
public:
    AVLTree();
    ~AVLTree();
    
    void insert(Comparable value);
    void remove(Comparable value);
    
    void clear();
    bool empty() const;
    int size() const;
    
    
    int find(Comparable value) const;
    int lower_bound(Comparable value) const;
    int upper_bound(Comparable value) const;
    
    const Comparable& find_min() const;
    const Comparable& find_max() const;
    const Comparable& operator[](std::size_t idx) const;
    void drawTreeToFile(std::string filename) const;
    std::vector<Comparable> inorderSort() const;
    void inorderSort(AvlNode<Comparable>* node,std::vector<Comparable>& sortedList) const;
    std::vector<Comparable> preorderSort() const;
    void preorderSort(AvlNode<Comparable>* node,std::vector<Comparable>& sortedList) const;
    int getHeight(AvlNode<Comparable>* node) const;
    void printBalanceInfo() const;
    
private:
    using Node = AvlNode<Comparable>;
    void drawTreeToFileHelper(std::ofstream& file, const Node* node, int level, bool is_right) const;
    int heightLeftSubtree(const typename AVLTree<Comparable>::Node* node) const;
    int heightRightSubtree(const typename AVLTree<Comparable>::Node* node) const;
    bool checkBalance() const;
    bool checkBalanceHelper(const typename AVLTree<Comparable>::Node* node) const;
    AvlNode<Comparable>* getRoot() const {
        return root;
    }
};


#include "AvlTree.cpp"

#endif /* AvlTree_h */
