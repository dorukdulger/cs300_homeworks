//
//  cs300_hw4
//
//  Created by Doruk Dulger
//

#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include "BinarySearchTree.h"
#include "Contact.h"
#include "hashtable.h"
#include <cctype>
#include <algorithm>

using namespace std;

template <class Comparable>
void loadContacts_toBST(string filename, Comparable& phonebook) {
    ifstream file(filename);
    if (file.is_open()) {
        string firstName, lastName, phoneNumber, city;
        while (file >> firstName >> lastName >> phoneNumber >> city) {
            Contact contact(firstName, lastName, phoneNumber, city);
            phonebook.insert(contact);
        }
        file.close();
    }
}

template <class Comparable>
void loadContacts_toHash(string filename, Comparable& phonebook) {
    ifstream file(filename);
    if (file.is_open()) {
        string firstName, lastName, phoneNumber, city;
        while (file >> firstName >> lastName >> phoneNumber >> city) {
            Contact contact(firstName, lastName, phoneNumber, city);
            phonebook.insert(contact,contact);
        }
        file.close();
    }
}

template <class Comparable>
void searchContact(string search, const Comparable& book, bool tester) {
    string firstname,lastname;
    size_t pos = search.find(' ');
    bool checker = false;
    
    if (pos != string::npos) {
        firstname = search.substr(0, pos);
        lastname = search.substr(pos + 1);
        search = firstname + " " + lastname;
    } else {
        firstname = search;
    }
    auto contacts = book.inorderSort();
    for (const auto& contact : contacts) {
        if ((contact.firstName == firstname && contact.lastName == lastname) || contact.firstName.find(search) == 0) {
            if(!tester){
                cout << contact.firstName << " " << contact.lastName << " "<< contact.phoneNumber << " " << contact.city << endl;
                checker = true;
            }
        }
    }
    if(!checker && !tester){
        cout << "Name not found!" << endl;
    }
}

template <typename Key, typename Value>
void searchContactfromHash(const string& search, const HashTable<Key, Value>& book, bool tester) {
    string firstname, lastname;
    size_t pos = search.find(' ');

    if (pos != string::npos) {
        firstname = search.substr(0, pos);
        lastname = search.substr(pos + 1);
    } else {
        firstname = search;
    }

    Value contact(firstname, lastname, "", "");
    if (book.find(contact, contact)) {
        if(!tester){
            cout << contact.firstName << " " << contact.lastName << " "<< contact.phoneNumber << " " << contact.city << endl;
        }
    }else if(!tester){
        cout << "Name not found!" << endl;
    }
}

template <class Comparable>
bool CheckContact(string search, const Comparable& book) {
    string firstname,lastname;
    size_t pos = search.find(' ');
    
    if (pos != string::npos) {
        firstname = search.substr(0, pos);
        lastname = search.substr(pos + 1);
        search = firstname + " " + lastname;
    } else {
        firstname = search;
    }
    auto contacts = book.inorderSort();
    for (const auto& contact : contacts) {
        if ((contact.firstName == firstname && contact.lastName == lastname) || contact.firstName.find(search) == 0) {
            return true;
        }
    }
    return false;
}


template <class Comparable>
void addContact(string firstName, string lastName, string phoneNumber, string city, Comparable& phonebook) {
    Contact contact(firstName, lastName, phoneNumber, city);
    phonebook.insert(contact);
}

template <class Comparable>
bool deleteContact(string name, string lastname, Comparable& book) {
    Contact dummy(name, lastname, "", "");
    bool check = CheckContact(name + " " + lastname, book);
    if(check){
        book.remove(dummy);
        return check;
    }
    else{
        return check;
    }
}

template <typename Key, typename Value>
void addContact_toHash(string firstName, string lastName, string phoneNumber, string city, HashTable<Key, Value>& phonebook) {
    Contact contact(firstName, lastName, phoneNumber, city);
    phonebook.insert(contact,contact);
}


template <typename Key, typename Value>
bool CheckContactfromHash(const string& search, const HashTable<Key, Value>& phonebook) {
    string firstname, lastname;
    size_t pos = search.find(' ');

    if (pos != string::npos) {
        firstname = search.substr(0, pos);
        lastname = search.substr(pos + 1);
    } else {
        firstname = search;
    }

    Value contact(firstname, lastname, "", "");
    if (phonebook.find(contact, contact)) {
        return true;
    }
    return false;
}


template <typename Key, typename Value>
bool deleteContactfromHash(const string& name, const string& lastname, HashTable<Key, Value>& phonebook) {
    string search = name + " " + lastname;
    Key contact(name, lastname, "", "");
    if (CheckContactfromHash(search, phonebook)) {
        phonebook.remove(contact);
        return true;
    }
    return false;
}


int main(){
    string filename;
    ifstream inputfile;
    BinarySearchTree<Contact> BSTPhonebook;
    HashTable<Contact, Contact> HashTablePhonebook;
    
    cout << "Enter the file name: ";
    cin >> filename;
    
    inputfile.open(filename.c_str());
    if (inputfile.fail()){
        cout << "File cannot be opened" << endl;
    }else{
        
        loadContacts_toBST(filename, BSTPhonebook);
        loadContacts_toHash(filename, HashTablePhonebook);
        cout << "After preprocessing, the contact count is " << HashTablePhonebook.getSize()
        << ". Current load ratio is " << HashTablePhonebook.getLoadRatio() << endl;

        while (true) {
            cout << endl;
            cout << "Choose which action to perform from 1 to 4:\n";
            cout << "1 - Search a phonebook contact\n";
            cout << "2 - Adding a phonebook contact\n";
            cout << "3 - Deleting a phonebook contact\n";
            cout << "4 - Press 4 to exit\n";
            cout << "Enter choice: ";
            
            int choice;
            cin >> choice;
            cout << endl;
            
            if (choice == 1) {
                // Search contact
                string firstname,lastname,search;
                cout << "Enter name to search for: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                getline(cin, search);
                
                size_t pos = search.find(' ');
                
                if (pos != string::npos) {
                    firstname = search.substr(0, pos);
                    lastname = search.substr(pos + 1);
                    search = firstname + " " + lastname;
                } else {
                    firstname = search;
                }
                
                transform(search.begin(), search.end(), search.begin(),[](unsigned char c){ return std::toupper(c); });
                cout << endl;
                cout << "Searching an item in the phonebook (BST) . . ." << endl;
                cout << "Phonebook: Searching for: (" << search << ")" << endl;
                cout << "====================================\n";
                
                int k = 500;
                auto start = std::chrono::high_resolution_clock::now();

                // Perform search with BST
                for (int i = 0; i < k-1; i++) {
                    searchContact(search,BSTPhonebook,true);
                }
                searchContact(search,BSTPhonebook,false);
                
                auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
                (std::chrono::high_resolution_clock::now() - start);
                double BSTAverageTime = static_cast<double>(BSTTime.count()) / k;
                cout << "\nBST Search Time: " << BSTAverageTime << "\n";
                
                cout << endl;
                cout << "Searching for an item in the phonebook (HashTable) . . ." << endl;
                cout << "Phonebook: Searching for: (" << search << ")" << endl;
                cout << "====================================\n";
                
                start = std::chrono::high_resolution_clock::now();

                // Perform search with Hash Table
                for (int i = 0; i < k-1; i++) {
                    searchContactfromHash(search,HashTablePhonebook,true);
                }
                searchContactfromHash(search,HashTablePhonebook,false);
                
                
                auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
                (std::chrono::high_resolution_clock::now() - start);
                double HTAverageTime = static_cast<double>(HTTime.count()) / k;
                cout << "\nHash Table Search Time: " << HTAverageTime << "\n";
                double speedupRatio = BSTAverageTime / HTAverageTime;
                cout << "Speed up: " << speedupRatio << endl;
                
                
            } else if (choice == 2) {
                // Add contact
                string firstName, lastName, phoneNumber, city;
                cout << "Enter the information of the contact to be added:" << endl;
                cout << "Name: ";
                cin >> firstName >> lastName;
                transform(firstName.begin(), firstName.end(), firstName.begin(),[](unsigned char c){ return std::toupper(c); });
                transform(lastName.begin(), lastName.end(), lastName.begin(),[](unsigned char c){ return std::toupper(c); });
                cout << endl;
                cout << "Tel: " ;
                cin >> phoneNumber;
                cout << endl;
                cout << "City: ";
                cin >> city;
                cout << endl;
                cout << "Adding an item to the phonebook BST . . ."  << endl;
                
                auto startB = chrono::high_resolution_clock::now();
                addContact(firstName, lastName, phoneNumber, city, BSTPhonebook);
                auto endB = chrono::high_resolution_clock::now();
                cout << "===================================="  << endl;
                cout << "Contact has been added successfully to the BST\n" << endl;
                cout << endl;
                
                cout << "Adding an item to the phonebook (Hashtable) . . ."  << endl;
                auto startA = chrono::high_resolution_clock::now();
                addContact_toHash(firstName, lastName, phoneNumber, city, HashTablePhonebook);
                auto endA = chrono::high_resolution_clock::now();
                cout << "===================================="  << endl;
                cout << "Contact has been added successfully to the Hashtable\n" << endl;
               
                auto durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                
                cout << "Adding a contact to the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                cout << "Adding a contact to the Hashtable took " << durationA << " nanoseconds. . ." << endl;
                
            } else if (choice == 3) {
                // Delete contact
                cout << "Deleting an item from the phonebook . . ." << endl;
                string firstname,lastname;
                cout << "Enter the fullname of the contact to be deleted: " ;
                cin >> firstname >> lastname;
                transform(firstname.begin(), firstname.end(), firstname.begin(),[](unsigned char c){ return std::toupper(c); });
                transform(lastname.begin(), lastname.end(), lastname.begin(),[](unsigned char c){ return std::toupper(c); });
                cout << endl;
                auto startB = chrono::high_resolution_clock::now(); // start time
                bool checkA = deleteContact(firstname, lastname, BSTPhonebook);
                auto endB = chrono::high_resolution_clock::now(); // end time
                auto durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                
                auto startA = chrono::high_resolution_clock::now(); // start time
                bool checkB = deleteContactfromHash(firstname, lastname, HashTablePhonebook);
                auto endA = chrono::high_resolution_clock::now(); // end time
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
        
                if(checkA && checkB){
                    cout << "Deleted successfully" << endl;
                    cout << endl;
                    cout << "Deletion from the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                    cout << "Deletion from the Hashtable took " << durationA << " nanoseconds. . ." << endl;
                }
                else{
                    cout << "Contact does not exist in the tree" << endl;
                }
                
            } else if (choice == 4) {
                // Quit
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
   
    BSTPhonebook.makeEmpty();
    
    inputfile.clear();
    inputfile.seekg(0);
    inputfile.close();
    
    return 0;
}

