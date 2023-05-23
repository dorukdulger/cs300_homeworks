//
//  hashtable.h
//  cs300_hw4
//
//  Created by Doruk Dulger on 16.05.2023.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <vector>
#include <string>
#include <iostream>

using namespace std;

template <typename Key, typename Value>
class HashTable {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;
        Node(Key key, Value value) : key(key), value(value), next(nullptr) {}
    };
    vector<Node*> table;
    int tablesize;
    double loadFactor;
    int hash(Key key) const;
    
public:
    HashTable(int capacity = 53, double loadFactor = 0.85) : tablesize(0), loadFactor(loadFactor) {
        table.resize(capacity);
    }
    ~HashTable() {
        for (auto& head : table) {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
    int getSize() const {
        return tablesize;
    }
    
    double getLoadRatio() const {
        return static_cast<double>(tablesize) / table.size();
    }
    void insert(const Key& key,const Value& value);
    bool find(Key key, Value& value) const;
    void rehash();
    void remove(const Key& key);
};

template <typename Key, typename Value>
int HashTable<Key, Value>::hash(Key key) const {
    return key.hash() % table.size();
}


template <typename Key, typename Value>
void HashTable<Key, Value>::rehash() {
    int oldCapacity = table.size();
    vector<Node*> oldTable = table; // create a copy of the old table
    table.clear(); // clear the current table
    table.resize(oldCapacity * 2); // resize the table to the new capacity

    for (Node* head : oldTable) {
        Node* curr = head; // temporary pointer to traverse the linked list
        while (curr != nullptr) {
            Node* temp = curr;
            curr = curr->next;
            int index = hash(temp->key);
            temp->next = table[index];
            table[index] = temp;
        }
    }
    
    cout << "rehashed...\nprevious table size: " << oldCapacity
         << ", new table size: " << table.size() << ", current unique item count " << tablesize << ",\ncurrent load factor: " << static_cast<double>(tablesize) / table.size() << endl;
}


template <typename Key, typename Value>
void HashTable<Key, Value>::insert(const Key& key, const Value& value) {
    int index = hash(key);
    Node* newNode = new Node(key, value); // create a new node with the key-value pair

    if (table[index] == nullptr) {
        table[index] = newNode; // set the new node as the head of the linked list at the index
    } else {
        Node* head = table[index];
        Node* prev = nullptr; // pointer to the previous node in the linked list

        while (head != nullptr) {
            if (head->key == key) {
                head->value = value;
                delete newNode; // key already exists, so delete the newly created node
                return;
            }
            prev = head;
            head = head->next;
        }

        prev->next = newNode; // update the next pointer of the previous node to the new node
    }

    ++tablesize; // increase the size of the hash table

    if (static_cast<double>(tablesize) / table.size() > loadFactor) {
        rehash(); // if the load factor is exceeded, rehash the hash table
    }
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::find(Key key, Value& value) const {
    int index = hash(key);
    Node* head = table[index];
    while (head != nullptr) {
        if (head->key == key) {
            value = head->value;
            return true;
        }
        head = head->next;
    }
    return false;
}

template <typename Key, typename Value>
void HashTable<Key, Value>::remove(const Key& key) {
    int index = hash(key);
    Node* head = table[index];
    Node* prev = nullptr; // pointer to the previous node in the linked list

    while (head != nullptr) {
        if (head->key == key) {
            if (prev != nullptr) {
                prev->next = head->next; // update the next pointer of the previous node
            } else {
                table[index] = head->next; // update the head pointer of the linked list
            }

            delete head; // delete the node

            --tablesize; // decrease the size of the hash table
            return;
        }

        prev = head;
        head = head->next;
    }
}


#endif // HASHTABLE_H
