#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

#define NUM_BUCKETS 1000
#define BUCKET_SIZE 4
#define MAX_NUM_KICKS 500

class CuckooFilter {
    private:
        int num_buckets;
        vector<vector<string>> buckets;
        uint32_t Hash(string item);
        tuple<string, uint32_t, uint32_t> getCuckooParams(string item);
    public:
        CuckooFilter();
        CuckooFilter(int size);
        ~CuckooFilter();
        bool Delete(string item);
        bool Lookup(string item);
        bool Insert(string item);
};

CuckooFilter::CuckooFilter() {
    this->num_buckets = NUM_BUCKETS;
    this->buckets.resize(NUM_BUCKETS);
}

CuckooFilter::CuckooFilter(int size) {
    this->num_buckets = size;
    this->buckets.resize(size);
}

CuckooFilter::~CuckooFilter() {
    cout << "INFO: Cuckoo Filter Destructor called." << endl;
}

/**
 * Calculate parameters for cuckoo hashing.
 * @param item the item to create parameters for.
 * @return a tuple of fingerprint, hash1 and hash2.
 */
tuple<string, uint32_t, uint32_t> CuckooFilter::getCuckooParams(string item) {
    hash<string> hash_string; 
    uint32_t hash = hash_string(item);
    string f_item = to_string(hash);
    uint32_t i1 = Hash(item) % num_buckets;
    uint32_t i2 = (i1 ^ Hash(f_item)) % num_buckets;
    return make_tuple(f_item, i1, i2);
}

/**
 * Search for an elelment in our filter.
 * @param item the item to be searched for.
 * @return true if item is in filter; false otherwise.  
 */
bool CuckooFilter::Lookup(string item) {
    tuple<string, uint32_t, uint32_t> params = getCuckooParams(item);
    string f_item = get<0>(params);
    uint32_t i1 = get<1>(params);
    uint32_t i2 = get<2>(params);

    for (auto e : buckets[i1]) {
        if (e == f_item) return true;
    }
    for (auto e : buckets[i2]) {
        if (e == f_item) return true;
    }
    return false;
}

/**
 * Insert an elelment into our filter.
 * @param item the item to be inserted.
 * @return true if insertion succeeded; false otherwise.  
 */
bool CuckooFilter::Insert(string item) {
    tuple<string, uint32_t, uint32_t> params = getCuckooParams(item);
    string f_item = get<0>(params);
    uint32_t i1 = get<1>(params);
    uint32_t i2 = get<2>(params);

    if (buckets[i1].size() < BUCKET_SIZE) {
        buckets[i1].push_back(f_item);
        return true;
    }
    if (buckets[i2].size() < BUCKET_SIZE) {
        buckets[i2].push_back(f_item);
        return true;
    }

    // Relocate existing items.
    uint32_t i = rand() % 2 == 0 ? i1 : i2;
    for (int n = 0; n < MAX_NUM_KICKS; n++) {
        int entry_index = rand() % BUCKET_SIZE;
        string entry = buckets[i][entry_index];
        swap(f_item, buckets[i][entry_index]);
        i = i ^ Hash(f_item);
        if (buckets[i].size() < BUCKET_SIZE) {
            buckets[i].push_back(f_item);
            return true;
        }
    }
    return false;   // if after MAX_NUM_KICKS, we haven't found an empty spot. Filter is full.
}

/**
 * Remove an elelment from our filter.
 * @param item the item to be removed.
 * @return true if item was removed successfully; false otherwise.  
 */
bool CuckooFilter::Delete(string item) {
    tuple<string, uint32_t, uint32_t> params = getCuckooParams(item);
    string f_item = get<0>(params);
    uint32_t i1 = get<1>(params);
    uint32_t i2 = get<2>(params);

    for (int i = 0; i < buckets[i1].size(); i++) {
        if (buckets[i1][i] == f_item) {
            buckets[i1].erase(buckets[i1].begin() + i);
            return true;
        }
    }
    for (int i = 0; i < buckets[i2].size(); i++) {
        if (buckets[i2][i] == f_item) {
            buckets[i2].erase(buckets[i2].begin() + i);
            return true;
        }
    }
    return false;
}

/**
 * Jump consistent hashing as seen in https://arxiv.org/pdf/1406.2294.pdf. 
 * @param key the item to be hashed.
 * @return index in hash map.
 */
uint32_t CuckooFilter::Hash(string key) {
    unsigned long long key_val = hash<string>{}(key);
    uint32_t b = 1, j = 0;
    while (j < this->num_buckets) {
        b = j;
        key_val = key_val * 2862933555777941757ULL + 1; // Did you say magic number?
        j = (b + 1) * (double(1LL << 31) / double((key_val >> 33) + 1));
    }
    return b;
}

#endif