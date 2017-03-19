import os.path
from sklearn.ensemble import RandomForestClassifier
from sklearn import svm
import numpy as np
from random import random as rng
import time

t = int(time.time())

testing0 = open('testing0-' + str(t) + '.db', 'w')
testing1 = open('testing1-' + str(t) + '.db', 'w')
training = open('training-' + str(t) + '.db', 'w')

test0Count = 0
test1Count = 0
trainingCount = 0

with open('allinteractions.db') as f:
  for line in f:
    l = line.replace(' ', '').split(',')
    # Only keep y (l[0]) and the three real-valued traits of both species.
    entry = l[0] + ', ' + l[1] + ', ' + l[6] + ', ' + l[7] + ', ' + l[8] + ', ' + l[17] + ', ' + l[18];
    if rng() < 0.05:
      if line[0] == '0':
        testing0.write(entry)
        test0Count += 1
      elif line[0] == '1':
        testing1.write(entry)
        test1Count += 1
      else:
        print("Can't figure out the inter for line " + line)
    else:
      training.write(entry)
      trainingCount += 1

testing0.close()
testing1.close()
training.close()

testCount = test1Count + test0Count
print('Training count: ' + str(trainingCount))
print('Test count: ' + str(testCount))
print('Test 0 count: ' + str(test0Count))
print('Test 1 count: ' + str(test1Count))
print('Test/training ratio: ' + str(float(testCount) / trainingCount));
print('Total: ' + str(testCount + trainingCount))

# Load training data:

training_data = np.loadtxt('./training-' + str(t) + '.db', delimiter=',')

X = training_data[:,1:]
y = training_data[:,0]

# Load testing data:

testing_data0 = np.loadtxt('testing0-' + str(t) + '.db', delimiter=',')
testing_data1 = np.loadtxt('testing1-' + str(t) + '.db', delimiter=',')

test0X = testing_data0[:,1:]
test0y = testing_data0[:,0]

test1X = testing_data1[:,1:]
test1y = testing_data1[:,0]

# Functions to train & score:

def score_and_show(model, name):
  """Show results for testing datasets."""
  print(name)
  print("+------------------------------------------+")
  score1 = model.score(test1X, test1y)
  score0 = model.score(test0X, test0y)
  print("Interactions         " + str(score1))
  print("Non-interactions     " + str(score0))
  print("Accuracy             " + str((score0 * test0Count + score1 * test1Count) / testCount))
  a, b, c, d = score1 * test1Count, score0 * test0Count, (1.0 - score1) * test1Count, (1.0 - score0) * test0Count
  tss = (a * b - c * d) / ((a + c) * (b + d))
  print("TSS                  " + str(tss))

# Train random forests with the best parameters found by randomized search

rf = RandomForestClassifier(n_estimators = 100, bootstrap = True, min_samples_leaf = 10, criterion = 'entropy', max_depth = None)
rf.fit(X, y)
score_and_show(rf, "\nRandom Forest")
