import matplotlib.pyplot as plt
import numpy as np

N = 10
ind = np.arange(N)
width = 0.5

def cumul(data):
  ys = [data[0]]
  for i in xrange(1, len(data)):
    ys.append(ys[i-1] + data[i])
  return ys

pct = [55.5362, 5.05362, 3.31099, 1.84987, 2.05094, 1.26005, 1.26005, 1.15282, 0.924933, 0.670241]
xs = [x / 100.0 for x in pct]
cs = cumul(xs)

plt.bar(ind + width / 2.0 + 0.5, xs, width, label="Probability of success", color='black')
plt.plot([1,2,3,4,5,6,7,8,9,10], cs, linewidth=5, linestyle='-', c='grey', solid_capstyle='round', label="Cumulative probabilities")
plt.axis([0, N + 1, 0, 0.9])
plt.legend(loc=2, ncol=1)

# Hack hack hack
t = plt.title('Finding the missing interaction with $K$NN/Tanimoto')
t.set_y(1.09)
plt.subplots_adjust(top=0.86)

plt.xlabel('Number of recommendations')
plt.ylabel('Probability of finding the missing interaction')
plt.savefig('./Figures/tanimoto.svg', bbox_inches='tight')
