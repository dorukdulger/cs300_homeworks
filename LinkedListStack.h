#ifndef LinkedListStack_h
#define LinkedListStack_h

#include <iostream>

using namespace std;

template <class Object>
    class LinkedListStack
        {
        public:
            LinkedListStack( );
            LinkedListStack( const LinkedListStack & rhs );
            ~LinkedListStack( );
            
            bool isEmpty( ) const;
            void makeEmpty( );
            
            void pop( );
            void push( const Object & x );
            Object topAndPop( );
            const Object & top( ) const;
            const LinkedListStack & operator=( const LinkedListStack & rhs );
        private:
                struct ListNode
                {
                      Object   element;
                      ListNode *next;

                      ListNode( const Object & theElement, ListNode * n = NULL )
                          : element( theElement ), next( n ) { }
                };

                ListNode *topOfStack; 
          };

#include "LinkedListStack.cpp"

#endif /* LinkedListStack_h */
