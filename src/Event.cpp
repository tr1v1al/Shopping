/**
 * @file Event.cpp
 * @author DECSAI
 * @note To be implemented by students 
 * 
 */



#include <string>
#include <fstream>
#include "Event.h"
using namespace std;

Event::Event() {
    initDefault();
}

void Event::initDefault() {
    set(EVENT_DEFAULT);
}

Event::Event(const string & line) {
    set(line);
}

DateTime Event::getDateTime() const {
    return _dateTime;
}

string Event::getType() const {
    return _type;
}

string Event::getProductID() const {
    return _prod_id;
}

string Event::getCategoryID() const {
    return _cat_id;
}

string Event::getCategoryCode() const {
    return _cat_cod;
}

string Event::getBrand() const {
    return _brand;
}

double Event::getPrice() const {
    return _price;
}

string Event::getUserID() const {
    return _user_id;
}

string Event::getSession() const {
    return _session;
}

void Event::setDateTime(const string & time) {
    _dateTime.set(time);
}

void Event::setType(const string & type) {
    if (type == VALID_TYPES[0] || type == VALID_TYPES[1] || type == VALID_TYPES[2] || type == VALID_TYPES[3])
        _type = type;
    else
        _type = VALID_TYPES[0];
}

void Event::setProductID(const string & prod_id) {
    if (prod_id != "")
        _prod_id = prod_id;
    else
        _prod_id = EMPTY_FIELD;
}

void Event::setCategoryID(const string & cat_id) {
    _cat_id = cat_id;
}

void Event::setCategoryCode(const string & cat_cod) {
    _cat_cod = cat_cod;
}

void Event::setBrand(const string & brand) {
    _brand = brand;
}

void Event::setPrice(double price) {
    if (price >= 0)
        _price = price;
    else
        _price = -1;
}

void Event::setUserID(const string & user_id) {
    if (user_id != "")
        _user_id = user_id;
    else
        _user_id = EMPTY_FIELD;
}

void Event::setSession(const string & session) {
    if (session != "")
        _session = session;
    else
        _session = EMPTY_FIELD;
}

void Event::set(const string & line) {
    
    string data_chunks[10];
    int curr_pos = 0;
    int pos = 0;
    for (int i = 0; i < 8; i++) {
        pos = line.find(',', curr_pos);
        if (line[curr_pos] != ',') {
        data_chunks[i] = line.substr(curr_pos, pos - curr_pos);
        curr_pos = pos + 1;    
        } else {
            data_chunks[i] = "";
            curr_pos++;
        }        
    }    
    data_chunks[8] = line.substr(curr_pos, line.length() - curr_pos);
    
    double price = stod(data_chunks[6]);
    
    setDateTime(data_chunks[0]);
    setType(data_chunks[1]);
    setProductID(data_chunks[2]);
    setCategoryID(data_chunks[3]);
    setCategoryCode(data_chunks[4]);
    setBrand(data_chunks[5]);
    setPrice(price);
    setUserID(data_chunks[7]);
    setSession(data_chunks[8]);
}

bool Event::isEmpty() const {
    return _prod_id == EMPTY_FIELD || _user_id == EMPTY_FIELD || _session == EMPTY_FIELD || _price < 0;
}

string Event::to_string() const {
    string salida;
    salida += _dateTime.to_string();
    salida += "," + _type;
    salida += "," + _prod_id;
    salida += "," + _cat_id;
    salida += "," + _cat_cod;
    salida += "," + _brand;
    salida += "," + std::to_string(_price);
    salida += "," + _user_id;
    salida += "," + _session;
    return salida;
}

string Event::getField(const std::string &field) const {
    string field_value;
    if (field == "DateTime")
        field_value = _dateTime.to_string();
    else if (field == "Type")
        field_value = _type;
    else if (field == "ProductID")
        field_value = _prod_id;
    else if (field == "CategoryID")
        field_value = _cat_id;
    else if (field == "CategoryCode")
        field_value = _cat_cod;
    else if (field == "Brand")
        field_value = _brand;
    else if (field == "Price")
        field_value = std::to_string(_price);
    else if (field == "UserID")
        field_value = _user_id;
    else if (field == "Session")
        field_value = _session;
    return field_value;
}

void Event::write(ostream &os) const {
    os << to_string() << endl;
}

void Event::read(istream &is) {
    string line;
    getline(is,line);
    if (is)
        set(line);        
}