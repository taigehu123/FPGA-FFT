function [out_wf] = f_2D_free_prop_spectr(dx,dy,Z,ut0)
%  Angular Spectrum /Fourier Wavefront Propagation Algorithm
% ==Inputs==
% ut0 = Input field in the space domain
% dx, dy space steps of the ut0 matrix
% Z = propagation distance


% ==Outputs==

%out_wf = Output field in space domain
% written by Jacek Krzywinski


% Matrix dimenssions 

sizM=size(ut0);
Mx=sizM(1);My=sizM(2);

%======================
% FFT of the input field 
% and a shift  - moving the zero-frequency component to the center of the array
%======================
fwf0 = fftshift(fft2(ifftshift(ut0)));
%===========================
%===================================
% calculating the product of square of  k-vector components

dkx = 2*pi/(Mx*dx);
dky = 2*pi/(My*dy);

kx = dkx*((1:Mx)-Mx/2); ky = dky*((1:My)-My/2); 
[KY,KX] = meshgrid(ky,kx);

k_sqr=((KX).^2+(KY).^2)*Z;

%===================================
% Propagator 
%==================================

Dh = exp((-1i/2).*k_sqr);

%===================================
% Propagator * shifted Fourier transform of the "flat" input field
%==================================
wf_prop = Dh.*fwf0;

%===================================
% Shifted Inverse Fourier transform 
%  moving the zero-frequency component to the center of the array-> Output field
%==================================
%===================================
% the output field 
%==================================

out_wf = fftshift((ifft2(ifftshift(wf_prop))));

