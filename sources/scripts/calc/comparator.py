N = 5000000
counter = 4999997

#int1 = 4839
#int2 = 4757

int1 = 4757
int2 = 4734

cal1 = 1661
cal2 = 1661

dx = (int1 / cal1 - int2 / cal2) / 2

A = (N - counter) / N - dx / N

print (A)
