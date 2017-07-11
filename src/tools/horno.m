function horno(inputfile, solution, isofile)

ifid = fopen(inputfile);
solfid = fopen(solution);
instparam = fscanf(ifid, '%f',6);
if (nargin > 2)
   isofid = fopen(isofile);
end

angulos = instparam(4);
radios = instparam(3);
radioi = instparam(1);
radioe = instparam(2);
puntos = angulos*radios;
caloriso = instparam(5);
ninst = instparam(6);
deltaAng = 2*pi/angulos;
deltaRad = (radioe-radioi)/(radios-1);

for k = 1:ninst
    %armado de b 
    b = zeros(puntos,1);
    baux = fscanf(ifid,'%f',2*angulos);
    b(1:angulos) = baux(1:angulos);
    b(end-angulos+1:end) = baux(angulos+1:end);
    
    x = fscanf(solfid,'%f',puntos);
    
    xdib = zeros(puntos,1);
    ydib = zeros(puntos,1);

    pis = linspace(0,2*pi,angulos+1);
    pis = pis(1:angulos);

    for i=1:radios
        radAct = radioi + deltaRad*(i-1); 
        for j=1:angulos
           xdib(idx(i,j,angulos)) = cos(pis(j))*radAct;
           ydib(idx(i,j,angulos)) = sin(pis(j))*radAct;
        end
    end

    %Aca ploteo el calor en el horno
    figure;
    xdib = reshape(xdib,angulos,radios);
    xdib(angulos+1,:) = xdib(1,:);
    ydib = reshape(ydib,angulos,radios);
    ydib(angulos+1,:) = ydib(1,:);
    x = reshape(x,angulos,radios);
    x(angulos+1,:) = x(1,:);
    h = pcolor(xdib,ydib,x);
    axis tight;

    %aca ploteo la isoterma
    if (nargin > 2)
        %figure;
        radioiso = fscanf(isofid, '%f',angulos);
        radioiso = [radioiso; radioiso(1)]; % para que se hace esto?
        radioiso = radioiso';


        theta=linspace(0,2*pi,angulos+1); 
        % ^ theta es usado para saber en que coordinadas va cada punto.
        % esta compuesto por numeros angulos + 1 numeros separados linealmente entre 0 y 2 * pi.
        interno=linspace(radioi,radioi,angulos+1);
        externo=linspace(radioe,radioe,angulos+1);
        % interno y externo son usados para plotear las paredes del horno.
        hold on; % hold on es usado para que se grafique en la misma figura donde se grafica la pared interna.
        liso = polar(theta,radioiso, 'r');
        polar(theta,interno);
        polar(theta,externo); % dibujamos ultimo el radio externo para que octave haga zoom en el
        set(liso, 'linewidth', 2.5);
        hold off;
        %axis tight;
    end
end
