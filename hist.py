import matplotlib.pyplot as plt
import numpy as np
import sys


def plot(a, b):
	aa = np.loadtxt(a);
	bb = np.loadtxt(b);

	cc = aa - bb
	plt.hist(cc)
	plt.show()
	print(np.std(cc))
def main(argv):
	plot(argv[1], argv[2])

if __name__ == "__main__":
	main(sys.argv)