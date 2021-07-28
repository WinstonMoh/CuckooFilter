#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "./CuckooFilter.h"

using namespace std;

#define TABLE_SIZE 1

int main() {
    // Create cuckoo filter with 2 hash tables.
    CuckooFilter filter(TABLE_SIZE);
    vector<string> elements = {"chepson", "gerald", "brandon", "placali"};
    
    // INSERTION
    for (auto elt : elements) {
        if (filter.insert(elt)) {
            cout << elt + " inserted to filter. " << endl;
        }
    } 

    // SEARCHING.
    for (auto elt : elements) {
        if (!filter.find(elt)) {
            cout << elt + " not found! " << endl;
        }
        else {
            cout << elt + " found! " << endl;
        }
    }

    // DELETION
    for (auto elt : elements) {
        if (filter.remove(elt)) {
            cout << elt + " removed from filter. " << endl;
        }
    }

    // EMPTY TABLES
    assert(filter.size() == 0 and filter.empty());

    return 1;
}