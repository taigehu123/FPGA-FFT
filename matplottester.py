import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import math
import sys

def plot(a, b, c, sample_size):

	x = np.arange(0, sample_size)

	initial = np.array([])
	result = np.array([])
	inter = np.array([])

	with open(a, 'r') as f:
		line = f.readline()
		while line:
			initial = np.append(initial, float(line))
			line = f.readline()

	with open(b, 'r') as h:
		line = h.readline()
		while line:
			inter = np.append(inter, float(line))
			line = h.readline()


	with open(c, 'r') as g:
		line = g.readline()
		while line:
			result = np.append(result, float(line))
			line = g.readline()

	plt.figure(1)
	plt.subplot(311)
	plt.plot(x, initial)

	plt.subplot(312)
	
	plt.plot(x, inter)

	plt.subplot(313)
	plt.plot(x, result)
	plt.show()

def main(argv):
	plot(argv[1], argv[2], argv[3], int(argv[4]))

if __name__ == "__main__":
	main(sys.argv)