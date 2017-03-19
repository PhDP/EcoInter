import matplotlib.pyplot as plt
import numpy as np

N = 3
ind = np.arange(N)
width = 0.5

ax = plt.subplot()

#     n <= 10        10 < n <= 100,   n > 100
xs = [0.8078,       0.3706,           0.2403]

plt.bar(ind + width / 2.0 + 0.5, xs, width, color='green', align='center')
plt.xticks(ind + width / 2.0 + 0.5, ['$p \leq 10$', '$10 < p \leq 100$', '$100 < p$'])

# Set the tick labels font
for label in ax.get_xticklabels():
    label.set_fontsize(18)

plt.axis([0, N + 0.5, 0, 0.9])
plt.legend(loc=2, ncol=1)

plt.xlabel('Number of preys $p$')
plt.ylabel('Success on first guess')
plt.savefig('./Figures/tanimoto_n_preys.pdf', bbox_inches='tight')
