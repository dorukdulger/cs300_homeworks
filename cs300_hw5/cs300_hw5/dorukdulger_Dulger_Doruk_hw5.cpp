//
//  main.cpp
//  cs300_hw5
//
//  Created by Doruk Dulger on 19.05.2023.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include "Contact.h"
#include <cctype>
#include <algorithm>

using namespace std;

template <typename T>
void swap_(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Sorting Algorithms
void insertionSort(vector<Contact>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        Contact key = arr[i];
        int j = i - 1;
        while (j >= 0 && (arr[j].firstName > key.firstName || (arr[j].firstName == key.firstName && arr[j].lastName > key.lastName))) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

int partition(vector<Contact>& arr, int low, int high) {
    // Choose pivot as the median of first, middle, and last elements
    int mid = low + (high - low) / 2;
    if (arr[mid].firstName < arr[low].firstName || (arr[mid].firstName == arr[low].firstName && arr[mid].lastName < arr[low].lastName)) {
        swap_(arr[mid], arr[low]);
    }
    if (arr[high].firstName < arr[low].firstName || (arr[high].firstName == arr[low].firstName && arr[high].lastName < arr[low].lastName)) {
        swap_(arr[high], arr[low]);
    }
    if (arr[mid].firstName < arr[high].firstName || (arr[mid].firstName == arr[high].firstName && arr[mid].lastName < arr[high].lastName)) {
        swap_(arr[mid], arr[high]);
    }
    Contact pivot = arr[high];

    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].firstName < pivot.firstName || (arr[j].firstName == pivot.firstName && arr[j].lastName < pivot.lastName)) {
            ++i;
            swap_(arr[i], arr[j]);
        }
    }
    swap_(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Contact>& arr, int low, int high) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pivotIndex = partition(arr, low, high);

        // Recursively sort the subarrays
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

void merge(vector<Contact>& arr, int low, int mid, int high) {
    int left = low;
    int right = mid + 1;

    if (arr[mid].firstName < arr[right].firstName || (arr[mid].firstName == arr[right].firstName && arr[mid].lastName <= arr[right].lastName)) {
        return;
    }

    while (left <= mid && right <= high) {
        if (arr[left].firstName <= arr[right].firstName || (arr[left].firstName == arr[right].firstName && arr[left].lastName <= arr[right].lastName)) {
            ++left;
        } else {
            Contact temp = arr[right];
            int index = right;

            while (index != left) {
                arr[index] = arr[index - 1];
                --index;
            }
            arr[left] = temp;

            ++left;
            ++mid;
            ++right;
        }
    }
}

void mergeSort(vector<Contact>& arr, int low, int high) {
    if (low < high) {
        if (high - low + 1 < 10) {
            insertionSort(arr); // Use Insertion Sort for smaller subarrays
        } else {
            int mid = low + (high - low) / 2;
            mergeSort(arr, low, mid);
            mergeSort(arr, mid + 1, high);
            merge(arr, low, mid, high);
        }
    }
}

void heapify(vector<Contact>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && (arr[left].firstName > arr[largest].firstName || (arr[left].firstName == arr[largest].firstName && arr[left].lastName > arr[largest].lastName))) {
        largest = left;
    }

    if (right < n && (arr[right].firstName > arr[largest].firstName || (arr[right].firstName == arr[largest].firstName && arr[right].lastName > arr[largest].lastName))) {
        largest = right;
    }

    if (largest != i) {
        swap_(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<Contact>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; --i) {
        swap_(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}


vector<Contact> loadPhoneBook(const string& filename) {
    vector<Contact> phoneBook;
    ifstream file(filename);
    
    if (file.is_open()) {
        string firstName, lastName, phoneNumber, city;
        while (file >> firstName >> lastName >> phoneNumber >> city) {
            Contact contact{firstName, lastName, phoneNumber, city};
            phoneBook.push_back(contact);
        }
        file.close();
    } else {
        cout << "Failed to open the file." << endl;
    }
    
    return phoneBook;
}

bool isPartialNameSearch(const string& searchQuery) {
    return searchQuery.find(' ') == string::npos;
}

bool isMatch(const Contact& contact, const string& searchQuery, bool partialSearch) {
    if (partialSearch) {
        return contact.firstName.find(searchQuery) == 0;
    } else {
        size_t pos = searchQuery.find(' ');
        string firstName = searchQuery.substr(0, pos);
        string lastName = searchQuery.substr(pos + 1);
        return contact.firstName == firstName && contact.lastName == lastName;
    }
}

int sequentialSearch(const vector<Contact>& phoneBook, const string& search) {
    bool partialSearch = isPartialNameSearch(search);
    int count = 0;

    for (const auto& contact : phoneBook) {
        if (isMatch(contact, search, partialSearch)) {
            ++count;
            cout << contact.firstName << " " << contact.lastName << " "
                 << contact.phoneNumber << " " << contact.city << endl;
        }
    }
    return count;
}


bool binarySearch(const vector<Contact>& phoneBook, const string& searchQuery, bool partialSearch, int left, int right) {
    bool found = false;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        const Contact& contact = phoneBook[mid];

        if (isMatch(contact, searchQuery, partialSearch)) {
            found = true;
            cout << contact.firstName << " " << contact.lastName << " "
                 << contact.phoneNumber << " " << contact.city << endl;

            // Search to the left
            int prev = mid - 1;
            while (prev >= left && isMatch(phoneBook[prev], searchQuery, partialSearch)) {
                cout << phoneBook[prev].firstName << " " << phoneBook[prev].lastName << " "
                     << phoneBook[prev].phoneNumber << " " << phoneBook[prev].city << endl;
                --prev;
            }

            // Search to the right
            int next = mid + 1;
            while (next <= right && isMatch(phoneBook[next], searchQuery, partialSearch)) {
                cout << phoneBook[next].firstName << " " << phoneBook[next].lastName << " "
                     << phoneBook[next].phoneNumber << " " << phoneBook[next].city << endl;
                ++next;
            }

            return true;
        }

        if (partialSearch) {
            if (contact.firstName.compare(0, searchQuery.size(), searchQuery) < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        } else {
            if (contact.firstName == searchQuery) {
                return true;
            } else if (contact.firstName < searchQuery) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }

    return found;
}

auto quick_sorting_time(vector<Contact>& Phonebook){
    int N = 100; // start measuring
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < N; i++)
        quickSort(Phonebook, 0, Phonebook.size() - 1);
    auto end = chrono::steady_clock::now();
    auto Time = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / N;
    
    return Time;
}

auto merge_sorting_time(vector<Contact>& Phonebook){
    int N = 100; // start measuring
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < N; i++)
        mergeSort(Phonebook, 0, Phonebook.size() - 1);
    auto end = chrono::steady_clock::now();
    auto Time = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / N;
    
    return Time;
}

auto insertion_sorting_time(vector<Contact>& Phonebook){
    int N = 100; // start measuring
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < N; i++)
        insertionSort(Phonebook);
    auto end = chrono::steady_clock::now();
    auto Time = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / N;
    
    return Time;
}

auto heap_sorting_time(vector<Contact>& Phonebook){
    int N = 100; // start measuring
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < N; i++)
        heapSort(Phonebook);
    auto end = chrono::steady_clock::now();
    auto Time = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / N;
    
    return Time;
}

int main(){
    string filename;
    ifstream inputfile;
    
    cout << "Please enter the contact file name:" << endl;
    cin >> filename;
    
    inputfile.open(filename.c_str());
    if (inputfile.fail()){
        cout << "File cannot be opened" << endl;
    }else{
        vector<Contact> phoneBook = loadPhoneBook(filename);
        
        vector<Contact> q_Phonebook = phoneBook;
        vector<Contact> m_Phonebook = phoneBook;
        vector<Contact> i_Phonebook = phoneBook;
        vector<Contact> h_Phonebook = phoneBook;
        
        // Search contact
        string firstname,lastname,search;
        cout << "Please enter the word to be queried:\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, search);
        
        cout << endl;
        
        auto q_time = quick_sorting_time(q_Phonebook);
        auto m_time = merge_sorting_time(m_Phonebook);
        auto i_time = insertion_sorting_time(i_Phonebook);
        auto h_time = heap_sorting_time(h_Phonebook);
        
        cout << "Sorting the vector copies" << endl;
        cout << "====================================\n";
        cout << "Quick Sort Time: " << q_time << " Nanoseconds" << endl;
        cout << "Insertion Sort Time: " << m_time << " Nanoseconds" << endl;
        cout << "Merge Sort Time: " << i_time << " Nanoseconds" << endl;
        cout << "Heap Sort Time: " << h_time << " Nanoseconds" << endl;
        
        phoneBook = q_Phonebook;
        
        auto smallest = q_time;

        if (m_time < smallest) {
            smallest = m_time;
            phoneBook = m_Phonebook;
        }
        if (i_time < smallest) {
            smallest = i_time;
            phoneBook = i_Phonebook;
        }
        if (h_time < smallest) {
            smallest = h_time;
            phoneBook = h_Phonebook;
        }
        
        cout << endl;

        cout << "Searching for " << search << endl;
        cout << "====================================\n";
        transform(search.begin(), search.end(), search.begin(),[](unsigned char c){ return std::toupper(c); });
        
        auto start = chrono::steady_clock::now();
        bool found = binarySearch(phoneBook, search, isPartialNameSearch(search), 0, phoneBook.size() - 1);
        auto end = chrono::steady_clock::now();
        auto binary_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        if(!found)
            cout << search << " does NOT exist in the dataset\n";
        
        cout << "Binary Search Time: " << binary_time << " Nanoseconds" << endl;
        
        cout << endl;
        
        cout << "Search results for Sequential Search:" << endl;
        auto startS = chrono::high_resolution_clock::now();
        int count = sequentialSearch(phoneBook, search);
        auto endS = chrono::high_resolution_clock::now();
        auto seq_time = chrono::duration_cast<chrono::nanoseconds>(endS - startS).count();
        
        if(count == 0)
            cout << search << " does NOT exist in the dataset\n";
        
        cout << "Sequential Search Time: " << seq_time << " Nanoseconds" << endl;
        
        cout << endl;
        
        double speedup_search = static_cast<double>(seq_time) / static_cast<double>(binary_time);
        
        cout << "SpeedUp between Search Algorithms" << endl;
        cout << "====================================\n";
        cout << "(Sequential Search/ Binary Search) SpeedUp = " << speedup_search << endl << endl;
        
        double i = static_cast<double>(i_time) / static_cast<double>(q_time);
        double m = static_cast<double>(m_time) / static_cast<double>(q_time);
        double h = static_cast<double>(h_time) / static_cast<double>(q_time);
        
        cout << "SpeedUps between Sorting Algorithms" << endl;
        cout << "====================================\n";
        cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << i << endl;
        cout << "(Merge Sort / Quick Sort) SpeedUp = " << m << endl;
        cout << "(Heap Sort / Quick Sort) SpeedUp = " << h << endl;
    }
        
    inputfile.clear();
    inputfile.seekg(0);
    inputfile.close();
    
    return 0;
}
