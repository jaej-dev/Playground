################################################################################
# lab7-1 : Learning rate and evalutation
#          
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

# for reproducibility
tf.set_random_seed(777) 

# Predicting animal type based on various features
x_data = [[1, 2, 1],
          [1, 3, 2],
          [1, 3, 4],
          [1, 5, 5],
          [1, 7, 5],
          [1, 2, 5],
          [1, 6, 6],
          [1, 7, 7]]

y_data = [[0, 0, 1],
          [0, 0, 1],
          [0, 0, 1],
          [0, 1, 0],
          [0, 1, 0],
          [0, 1, 0],
          [1, 0, 0],
          [1, 0, 0]]

# Evaluation our model using this test dataset
x_test = [[2, 1, 1],
          [3, 1, 2],
          [3, 3, 4]]

y_test = [[0, 0, 1],
          [0, 0, 1],
          [0, 0, 1]]


x = tf.placeholder(tf.float32, shape=[None, 3])
y = tf.placeholder(tf.float32, shape=[None, 3])

W = tf.Variable(tf.random_normal([3, 3]))
b = tf.Variable(tf.random_normal([3]))

# tf.nn.softmax cmputes softmax activatins
# softmax = exp(logits) / reduce_mean(exp(logits), dim)
hypothesis = tf.nn.softmax(tf.matmul(x, W) + b)

# Cross entropy cost/loss
cost = tf.reduce_mean(-tf.reduce_sum(y * tf.log(hypothesis), axis=1))

# Try to change learning_rate to small numbers
optimizer = tf.train.GradientDescentOptimizer(
#    learning_rate = 1e-10).minimize(cost)   # case1 1e-10
#    learning_rate = 10.0).minimize(cost)   # case2 10.0
    learning_rate = 0.1).minimize(cost)   # case3 0.1

# Correct prediction Test model
prediction = tf.arg_max(hypothesis, 1)
is_correct = tf.equal(prediction, tf.arg_max(y, 1))
accuracy = tf.reduce_mean(tf.cast(is_correct, tf.float32))

# Launch graph
with tf.Session() as sess:
    # Initialize TensorFlow variables
    sess.run(tf.global_variables_initializer())

    for step in range(1001):
        cost_val, W_val, _ = sess.run(
            [cost, W, optimizer], feed_dict={x: x_data, y: y_data})
        if step % 100 == 0:
            print(step, cost_val, W_val)

    # Pridict
    print("Prediction: ", sess.run(prediction, feed_dict={x: x_test}))

    # Calcuate the accuracy
    print("Accuracy: ", sess.run(accuracy, feed_dict={x: x_test, y: y_test}))
