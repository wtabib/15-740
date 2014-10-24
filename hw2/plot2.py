#!/usr/bin/env python

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib import mlab
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np


f = open('output_matrix_12_ghc.txt', 'r')
X = []
Y = []
for l in f:
    tmp = l.split(",")
    X.append(float(tmp[0]))
    Y.append(float(tmp[1]))

fig = plt.figure()
h1, = plt.plot(X, Y, 'r+', label='12 threads')

f = open('output_matrix_6_ghc.txt', 'r')
X = []
Y = []
for l in f:
    tmp = l.split(",")
    X.append(float(tmp[0]))
    Y.append(float(tmp[1]))
h2, = plt.plot(X, Y, 'b*', label='6 threads')

plt.legend([h1, h2], ["12 threads", "6 threads"])

plt.show()

