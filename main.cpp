#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "./CuckooFilter.h"

using namespace std;

#define TABLE_SIZE 2

int main() {
    // Create cuckoo filter with 2 hash tables.
    CuckooFilter filter(TABLE_SIZE);
    vector<string> elements = {"chepson", "gerald", "TheProcess", "brandon"};
    vector<string> mixedElements = {"chepson", "gerald", "TheProcess", "brandon", "placali", "Koded"};
    
    // INSERTION
    for (auto elt : elements) {
        if (filter.Insert(elt)) {
            cout << elt + " inserted to filter. " << endl;
        }
        else {
            cout << elt + " cannot be inserted since filter is full. RE-HASH all elements" << endl;
        }
    }
    cout << endl; 

    // SEARCHING.
    for (auto elt : mixedElements) {
        if (filter.Lookup(elt)) {
            cout << elt + " might be in filter. " << endl;
        }
        else {
            cout << elt + " is definitely not in filter. " << endl;
        }
    }
    cout << endl;

    // DELETION
    for (auto elt : elements) {
        if (filter.Delete(elt)) {
            cout << elt + " removed from filter. " << endl;
        }
    }

    return 1;
}