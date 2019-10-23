% clear all
% close all
En=1400 %[eV]
lambda=1.24/En*1E-6 % X-ray wavelegth [m]
k0=2*pi/lambda; %wave vector number


%window size

Fsize=3.9*1E-3;


% Load field
b=load('Esum14000eV');
Efield1400=b.Efieldsum;
sizM=size(Efield1400);
Mx=sizM(1);My=sizM(2);

%step size and x,y coordinates in dimensionless units

dx=Fsize*k0/Mx; dy=Fsize*k0/My;
x = dx*((1:Mx)-Mx/2); y = dy*((1:Mx)-Mx/2);
[Y,Xj] = meshgrid(y,x);

%  Propagation distance

zU_S= 73.8230*k0; %distance undulator  -  slit 
 
%Propagation

%Electric field

[E_Out] =  f_2D_free_prop_spectr(dx,dy,zU_S,Efield1400);

%intensity
% source 

I_Source=abs(Efield1400).^2;

%after propagation 

I_Out=abs(E_Out);
save('results', 'I_Out');
 
figure

positionVector1 = [0.05, 0.3, 0.4, 0.5];
subplot('Position',positionVector1)
plot(y)

positionVector2 = [0.5, 0.3, 0.4, 0.5];
subplot('Position',positionVector2)
bar(y)

%display results

 figure
subplot('Position',positionVector1 ) 

imagesc(y/k0,x/k0,I_Source)

axis([-1e-3 1e-3 -1e-3 1e-3 ])
title('FEL intensity the undulator exit')
subplot('Position',positionVector2 ) 
imagesc(y/k0,x/k0,I_Out)

axis([-2e-3 2e-3 -2e-3 2e-3 ])
title('FEL intensity at the slit')