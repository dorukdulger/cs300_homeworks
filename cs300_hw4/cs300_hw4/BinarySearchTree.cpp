#include "BinarySearchTree.h"
#include "Contact.h"
#include <memory>

using namespace std;

/**
 * Construct the tree.
 */
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound ) : ITEM_NOT_FOUND( notFound ), root( nullptr )
{}

/**
 * Internal method to get element field in node t.
 * Return the element field or ITEM_NOT_FOUND if t is nullptr.
 */
template <class Comparable>
Comparable & BinarySearchTree<Comparable>::elementAt( BinaryNode<Comparable> *t ){
    return t;
}

template <class Comparable>
BinaryNode<Comparable> * & BinarySearchTree<Comparable>::BSTrootgiver(){
    return root;
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class Comparable>
Comparable & BinarySearchTree<Comparable>::find( Comparable & x ){
    return elementAt( find( x, root ) );
}


template <class Comparable>
Comparable & BinarySearchTree<Comparable>::find( Comparable & x, BinaryNode<Comparable> *t ) const{
    while( t != nullptr ){
        if(  t->element > x ){
            t = t->left;
        }else if( x > t->element ){
            t = t->right;
        }else{
            return t;    // Match
        }
    }
    return nullptr;   // No match
}

/**
 * Find the smallest item in the tree.
 * Return smallest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin( ) const{
    return elementAt( findMin( root ) );
}

/**
 * Internal method to find the smallest item in a subtree t.
 * Return node containing the smallest item.
 */
template <class Comparable>BinaryNode<Comparable> *BinarySearchTree<Comparable>::findMin( BinaryNode<Comparable> *t ) const{
    if( t == nullptr ){
        return nullptr;
    }
    if( t->left == nullptr ){
        return t;
    }
    
    return findMin( t->left );
}

/**
 * Find the smallest item in the tree.
 * Return smallest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMax( ) const{
    return elementAt( findMax( root ) );
}

/**
 * Internal method to find the largest item in a subtree t.
 * Return node containing the largest item.
 */
template <class Comparable>BinaryNode<Comparable> *BinarySearchTree<Comparable>::findMax( BinaryNode<Comparable> *t ) const{
    if( t != nullptr ){
        while( t->right != nullptr ){
            t = t->right;
        }
    }
    return t;
}

/**
 * Insert x into the tree; duplicates are ignored.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::insert( Comparable & x ){
    insert( x, root );
}

/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the tree.
 * Set the new root.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::insert( Comparable & x, BinaryNode<Comparable> * & t ) const{
    if( t == nullptr ){ //  create a new node at the right place
        t = new BinaryNode<Comparable>( x, nullptr, nullptr );
    }else if( t->element > x){
        insert( x, t->left );  // insert at the left or
    }else if( x > t->element ){
        insert( x, t->right );  // right subtree
    }else{
        ;  // Duplicate; do nothing
    }
}

/**
 * Remove x from the tree. Nothing is done if x is not found.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::remove (Comparable & x){
    remove( x, root );
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the tree.
 * Set the new root.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::remove( Comparable & x, BinaryNode<Comparable> * & t ) const{
    if( t == nullptr ){
        return;   // Item not found; do nothing
    }
    if( x < t->element ){
        remove( x, t->left );
    }else if( t->element < x ){
        remove( x, t->right );
    }else if( t->left != nullptr && t->right != nullptr ){ // Two children
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
    }
    else{ // one or no children
        BinaryNode<Comparable> *oldNode = t;
        t = ( t->left != nullptr ) ? t->left : t->right;
        delete oldNode;
    }
}            

/**
 * Make the tree logically empty.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( ){
    makeEmpty( root );
}

/**
 * Internal method to make subtree empty.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryNode<Comparable> * & t ) const{
    if( t != nullptr )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullptr;
}

template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const{
    return root == nullptr;
}

/**
 * Print the tree contents in sorted order.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::printTree( ) const{
    if( isEmpty( ) ){
        std::cout << "";
    }else{
        printTree( root );
    }
}

/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::printTree( BinaryNode<Comparable> * t ) const{
    if ( t != nullptr ){
        printTree( t->left );
        std::cout << t->element << std::endl;
        printTree( t->right );
    }
}

/**
 * Internal method to clone subtree.
 */
template <class Comparable>BinaryNode<Comparable> *BinarySearchTree<Comparable>::clone( BinaryNode<Comparable> * t ) const{
    if ( t == nullptr ){
        return nullptr;
    }else{
        return new BinaryNode<Comparable> ( t->element, clone( t->left ), clone( t->right ) );
    }
}

/**
 * Destructor for the tree.
 */
template <class Comparable>BinarySearchTree<Comparable>::~BinarySearchTree( ){
    makeEmpty();
}

/**
 * Copy constructor.
 */
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree( const BinarySearchTree<Comparable> & rhs ) :root( nullptr ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ){
    *this = rhs;
}

template <class Comparable>
int BinarySearchTree<Comparable>::heightLeftSubtree(const typename BinarySearchTree<Comparable>::Node* node) const {
    if (node == nullptr) {
        return -1; // height of an empty tree is -1
    }
    return 1 + std::max(heightLeftSubtree(node->left), heightLeftSubtree(node->right));
}

template <class Comparable>
int BinarySearchTree<Comparable>::heightRightSubtree(const typename BinarySearchTree<Comparable>::Node* node) const {
    if (node == nullptr) {
        return -1; // height of an empty tree is -1
    }
    return 1 + std::max(heightRightSubtree(node->left), heightRightSubtree(node->right));
}

template <class Comparable>
bool BinarySearchTree<Comparable>::checkBalance() const {
    return checkBalanceHelper(root);
}

template <class Comparable>
bool BinarySearchTree<Comparable>::checkBalanceHelper(const typename BinarySearchTree<Comparable>::Node* node) const {
    if (node == nullptr) {
        return true; // an empty tree is balanced
    }
    int height_left = heightLeftSubtree(node->left);
    int height_right = heightRightSubtree(node->right);
    int balance_factor = height_left - height_right;
    if (std::abs(balance_factor) > 1) {
        return false; // the tree is unbalanced
    }
    return checkBalanceHelper(node->left) && checkBalanceHelper(node->right);
}

template <class Comparable>
void BinarySearchTree<Comparable>::printBalanceInfo() const {
    bool balanced = checkBalance();
    std::cout << "The tree is " << (balanced ? "balanced" : "not balanced") << std::endl;
    
    int height_left = heightLeftSubtree(root->left);
    int height_right = heightRightSubtree(root->right);
    std::cout << "The heights of BST are for left: " << height_left + 1<< " and right: " << height_right + 1<< std::endl;
    
}


template <class Comparable>
std::vector<Comparable> BinarySearchTree<Comparable>::inorderSort() const {
    std::vector<Comparable> sortedList;
    inorderSort(root, sortedList);
    return sortedList;
}

template <class Comparable>
void BinarySearchTree<Comparable>::inorderSort(BinaryNode<Comparable>* node,std::vector<Comparable>& sortedList) const {
    if (node != nullptr) {
        inorderSort(node->left, sortedList);  // Visit left subtree
        sortedList.push_back(node->element);  // Add element to sorted list
        inorderSort(node->right, sortedList); // Visit right subtree
    }
}

template <class Comparable>
std::vector<Comparable> BinarySearchTree<Comparable>::preorderSort() const {
    std::vector<Comparable> sortedList;
    preorderSort(root, sortedList);
    return sortedList;
}

template <class Comparable>
void BinarySearchTree<Comparable>::preorderSort(BinaryNode<Comparable>* node, std::vector<Comparable>& sortedList) const {
    if (node != nullptr) {
        sortedList.push_back(node->element);  // Add element to sorted list
        preorderSort(node->left, sortedList);  // Visit left subtree
        preorderSort(node->right, sortedList); // Visit right subtree
    }
}



template <class Comparable>
void BinarySearchTree<Comparable>::drawTreeToFileHelper(std::ofstream& file, const typename BinarySearchTree<Comparable>::Node* node, int level, bool is_right) const {
    if (node != nullptr) {
        
//        for(int i=0; i < level*3; i++){
//            if(i % 3 == 0) cout << "|" ;
//            cout << " ";
//        }
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
        file << node->element;
        
        // Recursively draw left and right subtrees
        if (node->left != nullptr || node->right != nullptr) {
            drawTreeToFileHelper(file, node->left, level + 3,false);
            drawTreeToFileHelper(file, node->right, level + 3,true);
        }
    }
}

template <class Comparable>
void BinarySearchTree<Comparable>::drawTreeToFile(std::string filename) const{
    std::ofstream file(filename);
    if (file.is_open()) {
        drawTreeToFileHelper(file, root, 0, false);
        file.close();
    }
}
