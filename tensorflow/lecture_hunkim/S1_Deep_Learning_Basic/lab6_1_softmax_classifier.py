################################################################################
# lab6-1 : Softmax Classifier
# 
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

tf.set_random_seed(777) # for reproducibility

x_data = [[1, 2, 1, 1],
          [2, 1, 3, 2],
          [3, 1, 3, 4],
          [4, 1, 5, 5],
          [1, 7, 5, 5],
          [1, 2, 5, 6],
          [1, 6, 6, 6],
          [1, 7, 7, 7]]
y_data = [[0, 0, 1],
          [0, 0, 1],
          [0, 0, 1],
          [0, 1, 0],
          [0, 1, 0],
          [0, 1, 0],
          [1, 0, 0],
          [1, 0, 0]]          


# Placeholder for a tensor that will be always fed
x = tf.placeholder(tf.float32, shape=[None, 4])
y = tf.placeholder(tf.float32, shape=[None, 3])
nb_classes = 3

# Model parameters
W = tf.Variable(tf.random_normal([4, nb_classes]), name='weight')
b = tf.Variable(tf.random_normal([nb_classes]), name='bias')

# tf.nn.softmax computes softmax activations
# softmax = exp(logits) / reduce_sum(exp(logits), dim)
hypothesis = tf.nn.softmax(tf.matmul(x, W) + b)

# Cross entropy cost/loss function
cost = tf.reduce_mean(-tf.reduce_sum(y * tf.log(hypothesis), axis=1))

# Minimize
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.1).minimize(cost)

# launch the graph in a session
with tf.Session() as sess:
    # Initialize TensorFlow variables
    sess.run(tf.global_variables_initializer())
    
    for step in range(10001):
        sess.run(optimizer, feed_dict={x: x_data, y: y_data})
        if step % 5000 == 0:
            print(step, sess.run(cost, feed_dict={x: x_data, y: y_data}))

    # Testing & One-hot encoding
    print("------------------------------------------------------------------")
    a = sess.run(hypothesis, feed_dict={x: [[1, 11, 7, 9]]})
    print("a: ", a, sess.run(tf.argmax(a, 1)))

    print("------------------------------------------------------------------")
    b = sess.run(hypothesis, feed_dict={x: [[1, 3, 4, 3]]})
    print("b: ", b, sess.run(tf.argmax(b, 1)))

    print("------------------------------------------------------------------")
    c = sess.run(hypothesis, feed_dict={x: [[1, 1, 0, 1]]})
    print("c: ", c, sess.run(tf.argmax(c, 1)))

    print("------------------------------------------------------------------")
    tot = sess.run(hypothesis, feed_dict={
        x: [[1, 11, 7, 9], [1, 3, 4, 3], [1, 1, 0, 1]]})
    print("tot: ", tot, sess.run(tf.argmax(tot, 1)))
