//
//  cs300_hw3
//
//  Created by Doruk Dulger
//

#include "AvlTree.h"
#include "Contact.h"
#include <iostream>
#include <string>

using namespace std;



template <class Comparable>
AvlNode<Comparable>::AvlNode(Comparable value): value(value){
    count = 1;
    height = 1;
    
    left = nullptr;
    right = nullptr;
}

template <class Comparable>
void AvlNode<Comparable>::updateValues(){
    count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;
    
    height = max(left != nullptr ? left->height : 0,
                 right != nullptr ? right->height : 0) + 1;
}

template <class Comparable>
int AvlNode<Comparable>::balanceFactor(){
    return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height: 0);
}

template <class Comparable>
AvlNode<Comparable>* AvlNode<Comparable>::left_rotate(){
    AvlNode* R = right;
    right = right->left;
    R->left = this;
    
    this->updateValues();  // the order is important
    R->updateValues();
    
    return R;
}

template <class Comparable>
AvlNode<Comparable>* AvlNode<Comparable>::right_rotate(){
    AvlNode* L = left;
    left = left->right;
    L->right = this;
    
    this->updateValues();  // the order is important
    L->updateValues();
    
    return L;
}


template class AvlNode<int>;
template class AvlNode<short>;
template class AvlNode<long>;
template class AvlNode<long long>;
template class AvlNode<string>;

template <class Comparable>
AVLTree<Comparable>::AVLTree(){
    root = nullptr;
    _size = 0;
}

template <class Comparable>
AVLTree<Comparable>::~AVLTree(){
    clear();
}

template <class Comparable>
void AVLTree<Comparable>::clear(){
    vector<AvlNode<Comparable>*> stack;
    
    if (root != nullptr)
        stack.push_back(root);
    
    while (!stack.empty()){
        AvlNode<Comparable> *node = stack.back();
        stack.pop_back();
        
        if (node->left != nullptr)
            stack.push_back(node->left);
        
        if (node->right != nullptr)
            stack.push_back(node->right);
        
        _size--;
        delete node;
    }
    
    root = nullptr;
}

template <class Comparable>
void AVLTree<Comparable>::insert(Comparable value){
    AvlNode<Comparable> **indirect = &root;  // to generalize insertion
    vector<AvlNode<Comparable>**> path;  // to update height values
    
    while (*indirect != nullptr){
        path.push_back(indirect);
        
        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    *indirect = new AvlNode<Comparable>(value);
    (*indirect)->parent = path.empty() ? nullptr : *path.back();
    path.push_back(indirect);
    
    balance(path);
    _size++;
}

template <class Comparable>
void AVLTree<Comparable>::remove(Comparable value){
    AvlNode<Comparable> **indirect = &root;  // to generalize insertion
    vector<AvlNode<Comparable>**> path;  // to update height values
    
    while (*indirect != nullptr and (*indirect)->value != value){
        path.push_back(indirect);
        
        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    if (*indirect == nullptr)  // the value does not exist in tree
        return;  // may be raising an Exception is more elegant
    
    else
        path.push_back(indirect);
    
    size_t index = path.size();
    
    if ((*indirect)->left == nullptr and (*indirect)->right == nullptr){  // the node is leaf
        delete *indirect;  // just delete node
        *indirect = nullptr;
        path.pop_back();  // pop the deleted node from path
    }
    
    else if ((*indirect)->right == nullptr){  // only left child exists
        AvlNode<Comparable> *toRemove = *indirect;
        
        (*indirect) = (*indirect)->left;
        delete toRemove;
        
        path.pop_back();
    }
    
    else{  // right child exists
        AvlNode<Comparable> **successor = &((*indirect)->right);
        
        while ((*successor)->left != nullptr){
            path.push_back(successor);
            successor = &((*successor)->left);
        }
        
        if (*successor == (*indirect)->right){
            (*successor)->left = (*indirect)->left;
            
            AvlNode<Comparable> *toRemove = *indirect;
            *indirect = *successor;
            delete toRemove;
        }
        
        else{
            AvlNode<Comparable> *tmp = *path.back(), *suc = *successor;
            
            tmp->left = (*successor)->right;
            suc->left = (*indirect)->left;
            suc->right = (*indirect)->right;
            
            delete *indirect;
            *indirect = suc;
            path[index] = &(suc->right);
        }
    }
    
    balance(path);
    _size--;
}


template <class Comparable>
void AVLTree<Comparable>::balance(vector<AvlNode<Comparable> **> path){  // starting from root
    reverse(path.begin(), path.end());
    
    for (auto indirect: path){
        (*indirect)->updateValues();
        
        if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 0)   // left - left
            *indirect = (*indirect)->right_rotate();
        
        else if ((*indirect)->balanceFactor() >= 2){  // left - right
            (*indirect)->left = (*indirect)->left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }
        
        else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= 0)  // right - right
            *indirect = (*indirect)->left_rotate();
        
        else if ((*indirect)->balanceFactor() <= -2){  // right - left
            (*indirect)->right = ((*indirect)->right)->right_rotate();
            *indirect = (*indirect)->left_rotate();
        }
    }
}

template <class Comparable>
bool AVLTree<Comparable>::empty() const{
    return _size == 0;
}

template <class Comparable>
int AVLTree<Comparable>::size() const{
    return _size;
}

template <class Comparable>
int AVLTree<Comparable>::find(Comparable value) const{
    AvlNode<Comparable> *direct = root;
    int idx = 0;
    
    while (direct != nullptr and direct->value != value){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    if (direct == nullptr)
        return -1;
    
    else
        return idx + (direct->left ? direct->left->count : 0);
}

template <class Comparable>
int AVLTree<Comparable>::upper_bound(Comparable value) const{
    AvlNode<Comparable> *direct = root;
    int idx = 0;
    
    while (direct != nullptr){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    return idx;
}

template <class Comparable>
int AVLTree<Comparable>::lower_bound(Comparable value) const{
    AvlNode<Comparable> *direct = root;
    int idx = 0;
    
    while (direct != nullptr){
        if (direct->value >= value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    return idx;
}

template <class Comparable>
const Comparable& AVLTree<Comparable>::find_min() const{
    AvlNode<Comparable> *cur = root;
    
    while (cur->left != nullptr)
        cur = cur->left;
    
    return cur->value;
}

template <class Comparable>
const Comparable& AVLTree<Comparable>::find_max() const{
    AvlNode<Comparable> *cur = root;
    
    while (cur->right != nullptr)
        cur = cur->right;
    
    return cur->value;
}

template <class Comparable>
const Comparable& AVLTree<Comparable>::operator[](size_t idx) const{
    AvlNode<Comparable> *cur = root;
    int left = cur->left != nullptr ? cur->left->count : 0;
    
    while (left != idx){
        if (left < idx){
            idx -= left + 1;
            
            cur = cur->right;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
        
        else{
            cur = cur->left;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
    }
    
    return cur->value;
}

template <class Comparable>
std::vector<Comparable> AVLTree<Comparable>::inorderSort() const {
    std::vector<Comparable> sortedList;
    inorderSort(root, sortedList);
    return sortedList;
}

template <class Comparable>
void AVLTree<Comparable>::inorderSort(AvlNode<Comparable>* node,std::vector<Comparable>& sortedList) const {
    if (node != nullptr) {
        inorderSort(node->left, sortedList);  // Visit left subtree
        sortedList.push_back(node->value);  // Add element to sorted list
        inorderSort(node->right, sortedList); // Visit right subtree
    }
}

template <class Comparable>
std::vector<Comparable> AVLTree<Comparable>::preorderSort() const {
    std::vector<Comparable> sortedList;
    inorderSort(root, sortedList);
    return sortedList;
}

template <class Comparable>
void AVLTree<Comparable>::preorderSort(AvlNode<Comparable>* node,std::vector<Comparable>& sortedList) const {
    if (node != nullptr) {
        sortedList.push_back(node->element);  // Add element to sorted list
        preorderSort(node->left, sortedList);  // Visit left subtree
        preorderSort(node->right, sortedList); // Visit right subtree
    }
}
template <class Comparable>
int AVLTree<Comparable>::heightLeftSubtree(const typename AVLTree<Comparable>::Node* node) const {
    if (node == nullptr) {
        return -1; // height of an empty tree is -1
    }
    return 1 + std::max(heightLeftSubtree(node->left), heightLeftSubtree(node->right));
}

template <class Comparable>
int AVLTree<Comparable>::heightRightSubtree(const typename AVLTree<Comparable>::Node* node) const {
    if (node == nullptr) {
        return -1; // height of an empty tree is -1
    }
    return 1 + std::max(heightRightSubtree(node->left), heightRightSubtree(node->right));
}

template <class Comparable>
bool AVLTree<Comparable>::checkBalance() const {
    return checkBalanceHelper(root);
}

template <class Comparable>
bool AVLTree<Comparable>::checkBalanceHelper(const typename AVLTree<Comparable>::Node* node) const {
    if (node == nullptr) {
        return true; // an empty tree is balanced
    }
    int height_left = heightLeftSubtree(node->left);
    int height_right = heightRightSubtree(node->right);
    int balance_factor = height_left - height_right;
    if (abs(balance_factor) >= 1) {
        return false; // the tree is unbalanced
    }
    return checkBalanceHelper(node->left) && checkBalanceHelper(node->right);
}

template <class Comparable>
void AVLTree<Comparable>::printBalanceInfo() const {
    bool balanced = checkBalance();
    cout << "The tree is " << (balanced ? "balanced" : "not balanced") << endl;
    
    int height_left = heightLeftSubtree(root->left);
    int height_right = heightRightSubtree(root->right);
    cout << "The heights of AVL are for left: " << height_left + 1 << " and right: " << height_right + 1 << endl;
    
}

template <class Comparable>
void AVLTree<Comparable>::drawTreeToFileHelper(std::ofstream& file, const typename AVLTree<Comparable>::Node* node, int level, bool is_right) const {
    if (node != nullptr) {
        if (level == 0) {
            file << "|__";
        }
        else {
            file << string(level - 1, ' ');
            if (is_right) {
                file << "|__";
            }
            else {
                file << "|--";
            }
        }
        file << node->value;
        
        // Recursively draw left and right subtrees
        if (node->left != nullptr || node->right != nullptr) {
            drawTreeToFileHelper(file, node->left, level + 3,false);
            drawTreeToFileHelper(file, node->right, level + 3,true);
        }
    }
}


template <class Comparable>
void AVLTree<Comparable>::drawTreeToFile(std::string filename) const{
    std::ofstream file(filename);
    if (file.is_open()) {
        drawTreeToFileHelper(file, root, 0, false);
        file.close();
    }
}
