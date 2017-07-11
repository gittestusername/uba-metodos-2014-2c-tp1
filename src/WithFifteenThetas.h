#ifndef WITH_FIFTEEN_THETAS
#define WITH_FIFTEEN_THETAS
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "Matrix.h"

class WithFifteenThetas {
public:
    WithFifteenThetas();
    WithFifteenThetas(std::istream& istream, int& ninst);
    WithFifteenThetas(double r_i, double r_e, int m, int n, double isovalue,
                       std::vector<double> T_i, std::vector<double> T_e);
    void load(std::istream& istream, int& ninst);
    void load(double r_i, double r_e, int m, int n, double isovalue,
                       std::vector<double> T_i, std::vector<double> T_e);
    void loadData(std::istream& istream);
    void printResult(std::ostream& ostream);
    void printIsoterma(std::ostream& ostream);
                
    void reset();
    void wipe();
    void resolve();
    std::vector<double> getIsoterm() const;
    void toggleLU();

    double r_i; 
    double r_e; 
    int m; 
    int n; 
    double isovalue;
    
    // Benchmarking
    void benchmarkResolve();
    unsigned long long int getBenchmarkResult() const;
    
private:
    std::vector<double> getCoefficients(double& dr, double& dt, double& r_i, int& j);
    Matrix backSubstitution(Matrix& A, Matrix& b);
    Matrix forwardSubstitution(Matrix& A, Matrix& b);
    void gaussianElimination(Matrix& A, Matrix& b);
    void LUDecomposition(Matrix& A);
    bool LUDecompCheck(Matrix& O, Matrix& A);
    Matrix LUSubstitution(Matrix& A, Matrix& b);
    Matrix LUSimpleSubstitution(Matrix& A, Matrix& b);
    
    std::vector<double> T_i; 
    std::vector<double> T_e;
    Matrix R;
    Matrix A;
    Matrix b;
    bool isLUMode;
    bool isLULoaded;
    
    // Distance to isovalue
    double dist(double) const;
    
    // Benchmarking
    unsigned long long int bench_start, bench_end, bench_result;
    void startBenchmark();
    void endBenchmark();    
};

#endif // WITH_FIFTEEN_THETAS
