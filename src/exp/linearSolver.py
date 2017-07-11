#!/usr/bin/python
import numpy as np
import sys as systemOS

# Algoritmo de eliminacion de Gauss sin rotacion de filas
# Convertir la matriz en diagonal inferior, sabiendo que a_{i,j} != 0 \forall{i,j \in [[1,n]x[1,m]]}
def gaussianElimination(A,b):
    for i in range(0,A.shape[0]-1):                 # i recorre filas
        for z in range(i+1,A.shape[0]):             # z recorre filas comenzando desde i+1 
            if A[i,i] == 0:
                print "\n\n Zero in diagonal found. \n"
                print A
                exit()
            coef = (1.*A[z,i])/(1.*A[i,i])          # Como es cuadrada el elemento (i,i) es el de la diagonal
            A[z] = A[z] - coef*A[i]                 # Le restamos a la fila z el coeficiente por la fila i para dejar un 0 en el lugar (z,i)
            b[z] = b[z] - coef*b[i]                 # Hacemos lo mismo con el vector resultado

def vecDot(i, A, b):
    result = 0.0
    for j in range(0,A.shape[1]):
        result = result + A[i,j] * b[j]
    return result

def linearSolver(sys):
    A = sys[0].copy()
    B = sys[1].copy()
    R = np.zeros(shape=B.shape)
    gaussianElimination(A,B)
    
    # Resolucion de matriz diagonal 
    # Ejecutamos los reemplazos para calcular la solucion del sistema
    for i in range(0,A.shape[0]):
        i = A.shape[0]-i-1                                     # Empiezo por el ultimo
        if (A[i] == 0).all():                                  # Si la fila i son ceros me fijo si en B tambien es cero
            if B[i,0] != 0:
                print "\n\n The given system is inconsistent. \n"
                exit()
            else:
                continue
        else:
            const = vecDot(i, A, R)                        # Averiguo la constante mutliplicando la fila i por la resolucion parcial
            R[i] = (1.*(B[i]-const))/(1.*A[i,i])
    
    return R

# Genera un sistema random sin ceros en la diagonal con A de tamanio NxN
def generateRandomSystem(n):
    max_value = 1000
    
    A = np.matrix(np.random.random_integers(0,max_value,n*n),dtype=float).reshape(n,n)
    B = np.matrix(np.random.random_integers(0,max_value,n),dtype=float).transpose()
    
    for i in range(0,A.shape[0]):
        for j in range(0,A.shape[1]):
            A[i,j] = np.random.random_integers(1,max_value)
    
    return [A,B]

def testSolver(system_size, n_tests):
    print "[ Running", n_tests ,"random tests of size",system_size,"x",system_size,"]:",
    for i in range(0,n_tests):
        sys = generateRandomSystem(system_size)
        R = linearSolver(sys)
        if (sys[0]*R == sys[1]).all:
            print i+1,
            systemOS.stdout.flush()
        else:
            print "\n\nThe solver failed at system number", i+1, ":"
            print "Matrix A\n"
            print sys[0]
            print "\nMatrix B\n"
            print sys[1]
            return
    print "\n\nAll random tests were resolved successfully, yay!\n"
    
def getCoefficients(dr, dt, r_i, j):
    r = r_i + (j-1)*dr
    c_1 = 1/(r*dr) - 2/pow(dr,2) - 2/pow(r*dt,2)
    c_2 = 1/pow(dr,2) - 1/(r*dr)
    c_3 = 1/pow(dr,2)
    c_4 = 1/pow(r*dt,2)
    return [c_1, c_2, c_3, c_4]

# r_i = radio interno, r_e = radio externo,
# m+1 = discretizacion radios, n = discretizacion angulos,
# isovalue = isoterma buscada, T_i = temperaturas internas
# T_e = temperaturas externas
def withFifteenThetas(r_i, r_e, m, n, isovalue, T_i, T_e):
    # Calculo los delta
    dr = 1.*(r_e-r_i)/(m-1)
    dt = 2.0*np.pi/n
    
    # Inicializo B
    B = np.zeros(shape=(n*m,1))
    for i in range(0,n):
        B[i,0] = T_i[i]
        B[n*m-1-i,0] = T_e[i]
    
    # Lleno A
    A = np.zeros(shape=(n*m,n*m))   
    j = 1
    k = n+1
    C = [0,0,0,0]
    for i in range(0,n*m):
        if (i < n) or (i >= (n*m-n)):       # Area identidad de A
            A[i,i] = 1
        else:
            # Actualizamos los indices y las constantes
            if k > n:
                j = j+1
                k = 1
                C = getCoefficients(dr, dt, r_i, j)
            
            # t_{j,k}
            A[i,i] = C[0]
            
            # t_{j+1,k}, t_{j-1,k}
            A[i,i-n] = C[1]
            A[i,i+n] = C[2]
            
            # t_{j,k+1}
            if k == n:
                A[i,i-n+1] = C[3]
            else:
                A[i,i+1] = C[3]
                
            # t_{j,k-1}
            if k == 1:
                A[i,i+n-1] = C[3]
            else:
                A[i,i-1] = C[3]
            
            k = k+1
    
    # Resuelvo el sistema
    #print np.linalg.solve(A,B)
    print linearSolver([A,B])
    
    return


#withFifteenThetas(3, 6, 3, 3, 500, [1500, 1500, 1500], [150, 100, 150])

# Test 3
#withFifteenThetas(1, 2, 30, 10, 500, [1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

# Test 1
withFifteenThetas(10, 100, 30, 30, 500, [1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    
       
#testSolver(10, 1000)

# Sistemas hechos a mano
# El resultado del siguiente sistema deberia ser [0,0,1/3]    
#sys = [np.matrix([[1,2,3],[4,7,6],[7,8,9]],dtype=float), np.matrix([[1],[2],[3]],dtype=float)]
#print linearSolver(sys)

# La solucin del siguiente sistema es [1,1,X]
#sys = [np.matrix([[1,0,2],[0,1,0],[1,0,2]],dtype=float), np.matrix([[1],[1],[1]],dtype=float)]
#print linearSolver(sys)

# El siguiente sistema no tiene solucion (los vectores son LD)    
#sys = [np.matrix([[1,0,2],[0,1,0],[1,0,2]],dtype=float), np.matrix([[1],[1],[3]],dtype=float)]
#print linearSolver(sys)


