################################################################################
# lab9-3 : xor by neural nerwork wide deep
#          
# Hypothesis:
#  [[7.8051252e-04]
#  [9.9923813e-01]
#  [9.9837923e-01]
#  [1.5565872e-03]] 
# Correct:
#  [[0.]
#  [1.]
#  [1.]
#  [0.]] 
# Accuracy:
# 1.0
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import random

tf.set_random_seed(777) # for reproducibility
learning_rate = 0.1    

x_data = [[0, 0],
          [0, 1],
          [1, 0],
          [1, 1]]
y_data = [[0],
          [1],
          [1],
          [0]]

x_data = np.array(x_data, dtype=np.float32)
y_data = np.array(y_data, dtype=np.float32)

X = tf.placeholder(tf.float32, shape=[None, 2])
Y = tf.placeholder(tf.float32, shape=[None, 1])

'''
W = tf.Variable(tf.random_normal([2, 1], name='weight'))
b = tf.Variable(tf.random_normal([1], name='bias'))

# Hyphthesis using sigmoid: tf.div(1., 1. + tf.exp(tf.matmul(X, W)))
hypothesis = tf.sigmoid(tf.matmul(X, W) + b)
'''
'''
W1 = tf.Variable(tf.random_normal([2, 2], name='weight1'))
b1 = tf.Variable(tf.random_normal([2], name='bias1'))
layer1 = tf.sigmoid(tf.matmul(X, W1) + b1)

W2 = tf.Variable(tf.random_normal([2, 1], name='weight2'))
b2 = tf.Variable(tf.random_normal([1], name='bias2'))
hypothesis = tf.sigmoid(tf.matmul(layer1, W2) + b2)
'''
W1 = tf.Variable(tf.random_normal([2, 10], name='weight1'))
b1 = tf.Variable(tf.random_normal([10], name='bias1'))
layer1 = tf.sigmoid(tf.matmul(X, W1) + b1)

W2 = tf.Variable(tf.random_normal([10, 10], name='weight2'))
b2 = tf.Variable(tf.random_normal([10], name='bias2'))
layer2 = tf.sigmoid(tf.matmul(layer1, W2) + b2)

W3 = tf.Variable(tf.random_normal([10, 10], name='weight1'))
b3 = tf.Variable(tf.random_normal([10], name='bias1'))
layer3 = tf.sigmoid(tf.matmul(layer2, W3) + b3)

W4 = tf.Variable(tf.random_normal([10, 1], name='weight2'))
b4 = tf.Variable(tf.random_normal([1], name='bias2'))
hypothesis = tf.sigmoid(tf.matmul(layer3, W4) + b4)

# Cost / loss function
cost = -tf.reduce_mean(Y * tf.log(hypothesis) + (1 - Y) *
    tf.log(1 - hypothesis))

train = tf.train.GradientDescentOptimizer(learning_rate=learning_rate).minimize(cost)

# Accuracy computation
# True if hypothesis > 0.5 else False
predicted = tf.cast(hypothesis > 0.5, dtype=tf.float32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(predicted, Y), dtype=tf.float32))

# Launch graph
with tf.Session() as sess:
    # Initialize tensorflow variables
    sess.run(tf.global_variables_initializer())

    for step in range(10001):
        sess.run(train, feed_dict={X: x_data, Y: y_data})
        if step % 5000 == 0:
            print(step, sess.run(cost, feed_dict={X: x_data, Y: y_data}),
                sess.run([W1, W2, W3, W4]))
    
    # Accuracy report
    h, c, a = sess.run([hypothesis, predicted, accuracy], feed_dict={
        X: x_data, Y: y_data})
    print("\nHypothesis:\n", h, "\nCorrect:\n", c, "\nAccuracy:\n", a)
