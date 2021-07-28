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
        string insertIntoFirstTable(string item);
        string insertIntoSecondTable(string item);
    public:
        CuckooFilter();
        CuckooFilter(int size);
        ~CuckooFilter();
        bool remove(string item);
        bool find(string item);
        bool insert(string item);
        int size();
        bool empty();
};

CuckooFilter::CuckooFilter() {
    this->h1Count = 0;
    this->h2Count = 0;
    this->hash1.resize(1000);
    this->hash2.resize(1000);
    this->hash_function.setSize(1000);
}

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

/**
 * Search for an elelment in our filter.
 * @param item the item to be searched for.
 * @return true if item is in filter; false otherwise.  
 */
bool CuckooFilter::find(string item) {
    int h1_position = hash_function.hash1(item);
    bool h1_exists = hash1[h1_position] == item;
    if (h1_exists) return true;

    int h2_position = hash_function.hash2(item);
    return hash2[h2_position] == item;
}


string CuckooFilter::insertIntoFirstTable(string item) {
    int position = hash_function.hash1(item);
    string curr_item = hash1[position];
    hash1[position] = item;
    h1Count += (curr_item == "") ? 1 : 0;
    return curr_item;
}

string CuckooFilter::insertIntoSecondTable(string item) {
    int position = hash_function.hash2(item);
    string curr_item = hash2[position];
    hash2[position] = item;
    h2Count += (curr_item == "") ? 1 : 0;
    return curr_item;
}

/**
 * Insert an elelment into our filter.
 * @param item the item to be inserted.
 * @return true if insertion succeeded; false otherwise.  
 */
bool CuckooFilter::insert(string item) {
    string returned_str = insertIntoFirstTable(item);
    while (returned_str != "") {
        returned_str = insertIntoSecondTable(returned_str);
    }
    return true;
}

/**
 * Remove an elelment from our filter.
 * @param item the item to be removed.
 * @return true if item was removed successfully; false otherwise.  
 */
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

/**
 * Get the size of the filter - the number of used slots.
 * @return the filter size.  
 */
int CuckooFilter::size() {
    return this->h1Count + this->h2Count;
}

/**
 * Find out if the filter is empty or not.
 * @return true if filter is empty; false otherwise.  
 */
bool CuckooFilter::empty() {
    return this->h1Count == 0 and this->h2Count == 0;
}

#endif