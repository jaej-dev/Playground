################################################################################
# lab3-3 : Linear Regression Minizing cost
#          Gradient Descent Algorithm via tensorflow api
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

x_data = [1, 2, 3]
y_data = [1, 2, 3]

# Model parameters
#W = tf.Variable(tf.random_normal([1]), name='weight')
W = tf.Variable(5.0)
#W = tf.Variable(-3.0)

# Model input and output
x = tf.placeholder(tf.float32)
y = tf.placeholder(tf.float32)

# Our hypothesis
linear_model = x * W

# cost/loss function
cost = tf.reduce_mean(tf.square(linear_model - y))

# Minimize: Gradient Descent using derivative: W -= learning_rate * deriavtive
learning_rate = 0.1
#gradient = tf.reduce_mean((W * x - y) * x)
#descent = W - learning_rate * gradient
#update = W.assign(descent)
optimizer = tf.train.GradientDescentOptimizer(learning_rate)
train = optimizer.minimize(cost)

# launch the graph in a session
init = tf.global_variables_initializer()    # over rev 1.0 api
sess = tf.Session()
sess.run(init)  # reset values to wrong

for step in range(21):
    print(step, sess.run(W))
    sess.run(train, feed_dict={x: x_data, y: y_data})
