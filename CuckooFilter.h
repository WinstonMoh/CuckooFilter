#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <iostream>
#include <string>
#include <vector>
#include "./Hash.h"
using namespace std;

class CuckooFilter {
    private:
        int h1Count, h2Count;
        vector<string> hash1, hash2;
        Hash hash_function;
    public:
        CuckooFilter(int size);
        ~CuckooFilter();
        bool remove(string item);
        bool find(string item);
        bool insert(string item);
        int size();
        bool empty();
};

CuckooFilter::CuckooFilter(int size) {
    this->h1Count = 0;
    this->h2Count = 0;
    this->hash1.resize(size);
    this->hash2.resize(size);
    this->hash_function.setSize(size);
}

CuckooFilter::~CuckooFilter() {
    cout << "INFO: Cuckoo Filter Destructor called." << endl;
}

bool CuckooFilter::find(string item) {
    int h1_position = hash_function.hash1(item);
    bool h1_exists = hash1[h1_position] == item;
    if (h1_exists) return true;

    int h2_position = hash_function.hash2(item);
    return hash2[h2_position] == item;
}

bool CuckooFilter::insert(string item) {
    int h1_position = hash_function.hash1(item);
    string curr_item = hash1[h1_position];
    if (curr_item == "") {
        hash1[h1_position] = item;
        h1Count++;
        return true;
    }

    // move current item to table 2. 
    // Will handle cycles later.
    int h2_position = hash_function.hash2(curr_item);
    if (hash2[h2_position] == "") {
        hash2[h2_position] = curr_item;
        hash1[h1_position] = item;
        h2Count++;
        return true;
    }
    return false;
}

bool CuckooFilter::remove(string item) {
    int h1_position = hash_function.hash1(item);
    if (hash1[h1_position] == item) {    // delete item.
        hash1[h1_position] = "";
        h1Count--;
        return true;
    }

    // Check in table 2.
    int h2_position = hash_function.hash2(item);
    if (hash2[h2_position] == item) {
        hash2[h2_position] = "";
        h2Count--;
        return true;
    }
    return false;
}

int CuckooFilter::size() {
    return this->h1Count + this->h2Count;
}

bool CuckooFilter::empty() {
    return this->h1Count == 0 and this->h2Count == 0;
}

#endif