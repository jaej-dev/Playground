################################################################################
# lab5-1 : Logistic Regression Classifier
# 
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

tf.set_random_seed(777) # for reproducibility

x_data = [[1, 2],
          [2, 3],
          [3, 1],
          [4, 3],
          [5, 3],
          [6, 2]]
y_data = [[0],
          [0],
          [0],
          [1],
          [1],
          [1]]          


# Placeholder for a tensor that will be always fed
x = tf.placeholder(tf.float32, shape=[None, 2])
y = tf.placeholder(tf.float32, shape=[None, 1])

# Model parameters
W = tf.Variable(tf.random_normal([2, 1]), name='weight')
b = tf.Variable(tf.random_normal([1]), name='bias')

# hypothesis using sigmoid: tf.div(1., 1. + tf.exp(tf.matmul(x, W)))
hypothesis = tf.sigmoid(tf.matmul(x, W) + b)

# cost/loss function
cost = -tf.reduce_mean(y * tf.log(hypothesis) + (1 - y) * 
                      tf.log(1 - hypothesis))

# Minimize
train =  tf.train.GradientDescentOptimizer(learning_rate=0.01).minimize(cost)

# Accuracy computation
# True if hypothesis > 0.5 else False
predicted = tf.cast(hypothesis > 0.5, dtype=tf.float32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(predicted, y), dtype=tf.float32))

# launch the graph in a session
with tf.Session() as sess:
    # Initialize TensorFlow variables
    sess.run(tf.global_variables_initializer())
    
    for step in range(10001):
        cost_val, _ = sess.run([cost, train], feed_dict={x: x_data, y: y_data})
        if step % 5000 == 0:
            print(step, cost_val)

    # Accuracy report
    h, c, a = sess.run([hypothesis, predicted, accuracy], 
            feed_dict={x: x_data, y: y_data})
    print("\nHypothesis: ", h, "\nCorrect: ", c, "\nAccuracy: ", a)

