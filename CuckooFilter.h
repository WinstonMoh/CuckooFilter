#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <iostream>
#include <string>
using namespace std;

class CuckooFilter {
    private:

    public:
        CuckooFilter();
        void remove(string item);
        bool find(string item);
        bool insert(string item);
        int size();
        bool empty();
};

#endif