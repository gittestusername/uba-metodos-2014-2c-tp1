#define _USE_MATH_DEFINES
#include <math.h>

#include "Matrix.h"

// Constructors
Matrix::Matrix() { this->n = 0; this->m = 0; }
Matrix::Matrix(int n, int m) { load(n, m, 0.0); }
Matrix::Matrix(int n, int m, double dvalue) { load(n, m, dvalue); }
void Matrix::load(int n, int m, double dvalue) {
    this->n = n;
    this->m = m;
    mat.resize(n);

    for (int i = 0; i < n; i++) {
        mat[i].resize(m);
        for (int j=0; j < m; j++) {
            mat[i][j] = dvalue;
        }
    }
}

Matrix::Matrix(int n, int m, double values[]) {
    this->n = n;
    this->m = m;
    mat.resize(n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        mat[i].resize(m);
        for (int j = 0; j < m; j++) {
            mat[i][j] = values[k];
            k++;
        }
    }
}

void Matrix::wipe() {
    n = 0;
    m = 0;
    mat.clear();
}

Matrix& Matrix::operator=(const Matrix &other) {
    n = other.n;
    m = other.m;
    mat.resize(other.n);

    for (int i = 0; i < other.n; i++) {
        mat[i].resize(other.m);
        for (int j = 0; j < other.m; j++) {
            mat[i][j] = other.mat[i][j];
        }
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, Matrix& obj) {
    for (int i = 0; i < obj.n; i++) {
        os << "[ ";
        for (int j = 0; j < obj.m; j++) {
            os << obj.get(i, j) << " ";
        }
        os << "]" << std::endl;
    }
    return os;
}

bool Matrix::operator==(const Matrix &other) {
    if (n != other.n || m != other.m) {
        return false;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (fabs(mat[i][j] - other.mat[i][j]) > 0.0000000000000001) {
                return false;
            }
        }
    }

    return true;
}

Matrix Matrix::operator+(const Matrix &other) {
    return add(other, true);
}

Matrix Matrix::operator-(const Matrix &other) {
    return add(other, false);
}

Matrix Matrix::add(const Matrix &other, bool isadd) {
    //TODO: Tirar excepcion si las dimensiones de las matrices no son iguales
    Matrix R(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < other.m; j++) {
            if (isadd) {
                R.mat[i][j] = mat[i][j] + other.mat[i][j];
            } else {
                R.mat[i][j] = mat[i][j] - other.mat[i][j];
            }
        }
    }
    return R;
}

Matrix Matrix::operator*(const Matrix &other) {    
    //TODO: Tirar excepcion si this->m != other.n
    Matrix R(n, other.m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < other.m; j++) {
            for (int k = 0; k < other.n; k++)  {
                R.mat[i][j] += mat[i][k] * other.mat[k][j];
            }
        }
    }
    return R;
}

Matrix Matrix::operator*(const double c) {
    Matrix R = *this;
    for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) {
            R.mat[i][j] = R.mat[i][j] * c;
        }
    }
    return R;
}

Matrix Matrix::operator^(const int c) {
    //TODO: Que tendria que dar si c es cero?
    Matrix R = *this;
    for (int i=0; i < c-1; i++) {
        R = R*R;
    }
    return R;
}

// Row operations
Matrix Matrix::row(int i) const {
    Matrix R(1, m);
    for (int j = 0; j < m; j++) {
        R.mat[0][j] = mat[i][j];
    }
    return R;
}

void Matrix::setRow(int i, Matrix& other) {
    for (int j = 0; j < m; j++) {
        mat[i][j] = other.mat[0][j];
    }
}

void Matrix::addRow2Row(int i, int k, double c) {
    for (int j = 0; j < m; j++) {
        mat[i][j] = mat[i][j] + mat[k][j] * c;
    }
}

double Matrix::multiplyRowByVector(int i, Matrix& b) {
    double result = 0.0; 
    for (int j = 0; j < m; j++) {
        result += mat[i][j] * b.mat[j][0];
    }
    return result;
}

Matrix Matrix::col(int j) const {
    Matrix R(n, 1);
    for (int i = 0; i < n; i++) {
        R.mat[i][0] = mat[i][j];
    }
    return R;
}

double Matrix::get(int i, int j) const {
    return mat[i][j];
}
