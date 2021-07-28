#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
using namespace std;

class Hash {
    private:
        int size = 1;
    public:
        Hash();
        Hash(int size);
        ~Hash();
        void setSize(int size);
        int hash1(string key);
        int hash2(string key);
};

Hash::Hash(int size) {
    this->size = size;
}

Hash::~Hash() {
    cout << "INFO: Hash Destructor called." << endl;
}

/**
 * Set the size of the Hash map.
 * @param the new size.
 * @return nothing.
 */
void Hash::setSize(int size) {
    this->size = size;
}

/**
 * Use cpp internal hashing algorithm.
 * @param key the item to be hashed.
 * @return index in hash map.
 */
int Hash::hash1(string key) {
    hash<string> hash_string;
    return hash_string(key) % this->size;
}

/**
 * Jump consistent hashing. 
 * @param key the item to be hashed.
 * @return index in hash map.
 */
int Hash::hash2(string key) {
    unsigned long long key_val = hash<string>{}(key);
    uint64_t b = 1, j = 0;
    while (j < this->size) {
        b = j;
        key_val = key_val * 2862933555777941757ULL + 1; // Did you say magic number?
        j = (b + 1) * (double(1LL << 31) / double((key_val >> 33) + 1));
    }
    return b;
}

#endif