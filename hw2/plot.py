#!/usr/bin/env python


from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib import mlab
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np


#f = open('better.txt', 'r')
f = open('output_moutain_ghc.txt', 'r')
X = []
Y = []
Z = []
for l in f:
    tmp = l.split()
    X.append(float(tmp[0]))
    Y.append(float(tmp[1]))
    Z.append(float(tmp[2]))

fig = plt.figure()
ax = fig.gca(projection='3d')
X2, Y2 = np.meshgrid(X, Y)
Z2 = mlab.griddata(X, Y, Z, X2, Y2)


#print X2
#print Y2
#print Z2
#surf = ax.plot_surface(X2, Y2, Z2, rstride=1, cstride=1, cmap=cm.coolwarm,
#                               linewidth=0, antialiased=False)
#ax.set_zlim(-1.01, 1.01)

#ax.zaxis.set_major_locator(LinearLocator(10))
#ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

#fig.colorbar(surf, shrink=0.5, aspect=5)


ax.scatter(X, Y, Z)
plt.show()

