import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import math
import sys

def plot(a):

	aa = np.loadtxt(a)
	f = aa.reshape((1024,1024))

	


	plt.imshow(f)
	
	plt.show()

def main(argv):
	plot(argv[1])

if __name__ == "__main__":
	main(sys.argv)