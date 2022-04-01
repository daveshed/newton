#! /usr/bin/env python

import matplotlib.pyplot as plt
import sys

def read_file(fn):
    X = []
    Y = []
    with open(fn) as f:
        line = f.readline().strip()
        while line:
            x, y = [int(i) for i in line.split('\t')]
            X.append(x)
            Y.append(y)
            line = f.readline().strip()
    X = [i - X[0] for i in X]
    return X, Y


for fn in sys.argv[1:]:
    data = read_file(fn)
    plt.plot(data[0], data[1], 'o:', label=fn)
plt.legend()
fig = plt.gcf()
fig.tight_layout()
plt.show()
