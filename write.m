y = load('results.mat');
a = y.I_Out;
sz = size(a);

fid = fopen('new.txt', 'wt');
for i = 1:sz(1) 
    for j = 1:sz(2)
        n = real(a(i, j));
        k = imag(a(i, j));
        fprintf(fid,'%f\n',n);
    end
end

        