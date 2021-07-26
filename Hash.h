#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
using namespace std;

class Hash {
    private:
        int size;
    public:
        Hash();
        ~Hash();
        void setSize(int size);
        int hash1(string key);
        int hash2(string key);
};

Hash::Hash() {
    this->size = 1;
}

Hash::~Hash() {
    cout << "INFO: Hash Destructor called." << endl;
}

void Hash::setSize(int size) {
    this->size = size;
}

int Hash::hash1(string key) {
    hash<string> hash_string;
    return hash_string(key) % size;
}

int Hash::hash2(string key) {
    unsigned long long key_val = hash<string>{}(key);
    uint64_t b = 1, j = 0;
	while (j < size) {
		b = j;
		key_val = key_val * 2862933555777941757ULL + 1; // Did you say magic number?
		j = (b + 1) * (double(1LL << 31) / double((key_val >> 33) + 1));
	}
	return b;
}

#endif