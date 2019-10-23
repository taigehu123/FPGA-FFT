import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import math
import sys


with open('zcu102_fft2d.txt') as file:
	array = []

	for line in file:
		curr = [float(x) for x in line.split()]
		curr.pop(0)
		array.append(curr)

to_write = open('2matrix.txt', 'w')
for i in array:
	for j in i:
		to_write.write(str(j) + "\n")