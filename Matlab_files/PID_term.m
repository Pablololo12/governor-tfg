clear all

Ti = 100;
Td = 5;
K = 10000;
alfa = 0.9;

s = tf('s');
C = K*(Ti*s+1)/Ti/s*(Td*s+1)/(alfa*Td*s+1)

Ts = 1;
Cd = c2d(C,Ts)
[num,den] = tfdata(Cd);
A = num{1}(1)
B = num{1}(2)
C = num{1}(3)
D = den{1}(1)
E = den{1}(2)
F = den{1}(3)

