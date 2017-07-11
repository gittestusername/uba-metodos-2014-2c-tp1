#ifndef _MATRIX_H_
#define _MATRIX_H_
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>

class Matrix {
public:
    
    Matrix();
    Matrix(int n, int m);
    Matrix(int n, int m, double dvalue);
    Matrix(int n, int m, double values[]);
    
    void load(int n, int m, double dvalue);
    void wipe();
    
    Matrix& operator=(const Matrix &other);
    bool operator==(const Matrix &other);
    
    friend std::ostream& operator<<(std::ostream& os, Matrix& obj);
    
    Matrix operator*(const Matrix &other);
    Matrix operator+(const Matrix &other);
    Matrix operator-(const Matrix &other);
    Matrix operator*(const double c);
    Matrix operator^(const int c);
    
    Matrix row(int i) const;
    Matrix col(int j) const;
    double get(int i, int j) const;
    void transpose();
    bool invert();
    std::vector<double> diagonal() const;
    void setDiagonal();
    void setRow(int i, Matrix& other);
    
    // Operaciones de triangulacion
    void addRow2Row(int i, int z, double c);
    double multiplyRowByVector(int i, Matrix& b);
    
    int n, m;
    std::vector<std::vector<double> > mat;
private:
    Matrix add(const Matrix &other, bool isadd);
};

#endif // _MATRIX_H_
