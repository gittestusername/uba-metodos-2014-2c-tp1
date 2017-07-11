#define _USE_MATH_DEFINES
#include <fstream>
#include <istream>
#include <iostream>
#include <vector>
#include <math.h>

#include "Matrix.h"
#include "WithFifteenThetas.h"

using namespace std;

#ifndef TEST

// Main Release
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "\nEl uso correcto es ./tp <archivo_entrada> <archivo_salida> <modo> <archivo_isoterma>." << 
            " En donde modo puede ser: 0. EG 1. LU.\n\n";
        return 0;
    }
        
    // Abrimos los archivos
    string input_filename = argv[1];
    ifstream input_file;
    input_file.open(input_filename.c_str());
    
    string output_filename = argv[2];
    ofstream output_file;
    output_file.open(output_filename.c_str());
    
    string mode = argv[3];

    // Ajustamos precision del archivo de salida a 6 decimales fixed
    // Puede ir hasta 15
    output_file.precision(6);
    output_file.setf( std::ios::fixed, std:: ios::floatfield );
    
    // Inicializacion de instancia
    WithFifteenThetas instance;
    if (mode == "1") instance.toggleLU();
    int ninst = 0;
    instance.load(input_file, ninst);

    // Nos fijamos si tenemos el parametro del archivo de isoterma.
    ofstream isoterm_file;
    if (argc >= 5) isoterm_file.open(argv[4]);

    for (int i = 0; i < ninst; i++) {
        instance.loadData(input_file);
        instance.resolve();
//		cout << instance.m << " " << instance.n << " " << instance.m + instance.n<< " " << instance.getBenchmarkResult() << endl;
        instance.printResult(output_file);
        if (argc >= 5) instance.printIsoterma(isoterm_file);
        instance.reset();
    }
    
    // Cerramos los archivos
    input_file.close();
    output_file.close();
    if (argc >= 5) isoterm_file.close();

    return 0;
}
#endif

#ifdef TEST
// Main Test
int main() {
    
    // Inicializamos
    WithFifteenThetas instance;
    
    // Ajustamos precision del cout a 6 decimales fixed
    // Puede ir hasta 15
    cout.precision(6);
    cout.setf( std::ios::fixed, std:: ios::floatfield );
    
    /* Ejemplo inventado */
    vector<double> T_i_1 = {1500, 1500, 1500};
    vector<double> T_e_1 = {150, 100, 150};
    instance.load(10, 40, 5, 3, 500, T_i_1, T_e_1);
    instance.benchmarkResolve();
    cout << "\nImprimiendo el resultado del problema 1.\n\n";
    instance.printResult(cout);
    cout << endl;
    cout << "\nLa resolucion insumio " << instance.getBenchmarkResult() << 
        " ciclos de clock." << endl;

    instance.wipe();
    

    /* Caso de test 3 de la catedra */
    vector<double> T_i_2 = {1500, 1500, 1500, 1500, 1500, 
                        1500, 1500, 1500, 1500, 1500};
    vector<double> T_e_2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    instance.load(1, 2, 30, 10, 500, T_i_2, T_e_2);
    instance.benchmarkResolve();
    cout << "\nImprimiendo el resultado del problema 2.\n\n";
    instance.printResult(cout);
    cout << endl;
    cout << "\nLa resolucion insumio " << instance.getBenchmarkResult() << 
        " ciclos de clock." << endl;
        
    instance.wipe();
    
    /* Caso de test 3 de la catedra desde archivo */
    string file_name = "tests/test3.in";
    int ninst = 0;
    ifstream input_file;
    cout << "\nCargando instancia desde archivo... ";
    input_file.open(file_name.c_str());
    instance.load(input_file, ninst);
    cout << "Se encontraron " << ninst << " instancia/s de datos. Resolviendo." << endl;
    instance.loadData(input_file);
    instance.benchmarkResolve();
    cout << "Imprimiendo el resultado del problema 3 desde archivo.\n\n";
    instance.printResult(cout);
    cout << endl;
    cout << "\nLa resolucion insumio " << instance.getBenchmarkResult() << 
        " ciclos de clock." << endl;
    input_file.close();

    return 0;
}
#endif
