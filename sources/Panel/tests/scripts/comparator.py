counter = 4999900
interpol1 = 55553
cal1 = 1660
interpol2 = 5972
cal2 = 1660

if (interpol1 & (1 << 15)) != 0:
    interpol1 -= 65536

if (interpol2 & (1 << 15)) != 0:
    interpol2 -= 65536

N = 5000000

is10s = False

if is10s:
    N = N * 10

k1 = interpol1
k1 /= cal1

k2 = interpol2
k2 /= cal2

dx = k1
dx -= k2
dx /= 2

A = N - counter

A -= dx
A /= N
A *= 1000000

if is10s:
    A *= 10;
    
print (A)
