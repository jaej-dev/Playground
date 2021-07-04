################################################################################
# lab3-2 : Linear Regression Minizing cost
#          Gradient Descent Algorithm
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

x_data = [1, 2, 3]
y_data = [1, 2, 3]

# Model parameters
W = tf.Variable(tf.random_normal([1]), name='weight')

# Model input and output
x = tf.placeholder(tf.float32)
y = tf.placeholder(tf.float32)

# Our hypothesis
linear_model = x * W

# cost/loss function
cost = tf.reduce_sum(tf.square(linear_model - y))  # sum of the squares

# Minimize: Gradient Descent using derivative: W -= learning_rate * deriavtive
learning_rate = 0.1
gradient = tf.reduce_mean((W * x - y) * x)
descent = W - learning_rate * gradient
update = W.assign(descent)

# launch the graph in a session
init = tf.global_variables_initializer()    # over rev 1.0 api
sess = tf.Session()
sess.run(init)  # reset values to wrong

for step in range(21):
    sess.run(update, feed_dict={x: x_data, y: y_data})
    print(step, sess.run(cost, feed_dict={x: x_data, y: y_data}), sess.run(W))
