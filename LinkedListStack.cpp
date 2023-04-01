#include "LinkedListStack.h"
#include <iostream>
#include <string>

using namespace std;

template <class Object>
LinkedListStack<Object>::LinkedListStack( )
{
    topOfStack = NULL;
}

template <class Object>
bool LinkedListStack<Object>::isEmpty( ) const
{
    return topOfStack == NULL;
}

template <class Object>
const Object & LinkedListStack<Object>::top( ) const
{
    return topOfStack->element;
}

template <class Object>
void LinkedListStack<Object>::pop( )
{
    ListNode *oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;
}

template <class Object>
void LinkedListStack<Object>::push( const Object & x )
{
    topOfStack = new ListNode( x, topOfStack );
}

template <class Object>
Object LinkedListStack<Object>::topAndPop( )
{
    Object topItem = top( );
    pop( );
    return topItem;
}

template <class Object>
void LinkedListStack<Object>::makeEmpty( )
{
    while ( !isEmpty() )
        pop();
}

template <class Object>
const LinkedListStack<Object> & LinkedListStack<Object>::
operator=( const LinkedListStack<Object> & rhs )
{
    if ( this != &rhs )
    {
        makeEmpty( );
        if ( rhs.isEmpty( ) )
            return *this;
        
        ListNode *rptr = rhs.topOfStack;
        ListNode *ptr  = new ListNode( rptr->element );
        topOfStack = ptr;
        
        for ( rptr = rptr->next; rptr != NULL; rptr = rptr->next )
            ptr = ptr->next = new ListNode( rptr->element );
    }
    return *this;
}

template <class Object>
LinkedListStack<Object>::LinkedListStack( const LinkedListStack<Object> & rhs )
{
    topOfStack = NULL;
    *this = rhs; // deep copy
}

template <class Object>
LinkedListStack<Object>::~LinkedListStack( )
{
    makeEmpty( );
}

