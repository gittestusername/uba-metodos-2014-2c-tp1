function f = getCoefficients(dr, dt, r_i, j)
    r = r_i + (j-1)*dr;
    c_1 = 1/(r*dr) - 2/(dr^2) - 2/((r*dt)^2);
    c_2 = 1/(dr^2) - 1/(r*dr);
    c_3 = 1/(dr^2);
    c_4 = 1/((r*dt)^2);
f = [c_1, c_2, c_3, c_4];
