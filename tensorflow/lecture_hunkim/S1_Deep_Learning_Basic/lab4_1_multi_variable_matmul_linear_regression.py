################################################################################
# lab4-1 : Multi Variable Linear Regression
# 
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

tf.set_random_seed(777) # for reproducibility

x_data = [[73., 80., 75.],
          [93., 88., 93.],
          [89., 91., 90.],
          [96., 98., 100.],
          [73., 66., 70.]]
y_data = [[152.],
          [185.],
          [180.],
          [196.],
          [142.]]          

# Model parameters
W = tf.Variable(tf.random_normal([3, 1]), name='weight')
b = tf.Variable(tf.random_normal([1]), name='bias')

# Placeholder for a tensor that will be always fed
x = tf.placeholder(tf.float32, shape=[None, 3])
y = tf.placeholder(tf.float32, shape=[None, 1])

# Our hypothesis
linear_model = tf.matmul(x, W) + b

# cost/loss function
cost = tf.reduce_mean(tf.square(linear_model - y))

# Minimize
optimizer = tf.train.GradientDescentOptimizer(learning_rate=1e-5)
train = optimizer.minimize(cost)

# launch the graph in a session
init = tf.global_variables_initializer()    # over rev 1.0 api
sess = tf.Session()
sess.run(init)  # reset values to wrong

# training
for step in range(10001):
    cost_val, hy_val, _ = sess.run([cost, linear_model, train],
            feed_dict={x: x_data, y: y_data})
    if step % 5000 == 0:
       print(step, "Cost:", cost_val, "\nPrediction:\n", hy_val)
