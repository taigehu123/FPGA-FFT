function [out_wf] = f_2D_free_prop_spectr_jac(dx,dy,Z,ut0)
%  Spectral/Fourier Wavefront  Propagation Algorithm
% ==Inputs==
% dx, dy space steps of the uto matrix
% Z = propagation distance
% ut0 = Input field in the space domain
%works for odd matrix dimensions
% ==Outputs==

%out_wf = Output field in space domain
% written by Jacek Krzywinski

%=============================================================
% Propagation , 0 to Z
%=============================================================

[Mx,My] = size(ut0);
Tx=(Mx+1)/2; Ty= (My+1)/2;

%======================
% shifting matrix ph
%======================
ph = zeros(Mx,My);
for ii=1:Mx         
    for jj = 1:My 

        ph(ii,jj)=  exp(i*2*pi*Tx*(ii+jj+Tx)*(-1)/(Mx));  
       
    end
end
%======================
% unshifting matrix ph_p
%======================

ph_p = zeros(Mx,My);
for ii=1:Mx         
    for jj = 1:My 

        ph_p(ii,jj)=  exp(i*2*pi*Tx*(ii+jj+Tx)*(1)/(Mx)); 
       
    end
end
%============================
dkx = 2*pi/(Mx*dx);
dky = 2*pi/(My*dy);

%======================
% FFT of the input field and
% shift  - moving the zero-frequency component to the center of the array
%======================


fwf0 = ph.*(fft2(ut0.*ph));
%===========================

%===================================
% calculating square of  k-vector 
%==================================

k_sqr = zeros(Mx,My);

for ii=1:Mx         
    for jj = 1:My

        k_sqr(ii,jj)=  ((ii-Tx)*dkx)^2 + ((jj-Ty)*dky)^2;  % w_sqr=s freq square matrix
       
    end
end

%===================================
% Propagator 
%==================================

Dh = exp(Z.*(-i/2).*k_sqr);
%figure;
%pr = k_sqr(-Tx,1:Mx-1)*Z/2/pi;
%plot(pr)
%===================================
% Propagator * shifted Fourier transform of the input field
%==================================
wf_prop = Dh.*fwf0;
%===================================
% Shifted Inverse Fourier transform 
%  moving the zero-frequency component to the center of the array-> Output field
%==================================

out_wf = ph_p.*(ifft2(wf_prop.*ph_p));