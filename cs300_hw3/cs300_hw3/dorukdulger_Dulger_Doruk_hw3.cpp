//
//  cs300_hw3
//
//  Created by Doruk Dulger
//

#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include "BinarySearchTree.h"
#include "AvlTree.h"
#include "Contact.h"
#include <cctype>
#include <algorithm>

using namespace std;

template <class Comparable>
void loadContactsFromFile(string filename, Comparable& phonebook) {
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
void searchContact(string search, const Comparable& book) {
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
            cout << contact.firstName << " " << contact.lastName << " "<< contact.phoneNumber << " " << contact.city << endl;
        }
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

template <class Comparable>
void InOrderPrintToFile(string filename, Comparable& phonebook){
    ofstream file(filename);
    if (file.is_open()) {
        auto bstResult = phonebook.inorderSort();
        for (const auto& r : bstResult) {
            file << r.firstName << " " << r.lastName << " " <<r.phoneNumber << " " << r.city << endl;
        }
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
    file.close();
}

template <class Comparable>
void PreOrderPrintToFile(string filename, Comparable& phonebook) {
    ofstream file(filename);
    if (file.is_open()) {
        auto bstResult = phonebook.preorderSort();
        for (const auto& r : bstResult) {
            file << r.firstName << " " << r.lastName << " " <<r.phoneNumber << " " << r.city << endl;
        }
    }
    else {
        cout << "Unable to open file " << filename << endl;
    }
    file.close();
}

int main(){
    string filename;
    ifstream inputfile;
    BinarySearchTree<Contact> BSTPhonebook;
    AVLTree<Contact> AVLPhonebook;
    cout << "Please enter the contact file name: ";
    cin >> filename;
    
    
    inputfile.open(filename.c_str());
    if (inputfile.fail()){
        cout << "File cannot be opened" << endl;
    }else{
        //BST
        cout << "Loading the phonebook into a BST.\n";
        auto start = chrono::high_resolution_clock::now();
        loadContactsFromFile(filename, BSTPhonebook);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Phonebook creation in BST took " << duration << " milliseconds.\n";
        
        BSTPhonebook.printBalanceInfo();
        
        //AVL
        cout << "Loading the phonebook into a AVL.\n";
        start = chrono::high_resolution_clock::now();
        loadContactsFromFile(filename, AVLPhonebook);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Phonebook creation in AVL took " << duration << " milliseconds.\n";
        
        AVLPhonebook.printBalanceInfo();
        
        while (true) {
            cout << endl;
            cout << "Choose which action to perform from 1 to 6:\n";
            cout << "1 - Search a phonebook contact\n";
            cout << "2 - Adding a phonebook contact\n";
            cout << "3 - Deleting a phonebook contact\n";
            cout << "4 - Print the phonebook to a file(inorder)\n";
            cout << "    Print the phonebook to a file(preorder)\n";
            cout << "5 - Draw the phonebook as a Tree to a file\n";
            cout << "6 - Press 6 to exit\n";
            cout << "Enter choice: ";
            
            int choice;
            cin >> choice;
            cout << endl;
            
            if (choice == 1) {
                // Search contact
                string firstname,lastname,search;
                cout << "Search for a contact: ";
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
                cout << "Searching an item in the phonebook (BST) . . .\n" << endl;
                cout << "Phonebook: Searching for: (" << search << ")" << endl;
                cout << "====================================\n";
                
                auto startB = chrono::high_resolution_clock::now();
                searchContact(search,BSTPhonebook);
                auto endB = chrono::high_resolution_clock::now();
                cout << endl;
                cout << "Searching an item in the phonebook (AVL) . . .\n" << endl;
                cout << "Phonebook: Searching for: (" << search  << ")" << endl;
                cout << "====================================\n";
                
                auto startA = chrono::high_resolution_clock::now();
                searchContact(search, AVLPhonebook);
                auto endA = chrono::high_resolution_clock::now();
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                auto durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                cout << endl;
                
                cout << "The search in BST took " << durationB << " nanoseconds. . ." << endl;
                cout << "The search in AVL took " << durationA << " nanoseconds. . ." << endl;
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
                
                cout << "Adding an item to the phonebook AVL . . ."  << endl;
                auto startA = chrono::high_resolution_clock::now();
                addContact(firstName, lastName, phoneNumber, city, AVLPhonebook);
                auto endA = chrono::high_resolution_clock::now();
                cout << "===================================="  << endl;
                cout << "Contact has been added successfully to the AVL" << endl;
                cout << endl;
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                auto durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                
                cout << "Adding a contact to the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                cout << "Adding a contact to the AVL Tree took " << durationA << " nanoseconds. . ." << endl;
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
                bool checkB = deleteContact(firstname, lastname, AVLPhonebook);
                auto endA = chrono::high_resolution_clock::now(); // end time
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                
                if(checkA && checkB){
                    cout << "Deleted successfully" << endl;
                    cout << endl;
                    cout << "Deletion from the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                    cout << "Deletion from AVL Tree took " << durationA << " nanoseconds. . ." << endl;
                }
                else{
                    cout << "Contact does not exist in the tree" << endl;
                }
                
            } else if (choice == 4) {
                // Print phonebook in InOrder
                auto startB = chrono::high_resolution_clock::now();
                InOrderPrintToFile("phonebookInOrderBST.txt", BSTPhonebook);
                auto endB = chrono::high_resolution_clock::now();
                auto durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                
                auto startA = chrono::high_resolution_clock::now();
                InOrderPrintToFile("phonebookInOrderAVL.txt", AVLPhonebook);
                auto endA = chrono::high_resolution_clock::now();
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                cout << "Printing in order to file from the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                cout << "Printing in order to file from the AVL Tree took " << durationA << " nanoseconds. . ." << endl;
                
                // Print phonebook in PreOrder
                startB = chrono::high_resolution_clock::now();
                PreOrderPrintToFile("phonebookPreOrderBST.txt", BSTPhonebook);
                endB = chrono::high_resolution_clock::now();
                durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                
                startA = chrono::high_resolution_clock::now();
                PreOrderPrintToFile("phonebookPreOrderAVL.txt", AVLPhonebook);
                endA = chrono::high_resolution_clock::now();
                durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                cout << "Printing pre order to file from the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                cout << "Printing pre order to file from the AVL Tree took " << durationA << " nanoseconds. . ." << endl;
                
            } else if (choice == 5) {
                string fileA = "phonebookTreeAVL.txt";
                string fileB = "phonebookTreeBST.txt";
                
                auto startB = chrono::high_resolution_clock::now();
                BSTPhonebook.drawTreeToFile(fileB);
                auto endB = chrono::high_resolution_clock::now();
                auto durationB = chrono::duration_cast<chrono::nanoseconds>(endB - startB).count();
                
                auto startA = chrono::high_resolution_clock::now();
                AVLPhonebook.drawTreeToFile(fileA);
                auto endA = chrono::high_resolution_clock::now();
                auto durationA = chrono::duration_cast<chrono::nanoseconds>(endA - startA).count();
                cout << "Drawing tree to file from the Binary Tree took " << durationB << " nanoseconds. . ." << endl;
                cout << "Drawing tree to file from AVL Tree took " << durationA << " nanoseconds. . ." << endl;
    
            }else if (choice == 6) {
                // Quit
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
    AVLPhonebook.empty();
    BSTPhonebook.makeEmpty();
    
    inputfile.clear();
    inputfile.seekg(0);
    inputfile.close();
    return 0;
}
