#define _USE_MATH_DEFINES
#include <math.h>

#include "Matrix.h"
#include "WithFifteenThetas.h"
#include "tiempo.h"

WithFifteenThetas::WithFifteenThetas() { 
    isLUMode = false; 
    isLULoaded = false;
}
WithFifteenThetas::WithFifteenThetas(std::istream& istream, int& ninst) { 
    //TODO: Complete this.
    istream >> ninst;
}
WithFifteenThetas::WithFifteenThetas(double r_i, double r_e, int m, int n, double isovalue,
                       std::vector<double> T_i, std::vector<double> T_e) {
    load(r_i, r_e, m, n, isovalue, T_i, T_e);
}
void WithFifteenThetas::load(double r_i, double r_e, int m, int n, double isovalue,
                       std::vector<double> T_i, std::vector<double> T_e) {
    this->r_i = r_i;
    this->r_e = r_e;
    this->m = m;
    this->n = n;
    this->isovalue = isovalue;
    this->T_i = T_i;
    this->T_e = T_e;
    
    bench_start = 0;
    bench_end = 0;
    bench_result = 0;
}

void WithFifteenThetas::load(std::istream& istream, int& ninst) {
    double r_i, r_e, iso;
    int m, n;
    std::string line;
    
    istream >> r_i;
    istream >> r_e;
    istream >> m;
    istream >> n;
    istream >> iso;
    istream >> ninst;
    std::getline(istream,line);
    
    std::vector<double> v(0);
    load(r_i, r_e, m, n, iso, v, v);
}

void WithFifteenThetas::loadData(std::istream& istream) {
    std::string line;
    T_i.resize(n);
    
    for (int i=0; i < n; i++) {
        istream >> T_i[i];
    }
    
    T_e.resize(n);
    for (int i=0; i < n; i++) {
        istream >> T_e[i];
    }
    
    getline(istream,line);
}

void WithFifteenThetas::printResult(std::ostream& ostream) {
    for (int i=0; i < R.n; i++) {
        ostream << R.mat[i][0] << std::endl;
    }
}

void WithFifteenThetas::printIsoterma(std::ostream& ostream) {
    std::vector<double> v = getIsoterm();
    for (uint i = 0; i < v.size(); ++i) {
       ostream << v[i] << std::endl;
    }
}

void WithFifteenThetas::reset() {
    //TODO: Creo que se podria sacar bool del lado izquierdo
    if (!isLUMode || !isLULoaded) {
        A.wipe();
    }
    b.wipe();
    R.wipe();
    
    bench_start = 0;
    bench_end = 0;
    bench_result = 0;
}

void WithFifteenThetas::wipe() {
    r_i = 0;
    r_e = 0;
    m = 0;
    n = 0;
    isovalue = 0;
    T_i.clear();
    T_e.clear();
    isLULoaded = false;
    reset();
}

void WithFifteenThetas::toggleLU() {
    isLUMode = !isLUMode;
    wipe();
}

void WithFifteenThetas::resolve() {
    
    // Calculo deltas
    double dr = (r_e - r_i) / (m - 1);
    double dt = 2*M_PI/n;
    
    // Lleno b
    b.load(n * m, 1, 0.0);
    for(int i = 0; i < n; i++) {
        b.mat[i][0] = T_i[i];
        b.mat[n*m-i-1][0] = T_e[i];
    }
    
    //TODO: Creo que se podria sacar bool del lado izquierdo
    if (!isLUMode || !isLULoaded) {
        // Lleno A
        A.load(n * m, n * m, 0.0);
        std::vector<double> C(4,0.0);
        int j = 1, k = n+1;
        for (int i=0; i < n*m; i++) {
            // Areas de identidad de A
            if (i < n || i >= (n*m-n)) {
                A.mat[i][i] = 1;
            } else {
                if (k > n) {
                    j = j+1;
                    k = 1;
                    C = getCoefficients(dr, dt, r_i, j);
                }
                
                //t_{j,k}
                A.mat[i][i] = C[0];
                
                //t_{j+1,k}, t_{j-1,k}
                A.mat[i][i-n] = C[1];
                A.mat[i][i+n] = C[2];
                
                //t_{j,k+1}
                if (k == n) {
                    A.mat[i][i-n+1] = C[3];
                } else {
                    A.mat[i][i+1] = C[3];
                }
                    
                //t_{j,k-1}
                if (k == 1) {
                    A.mat[i][i+n-1] = C[3];
                } else {
                    A.mat[i][i-1] = C[3];
                }
                
                k = k+1;
            }
        }
    }
    
    // Resuelvo el sistema
    if (isLUMode) {
        if (!isLULoaded) {
            LUDecomposition(A);
            isLULoaded = true;
        }
        R = LUSubstitution(A, b);
    } else {
        gaussianElimination(A, b);
        R = backSubstitution(A, b);
    }
}

std::vector<double> WithFifteenThetas::getCoefficients(double& dr, double& dt, double& r_i, int& j) {
    double r = r_i + (j-1)*dr;
    std::vector<double> coefs(4);
    coefs[0] = 1/(r*dr) - 2/pow(dr,2) - 2/pow(r*dt,2);
    coefs[1] = 1/pow(dr,2) - 1/(r*dr);
    coefs[2] = 1/pow(dr,2);
    coefs[3] = 1/pow(r*dt,2);
    return coefs;
}

Matrix WithFifteenThetas::backSubstitution(Matrix& A, Matrix& b) {
    Matrix R(A.m, 1, 0.0);
    
    for (int i = A.n-1; i >= 0; i--) {
        if (fabs(A.mat[i][i]) > 0.0000000000000001)  {
            double c = A.multiplyRowByVector(i, R);
            R.mat[i][0] = (b.mat[i][0]-c) / A.mat[i][i];
        }
    }
    
    return R;
}

Matrix WithFifteenThetas::forwardSubstitution(Matrix& A, Matrix& b) {
    Matrix R(A.m, 1, 0.0);
    
    for (int i = 0; i < A.n; i++) {
        if (fabs(A.mat[i][i]) > 0.0000000000000001)  {
            double c = A.multiplyRowByVector(i, R);
            R.mat[i][0] = (b.mat[i][0]-c) / (A.mat[i][i]);
        }
    }
    
    return R;
}

void WithFifteenThetas::gaussianElimination(Matrix& A, Matrix& b) {
    for (int i = 0; i < (A.m-1); i++) {
        for (int z = i+1; z < A.m; z++) {
            double c = A.mat[z][i] / A.mat[i][i];
            // A.addRow2Row(z, i, -1*c);
            // b.addRow2Row(z, i, -1*c);
            for (int j = i+1; j < A.m; j++) {
                A.mat[z][j] = A.mat[z][j] + A.mat[i][j] * (-1*c);
            }
            b.mat[z][0] = b.mat[z][0] + b.mat[i][0] * (-1*c);
        }
    }
}

bool WithFifteenThetas::LUDecompCheck(Matrix& O, Matrix& A) {
    Matrix L(A.n, A.m, 0.0);
    Matrix U(A.n, A.m, 0.0);
    
    for (int i = 0; i < A.m; i++) {
        for (int j = 0; j < A.m; j++) {
            if (i > j) {
                L.mat[i][j] = A.mat[i][j];
            } else {
                U.mat[i][j] = A.mat[i][j];
            }
            if (i == j) {
                L.mat[i][j] = 1;
            }
        }
    }
    
    Matrix R = L*U;
    return O==R;
}

void WithFifteenThetas::LUDecomposition(Matrix& A) {
    for (int i = 0; i < (A.m-1); i++) {
        for (int z = i + 1; z < A.m; z++) {
            double c = A.mat[z][i] / A.mat[i][i];
            
            // Diagonal inferior (L)
            A.mat[z][i] = c;
            
            // Diagonal superior (U)
            for (int j = i+1; j < A.m; j++) {
                A.mat[z][j] = A.mat[z][j] + A.mat[i][j] * (-1*c);
            }
        }
    }
}

Matrix WithFifteenThetas::LUSimpleSubstitution(Matrix& A, Matrix& b) {
    Matrix L(A.n, A.m, 0.0);
    Matrix U(A.n, A.m, 0.0);
    
    for (int i = 0; i < A.m; i++) {
        for (int j = 0; j < A.m; j++) {
            if (i > j) {
                L.mat[i][j] = A.mat[i][j];
            } else {
                U.mat[i][j] = A.mat[i][j];
            }
            if (i == j) {
                L.mat[i][j] = 1;
            }
        }
    }
    
    Matrix y = forwardSubstitution(L, b);
    Matrix x = backSubstitution(U, y);
    
    return x;
}

Matrix WithFifteenThetas::LUSubstitution(Matrix& A, Matrix& b) {
    Matrix y(A.m, 1, 0.0);
    Matrix x(A.m, 1, 0.0);
    
    // Resolucion de Ly = b, forward substitution.
    for (int i = 0; i < A.n; i++) {
        double s = 0;
        for (int j = 0; j < i; j++) {
            s += A.mat[i][j] * y.mat[j][0];
        }
        y.mat[i][0] = b.mat[i][0] - s;
    }
    
    // Resolucion de Ux = y, back substitution.
    for (int i = A.n - 1; i >= 0; i--) {
        if (fabs(A.mat[i][i]) > 0.0000000000000001)  {
            double s = 0;
            for (int j = i+1; j < A.m; j++) {
                s += A.mat[i][j] * x.mat[j][0];
            }
            x.mat[i][0] = (y.mat[i][0]-s) / (A.mat[i][i]);
        }
    }
    
    return x;
}

// Funciones de isoterma
double WithFifteenThetas::dist(double a) const {
    return fabs(a - isovalue);
}

std::vector<double> WithFifteenThetas::getIsoterm() const {
    std::vector<double> r(n, 0);

    for (int k = 0; k < n; ++k) {

        int best = 0;

        for (int j = 0; j < m; ++j) {
			best = j;
			if (R.mat[j * n + k][0] < isovalue) {
				break;
			}
        }

        int r1, r2;
        double dr = (r_e - r_i) / (m - 1);
        // Si el ultimo es mas caliente que la isoterma cerramos ahi.
        if (best >= m) {
            r[k] = r_e;
            continue;
        }

        r1 = best - 1;
        r2 = best;

        double t1, t2;
        t1 = R.mat[r1*n + k][0];
        t2 = R.mat[r2*n + k][0];

        //double dt = 2*M_PI/n;

        r[k] = (isovalue - t1)*((r2-r1)/(t2-t1)) + r1;
        r[k] *= dr;
	    r[k] += r_i;	
        //TODO: cambiar por busqueda binaria.


    }

    return r;
}

// Benchmarking
void WithFifteenThetas::startBenchmark() {
        MEDIR_TIEMPO_START(bench_start);
}
void WithFifteenThetas::endBenchmark() {
        MEDIR_TIEMPO_STOP(bench_end);
        bench_result = bench_end - bench_start;
}
unsigned long long int WithFifteenThetas::getBenchmarkResult() const {
        return bench_result;
}
void WithFifteenThetas::benchmarkResolve() {
        startBenchmark();
        resolve();
        endBenchmark();
}
