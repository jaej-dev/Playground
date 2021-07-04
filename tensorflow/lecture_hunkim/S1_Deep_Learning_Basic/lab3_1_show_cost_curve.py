################################################################################
# lab3-1 : Linear Regression Minizing cost
# 
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np


# Model parameters
W = tf.placeholder(tf.float32)

# Model input and output
x = [1, 2, 3]
y = [1, 2, 3]

# Our hypothesis
linear_model = x * W

# cost/loss function
cost = tf.reduce_sum(tf.square(linear_model - y))  # sum of the squares

# launch the graph in a session
init = tf.global_variables_initializer()    # over rev 1.0 api
sess = tf.Session()
sess.run(init)  # reset values to wrong

# Variables for flotting cost function
W_val = []
cost_val = []

for i in range(-30, 50):
    feed_W = i * 0.1
    curr_cost, curr_W = sess.run([cost, W], feed_dict={W: feed_W})
    W_val.append(curr_W)
    cost_val.append(curr_cost)

# Show the cost function
plt.plot(W_val, cost_val)
plt.show()
