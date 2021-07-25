#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "./CuckooFilter.h"
using namespace std;

int main() {
    // Create cuckoo filter with 2 hash tables.
    CuckooFilter cf;
    vector<string> elements = {"chepson", "gerald", "brandon", "placali"};
    
    // INSERTION
    for (auto elt : elements) {
        cf.insert(elt);
    } 

    // SEARCHING.
    for (auto elt : elements) {
        if (!cf.find(elt)) 
            cout << elt + " not found! " << endl;
    }

    // DELETION
    for (auto elt : elements) {
        cf.remove(elt);
    }

    // EMPTY TABLES
    assert(0 == cf.size() and cf.empty());

    return 1;
}