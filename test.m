a = [1 3 5 7 9 11 13 15 17 19];
shift = zeroes(10);
Mx = 10;
Tx = 5.5;
for i=1:10         
     shift(i)=  exp(i*2*pi*Tx*(i + Tx)*(1)/(Mx));
end

d = a.* shift;
print(d);

