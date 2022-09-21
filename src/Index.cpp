/* 
 * File:   Index.cpp
 * @author MP-Team DECSAI
 * @warning To be implemented by students 
 */
#include <string>
#include <iostream>
#include <fstream>
#include "Index.h"

using namespace std;

void Index::initialize() {
    _nEntries = 0;
    _capacity = 0;
    _entries = nullptr;    
}

void Index::reallocate(int size) {
    if (size > capacity()) {
        Pair * newArr = nullptr;
        newArr = new Pair [size];
        copy(newArr);
        
        int oldsize = this->size();
        deallocate();
        _entries = newArr;
        _capacity = size;
        _nEntries = oldsize;
        newArr = nullptr;
    }
}

void Index::deallocate() {
    if (_entries != nullptr) {
        delete [] _entries;
        initialize();
    }
}

void Index::copy(Pair *dest) {
    if (dest != nullptr)
        for (int i=0; i<size(); i++)
            dest[i] = at(i);
}

void Index::copy(const Index &orig) {
    deallocate();
    reallocate(orig.capacity());
    for (int i=0; i<orig.size(); i++)
        add(orig.at(i));
}

Index::Index(int onBrand) {
    initialize();
    setIOnWhich(onBrand);   
}

Index::~Index() {
    deallocate();
}

void Index::setIOnWhich(int val) {
    if (val == 0 || val == 1 || val == 2)
        _onBrand = val;
    else
        _onBrand = 0;
}

int Index::size() const {
    return _nEntries;
}

int Index::capacity() const {
    return _capacity;
}

void Index::clear() {
    deallocate();
}

int Index::getIOnWhich() const {
    return _onBrand;
}

int Index::add(const Pair & pair) {
    int failure = 1;

    if (pair.getKey() != EMPTY_FIELD) {
        
        if (size() == capacity())
            reallocate(2 * capacity() + 1);         
        
        int position = upper_bound(pair.getKey());
        for (int i = size(); i > position; i--) 
            _entries[i] = _entries[i-1];
        _entries[position] = pair;  
        _nEntries++;
    }
    else
        failure = 0;
    return failure;
}


void Index::build(const EventSet & evSet, int onBrand) {
    clear();
    setIOnWhich(onBrand);
    Pair a_pair;
    int res = 0;
    string field;
    
    for (int i = 0; i < evSet.size(); i++) {
        
        if (onBrand == 0)
            field = "UserID";
        else if (onBrand == 1)
            field = "Brand";
        else if (onBrand == 2)
            field = "Session";
        
        a_pair.set(evSet.at(i).getField(field),i);
        res = add(a_pair);
    }
}

int Index::lower_bound(const string & key) const {
    int position = 0;
    bool found = false;
    while (position < size() && !found) {
        if (key <= _entries[position].getKey())
            found = true;
        else
            position++;
    }
    return position;
}

int Index::upper_bound(const string & key) const {
    int position = 0;
    bool found = false;
    while (position < size() && !found) {
        if (key < _entries[position].getKey())
            found = true;
        else
            position++;
    }
    return position;   
}

const Pair & Index::at(int pos) const {
    return _entries[pos]; 
}

Pair & Index::at(int pos) {
    return _entries[pos];  
}

Index::Index(const Index & orig) {
    initialize();
    copy(orig);
}

Index & Index::operator=(const Index &one) {
    if (this != &one)
        copy(one);
    return * this;
}

void Index::print()const {
    cout << to_string();
}

Event getEvent(const EventSet & evSet, const Index & indx, int pos) {
    return evSet.at(indx.at(pos).getPos());
}

void write(ofstream &os, const EventSet & evSet, const Index & indx) {
    for (int i = 0; i < indx.size(); i++)
        getEvent(evSet,indx,i).write(os);
}

Index rawFilterIndex(const EventSet & evSet, const Index & indx, const std::string &field, const std::string &value) {
    Index subset;
    string field_data;
    for (int i = 0; i < indx.size(); i++) {
        field_data = getEvent(evSet,indx,i).getField(field);
        if (field_data == value) {
            subset.add(indx.at(i));
        }
    }
    return subset;
}

float sumPrice(const EventSet & evSet, const Index & indx) {
    float sum = 0;
    float price = 0;
    for (int i = 0; i < indx.size(); i++) {
        price = getEvent(evSet,indx,i).getPrice();
        if (price != -1)
            sum += price;
    }
    return sum;
}