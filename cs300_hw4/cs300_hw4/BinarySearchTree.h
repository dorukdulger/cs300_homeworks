#ifndef _BINARYSEARCHTREE_H_
#define _BINARYSEARCHTREE_H_

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode
{
public:
    Comparable element;
    BinaryNode  *left;
    BinaryNode  *right;
    
    BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
    : element( theElement ), left( lt ), right( rt ) { }
private:         
    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree
{
public:
    explicit BinarySearchTree( )
    : root{ nullptr }, ITEM_NOT_FOUND{"","","",""} { }
    explicit BinarySearchTree( const Comparable & notFound );
    BinarySearchTree( const BinarySearchTree & rhs );
    ~BinarySearchTree( );
    
    const Comparable & findMin( ) const;
    const Comparable & findMax( ) const;
    Comparable & find( Comparable & x );
    bool isEmpty( ) const;
    void printTree( ) const;
    
    void makeEmpty( );
    void insert( Comparable & x );
    
    void remove( Comparable & x );
    void insert( Comparable & x, BinaryNode<Comparable> * & t ) const;
    
    void printTree( BinaryNode<Comparable> *t ) const;
    
    BinaryNode<Comparable> * & BSTrootgiver();
    using Node = BinaryNode<Comparable>;
    const BinarySearchTree & operator=( const BinarySearchTree & rhs );
    void drawTreeToFile(std::string filename) const;
    std::vector<Comparable> inorderSort() const;
    void inorderSort(BinaryNode<Comparable>* node,std::vector<Comparable>& sortedList) const;
    std::vector<Comparable> preorderSort() const;
    void preorderSort(BinaryNode<Comparable>* node,std::vector<Comparable>& sortedList) const;
    int getHeight(BinaryNode<Comparable>* node) const;
    void printBalanceInfo() const;
private:
    BinaryNode<Comparable> *root;
    const Comparable ITEM_NOT_FOUND;
    friend class BinaryNode<Comparable>;
    Comparable & elementAt( BinaryNode<Comparable> *t );
    void drawTreeToFileHelper(std::ofstream& file, const Node* node, int level, bool is_right) const;
    void remove( Comparable & x, BinaryNode<Comparable> * & t ) const;
    BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t ) const;
    BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t ) const;
    Comparable & find( Comparable & x, BinaryNode<Comparable> *t ) const;
    void makeEmpty( BinaryNode<Comparable> * & t ) const;
    void draw();
    int heightLeftSubtree(const typename BinarySearchTree<Comparable>::Node* node) const;
    int heightRightSubtree(const typename BinarySearchTree<Comparable>::Node* node) const;
    bool checkBalance() const;
    bool checkBalanceHelper(const typename BinarySearchTree<Comparable>::Node* node) const;
    
    BinaryNode<Comparable> * clone(BinaryNode<Comparable> *t ) const;
};

template<class Comparable>
const BinarySearchTree<Comparable>& BinarySearchTree<Comparable>::operator=(const BinarySearchTree<Comparable>& rhs) {
    if (this != &rhs) {
        makeEmpty();
        root = clone(rhs.root);
    }
    return *this;
}

#include "BinarySearchTree.cpp"

#endif
