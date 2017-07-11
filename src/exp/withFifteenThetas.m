function f = withFifteenThetas(r_i, r_e, m, n, isovalue, T_i, T_e)
    % Calculo los delta
    % OBS: Si dividimos una parte en m partes iguales
    % vamos a tener m+1 "divisiones". En nuestro caso
    % cada division es un punto. Si dividimos
    % por m vamos a tener m+1 puntos por lo que tenemos
    % que dividir a la diferencia por m-1. El resto queda en m.
    dr = (r_e-r_i)/(m-1);
    dt = (2*pi)/n;

    % Inicializo B
    b = zeros(n*m,1);
    for i=1:n,
       b(i) = T_i(i);
       b(n*m-i+1) = T_e(i);
    end

    % Lleno A
    A = zeros(n*m,n*m);
    j = 1;
    k = n+1;
    C = [0 0 0 0];
    for i=1:n*m,
        if (i <= n) || (i > (n*m-n))
            A(i,i) = 1;
        else
            % Actualizamos los indices y las constantes
            % Aca entra recien en 2,1, con j=1 son las temp
            % de la pared.
            if k > n
                j = j+1;
                k = 1;
                C = getCoefficients(dr, dt, r_i, j);
            end
            
            % t_{j,k}
            A(i,i) = C(1);
            
            % t_{j+1,k}, t_{j-1,k}
            A(i,i-n) = C(2);
            A(i,i+n) = C(3);
            
            % t_{j,k+1}
            if k == n
                A(i,i-n+1) = C(4);
            else
                A(i,i+1) = C(4);
            end
                
            % t_{j,k-1}
            if k == 1
                A(i,i+n-1) = C(4);
            else
                A(i,i-1) = C(4);
            end
            
            k = k+1;
        end
    end
    %spy(A)
f = A\b;
