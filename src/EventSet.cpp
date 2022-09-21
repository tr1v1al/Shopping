/* 
 * File:   EventSet.cpp
 * @author MP-Team DECSAI
 * @note To be implemented by students 
 */


#include <fstream>
#include <unordered_map>
#include "EventSet.h"

using namespace std;

Event NULL_EVENT=Event();

void EventSet::initialize() {
    _nEvents = 0;
    _capacity = 0;
    _events = nullptr;    
}

void EventSet::reallocate(unsigned size) {
    if (size > capacity()) {
        Event * newArr = nullptr;
        newArr = new Event [size];
        copy(newArr);
        
        int oldsize = this->size();
        deallocate();
        _events = newArr;
        _capacity = size;
        _nEvents = oldsize;
        newArr = nullptr;
    }
}

void EventSet::deallocate() {
    if (_events != nullptr) {
        delete [] _events;
        initialize();
    }
}

void EventSet::copy(Event *dest) const {
    if (dest != nullptr)
        for (int i=0; i<size(); i++)
            dest[i] = at(i);
}

void EventSet::copy(const EventSet &orig) {
    deallocate();
    reallocate(orig.capacity());
    for (int i=0; i<orig.size(); i++)
        add(orig.at(i));
}

/**
 * @brief Give string[] and a string, it searches for string within string[]
 * @param array     The set of strings to llok for
 * @param n_array   The size of string[]
 * @param name      The string that is being seeked
 * @return          The position of the first occurrence of string within string[]
 *  -1 if the string does not belong to string[]
 */
int findString(const string array[], int n_array, const string & name) {
    int result_pos = -1, count = 0;
    while (count < n_array && result_pos == -1) {
        if (array[count] == name)
            result_pos = count;
        count++;
    }
    return result_pos;
}

std::string EventSet::to_string() const {
    string result;
    result = std::to_string(_nEvents) + " ";
    for (int i = 0; i < _nEvents; i++)
        result += _events[i].to_string() + "\n";
    return result;
}

EventSet::EventSet() {
    initialize();
}

EventSet::~EventSet() {
    deallocate();
}

int EventSet::size() const {
    return _nEvents;
}

int EventSet::capacity() const {
    return _capacity;
}

void EventSet::clear() {
    deallocate();
}
 
int EventSet::add(const Event & e) {
    if (size() == capacity())
        reallocate(2 * capacity() + 1);        
    _events[size()] = e;
    _nEvents++;
    return 1;
}

int EventSet::add(const std::string& line) {
    Event newEvent(line);
    return add(newEvent);
}

const Event & EventSet::at(int pos) const {
    if (pos >= 0 && pos < size())
        return _events[pos];
    else
        return NULL_EVENT;
}

EventSet::EventSet(const EventSet & orig) {
    initialize();
    copy(orig);
}

EventSet & EventSet::operator=(const EventSet & one) {
    if (this != &one)
        copy(one);
    return * this;
}

EventSet rawFilterEvents(const EventSet& es, const std::string &field, const std::string &value) {
    EventSet subset;
    string field_data;
    for (int i = 0; i < es.size(); i++) {
        
        if (field == "DateTime") {
            DateTime date_time, date_value;
            date_time = es.at(i).getField(field);
            date_value.set(value);
            if (date_time.sameDay(date_value))
                subset.add(es.at(i));
        } else {
            field_data = es.at(i).getField(field);
            if (field_data == value)
                subset.add(es.at(i));        
        }

    }
    return subset;
}

void findUnique(const EventSet& es, const std::string &field, std::string values[], int & n_values) {
    n_values = 0;
    string field_data;
    int k = 0;
    bool unique;
    
    if (field == "DateTime") {
        DateTime date_time;
        DateTime * dateArray = new DateTime [es.size()];
        int y,m,d,z = 0;
        
        for (int i = 0; i < es.size(); i++) {
            date_time = es.at(i).getField(field);
            unique = true;
            k = 0;
            while (k < n_values && unique) {
                if (date_time.sameDay(dateArray[k]))
                    unique = false;
                k++;
            }
            if (unique) {
                y = date_time.year();
                m = date_time.month();
                d = date_time.day();
                char local[64];
                sprintf(local, "%04i-%02i-%02i %02i:%02i:%02i UTC", y, m, d, z, z, z);       
                date_time.set(local);
                dateArray[n_values] = date_time;
                n_values++;
            }
        }
        for (int i = 0; i < n_values; i++) 
            values[i] = dateArray[i].to_string();
        
        delete [] dateArray;
        dateArray = nullptr;
        
    }else {
      
        for (int i = 0; i < es.size(); i++) {
            field_data = es.at(i).getField(field);
            unique = true;
            k = 0;
            while (k < n_values && unique) {
                if (field_data == values[k])
                    unique = false;
                k++;
            }
            if (unique) {
                values[n_values] = field_data;
                n_values++;
            }
        }    
    }
}

float sumPrice(const EventSet & evSet) {
    float sum = 0;
    float price = 0;
    for (int i = 0; i < evSet.size(); i++) {
        price = evSet.at(i).getPrice();
        if (price != -1)
            sum += price;
    }
    return sum;
}

void EventSet::write(ofstream &os) const {
    for (int i = 0; i < size(); i++)
        _events[i].write(os);
}

bool EventSet::read(ifstream &is, int nelements) {
    Event e;
    bool res = true;
    
    clear();
    int count = 0;
    while (count < nelements && res) {
        e.read(is);
        if (is && !e.isEmpty())
            add(e);
        else
            res = false;
        count++;
    }
    
    return res;
} 