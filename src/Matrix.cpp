/* 
 * File:   matrix.cpp
 * Author: MP-team 
 * @warning To be implemented by students. Follow the instructions given in the headers
 */


#include "Matrix.h"
#include <fstream>
#include <assert.h>

double v_npos = Matrix::npos;
const double c_npos = Matrix::npos;

using namespace std;

/**
 * @brief Copies the values in a 2D matrix org with nrows and ncols to an also 2D matrix dest.
 * It is assumed that org and dest have the memory properly allocated 
 * @param dest destination matrix
 * @param org  source matrix
 * @param nrows number of rows
 * @param ncols number of cols
 */
void copy(double **dest, double **org, size_t nrows, size_t ncols) {
    if (dest != nullptr && org != nullptr)
        for (int i=0; i<nrows; i++)
            for (int j=0; j<ncols; j++)
                dest[i][j] = org[i][j];
}

/** 
 * @brief this method reserve memory to allocate a 2D matrix of size r x c.
 * @param r number of rows
 * @param c number of cols
 * @return the pointer to the memory block containing the data
 */
double ** allocate(size_t r, size_t c) {
    double ** block;
    // allocate memory into block
    
    block = new double * [r];
    for (int i=0; i<r; i++)
        block[i] = new double [c];
    
    return block;
}

/**
 * @brief In this method, given an index idx it returns the key in the position i, 
 * such that i is the first position in the index such that idx.at(i).getPos()==p
 * @param idx the input index
 * @param p number of 
 * @return the key for first position i in the index such that idx.at(i).getPos()==p
 */
string labelWithValuePosInIndex(const Index & idx, size_t p) {
    string salida;

    bool enc = false;
    for (int i = 0; i < idx.size() && !enc; i++) {
        if (idx.at(i).getPos() == p) {
            salida = idx.at(i).getKey();
            enc = true;
        }
    }
    return salida;
}

void Matrix::deallocate() {
    if (_data != nullptr) {
        for (int i=0; i<rows(); i++)
            delete [] _data[i];
        delete [] _data;
        initialize();   
    }
}

void Matrix::initialize() {
    _nrows = 0;
    _ncols = 0;
    _rowLabels.clear();
    _colLabels.clear();    
    _data = nullptr;
}

Matrix::Matrix() {
    initialize();
}

void Matrix::setValues(double valor) {
    for (int i=0; i<rows(); i++)
        for (int j=0; j<columns(); j++)
            _data[i][j] = valor;
}


Matrix::Matrix(size_t f, size_t c, double vdef) {
    initialize();
    resize(f,c,vdef);
}

Matrix::Matrix(const Matrix& orig) {
    initialize();
    resize(orig.rows(),orig.columns());
    copy(_data, orig._data, rows(), columns());
    _rowLabels = orig._rowLabels;
    _colLabels = orig._colLabels;  
}

Matrix::Matrix(const std::string *rowLabels, size_t nrows, const std::string * colsLabels, size_t ncols, double value) {
    initialize();
    resize(rowLabels,nrows,colsLabels,ncols,value);
}


std::string Matrix::labelAtRow(size_t row) const {
    return labelWithValuePosInIndex(_rowLabels, row);
}

std::string Matrix::labelAtCol(size_t col) const {
    return labelWithValuePosInIndex(_colLabels, col);
}

void Matrix::resize(size_t nrows, size_t ncols, double value) {
    deallocate();
    if (nrows > 0 && ncols > 0) {
        _data = allocate(nrows,ncols);
        _nrows = nrows;
        _ncols = ncols;    
        setValues(value);    
    }
}

void Matrix::resize(const std::string * rowLabels, size_t nrows, const std::string * colLabels, size_t ncols, double value) {
    resize(nrows,ncols,value);
    setLabels(_rowLabels,rowLabels,nrows);
    setLabels(_colLabels,colLabels,ncols);    
}

void Matrix::clear() {
    deallocate();
}

Matrix::~Matrix() {
    deallocate();
}

double & Matrix::at(size_t f, size_t c) {
    //assert(f >= 0 && f < rows() && c >= 0 && c < columns());
    if (f >= 0 && f < rows() && c >= 0 && c < columns())
        return _data[f][c];
    else
        return v_npos;
}

double & Matrix::operator()(size_t f, size_t c) {
    return at(f,c);    
}

const double & Matrix::at(size_t f, size_t c) const {
    if (f >= 0 && f < rows() && c >= 0 && c < columns())
        return _data[f][c];
    else
        return c_npos;
}

const double & Matrix::operator()(size_t f, size_t c) const {
    return at(f,c);
}

int Matrix::rowIndexOf(const std::string & label) const {
    int res = 0;
    while (_rowLabels.at(res).getKey() != label && res < _rowLabels.size()) 
        res++;
    if (res == _rowLabels.size())
        res = Matrix::npos;
    else
        res = _rowLabels.at(res).getPos();
    return res;
}

int Matrix::colIndexOf(const std::string & label) const {
    int res = 0;
    while (_colLabels.at(res).getKey() != label && res < _colLabels.size()) 
        res++;
    if (res == _colLabels.size())
        res = Matrix::npos;
    else
        res = _colLabels.at(res).getPos();
    return res;    
}

double & Matrix::at(const std::string & rowLabel, const std::string & colLabel) {
    int r = rowIndexOf(rowLabel);
    int c = colIndexOf(colLabel);
    //assert(r != Matrix::npos && c != Matrix::npos);
    if (r != Matrix::npos && c != Matrix::npos)
        return _data[r][c];
    else
        return v_npos;
}

double & Matrix::operator()(const std::string & rowLabel, const std::string & colLabel) {
    return at(rowLabel,colLabel);   
}

const double & Matrix::at(const std::string & rowLabel, const std::string & colLabel)const {
    int r = rowIndexOf(rowLabel);
    int c = colIndexOf(colLabel);
    if (r != Matrix::npos && c != Matrix::npos)
        return _data[r][c];
    else
        return c_npos;  
}

const double & Matrix::operator()(const std::string & rowLabel, const std::string & colLabel)const {
    return at(rowLabel,colLabel);
}

ostream & operator<<(ostream & flujo, const Matrix& m) {
//    string *rowlabels = nullptr, * collabels = nullptr;
//    rowlabels = new string [m.rows()];
//    collabels = new string [m.columns()];
//    int nrowlab = m.getRowLabels(rowlabels);
//    int ncollab = m.getColLabels(collabels);
    
    flujo << m.rows() << " " << m.columns() << endl;
    if (m._rowLabels.size() > 0) {
        for (int i=0; i<m._rowLabels.size(); i++)
            flujo << m.labelAtRow(i) << " ";
        flujo << endl;
        for (int i=0; i<m._colLabels.size(); i++)
            flujo << m.labelAtCol(i) << " ";
        flujo << endl;
    }
    
    for (int i=0; i<m.rows(); i++) {
        for (int j=0; j<m.columns(); j++)
            flujo << m(i,j) << " ";
        flujo << endl;
    }
        
//    delete [] rowlabels;
//    delete [] collabels;
//    rowlabels = collabels = nullptr;
    return flujo;
}

Matrix & Matrix::operator=(const Matrix& m) {
    if (this != &m) {
        deallocate();
        resize(m.rows(),m.columns());
        copy(_data, m._data, rows(), columns());
        _rowLabels = m._rowLabels;
        _colLabels = m._colLabels;          
    }
    return * this;
}

void Matrix::setLabels(Index & idx, const string * labels, int nlabels){
    idx.clear();
    int lb,ub;
    bool correct = true;
    for (int i=0; i< nlabels && correct; i++){
        lb = idx.lower_bound(labels[i]);
        ub = idx.upper_bound(labels[i]);
        if (lb==ub) { // it is NOT in the set
            idx.add(Pair(labels[i],i));
        } else {
            correct = false;
         }
    }
    if (!correct) idx.clear();
 }

int Matrix::getLabels(const Index & idx, string * labels) const {
    if (idx.size() > 0) 
        for (int i = 0; i < idx.size(); i++) 
            labels[i] = idx.at(i).getKey();
    return idx.size();
}

int Matrix::getRowLabels(string * labels) const {
    return getLabels(_rowLabels,labels);
}

int Matrix::getColLabels(string *labels) const {
    return getLabels(_colLabels,labels);
}

size_t Matrix::columns() const {
    return _ncols;
}

size_t Matrix::rows() const {
    return _nrows;
}

//uncomment
void Matrix::save(const string & nf) const{
    ofstream fsal(nf);
    if (fsal.is_open()) {
        fsal << "#matrix "; 
        if (_rowLabels.size()==0)
            fsal << "unlabeled"<< endl;
        else fsal << "labeled" << endl;
        fsal << *this;
        fsal.close();
    } else cerr << "Fail creating " << nf << " file" <<endl;
}

void Matrix::load(const string & nf) {
    ifstream fent(nf);
    string labeled, cad;
    clear();
    if (fent.is_open()) {
         fent >> cad >> labeled;
         fent >> _nrows >> _ncols;
         if (labeled == "labeled"){
            for (int i=0;i<_nrows;i++){
                fent >> cad;
                _rowLabels.add(Pair(cad,i));
            }
             for (int i=0;i<_ncols;i++){
                fent >> cad;
                _colLabels.add(Pair(cad,i));
            }
        }
        _data = allocate(_nrows,_ncols); 
        for (int i = 0; i < _nrows; i++) {
            for (int j = 0; j < _ncols; j++) {
                fent >> _data[i][j];
            }
        }
        fent.close();
    } else cerr << "Fail reading " << nf << " file" <<endl;
}

Matrix & Matrix::operator*=(double val) {
    for (int i=0; i<rows(); i++)
        for (int j=0; j<columns(); j++)
            at(i,j) *= val;
    return * this;
}

Matrix & Matrix::operator+=(double val) {
    for (int i=0; i<rows(); i++)
        for (int j=0; j<columns(); j++)
            at(i,j) += val;
    return * this;    
}

Matrix Matrix::operator*(const Matrix & m) const {
    Matrix newmat;
    if (columns() == m.rows()) {
        
        string * rowlab = nullptr, * collab = nullptr;
        rowlab = new string [rows()];
        collab = new string [m.columns()];
        for (int i=0; i<rows(); i++)
            rowlab[i] = labelAtRow(i);
        for (int i=0; i<m.columns(); i++)
            collab[i] = m.labelAtCol(i);
        
        newmat.resize(rowlab,rows(),collab,m.columns());
        delete [] rowlab;
        delete [] collab;
        rowlab = collab = nullptr;
        
        for (int j=0; j<m.columns(); j++) 
            for (int i=0; i<rows(); i++) 
                for (int k=0; k<columns(); k++)
                        newmat(i,j) = newmat(i,j) + at(i,k)*m.at(k,j);
    }
    return newmat;
}

Matrix Matrix::operator+(double val) const {
    Matrix newmat(*this);
    newmat += val;
    return newmat;
}

Matrix Matrix::operator*(double val) const {
    Matrix newmat(*this);
    newmat *= val;   
    return newmat;
}

Matrix operator+(double ival, const Matrix & dch) {
    Matrix newmat(dch);
    newmat += ival;
    return newmat;
}

Matrix operator*(double ival, const Matrix & dch) {
    Matrix newmat(dch);
    newmat *= ival;
    return newmat;   
}

Matrix & Matrix::normalize(){
    double rowsum = 0.0;
    for (int i=0; i<rows(); i++) {
        rowsum = 0.0;
        for (int j=0; j<columns(); j++) 
            rowsum += at(i,j);
        if (rowsum != 0.0) 
            for (int j=0; j<columns(); j++)
                at(i,j) /= rowsum;        
        else
            for (int j=0; j<columns(); j++)
                at(i,j) = 1.0/columns();        
    }
}
