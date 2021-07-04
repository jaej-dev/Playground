################################################################################
# lab2-1 : Linear Regression
# From https://www.tensorflow.org/get_started/get_started
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

tf.set_random_seed(777) # for reproductibility

# Model parameters
# Try to find values for W and b to compute y_data - W * x_data + b
# We know that W should be 1 and b shoud be 0
# But let's use Tensorflow to figure it out
W = tf.Variable(tf.random_normal([1]), name='weight')
b = tf.Variable(tf.random_normal([1]), name='bias')

# Model input and output
# Now we can use x and y in place of x_data and y_data placeholders for
# a tensor that will be always fed using feed_dict
x = tf.placeholder(tf.float32, shape=[None])
y = tf.placeholder(tf.float32, shape=[None])

# Our hypothesis
linear_model = x * W + b

# cost/loss function
loss = tf.reduce_sum(tf.square(linear_model - y))  # sum of the squares

# optimizer for minimize
optimizer = tf.train.GradientDescentOptimizer(0.01)
train = optimizer.minimize(loss)

# launch the graph in a session
init = tf.global_variables_initializer()    # over rev 1.0 api
sess = tf.Session()
sess.run(init)  # reset values to wrong

# Fit the line
for step in range(2001):
    curr_W, curr_b, curr_loss, _= \
        sess.run([W, b, loss, train], feed_dict={x: [1, 2, 3], y: [1, 2, 3]})
    if step % 200 == 0:
        print("step: %s\tW: %s\tb: %s\tloss: %s" % (step, curr_W, curr_b, curr_loss))
'''
step: 2000W: [1.0000014]    b: [-2.8900315e-06] loss: 4.092726e-12
'''

# Testing our model
print("Testing our model...")
print(sess.run(linear_model, feed_dict={x: [5]}))
print(sess.run(linear_model, feed_dict={x: [2.5]}))
print(sess.run(linear_model, feed_dict={x: [1.5, 3.5]}))
'''
[5.0000043]
[2.5000007]
[1.4999993 3.5000021]
'''

# Fit the line with new training data
for step in range(2001):
    curr_W, curr_b, curr_loss, _= \
        sess.run([W, b, loss, train], \
                feed_dict={x: [1,   2,   3,   4,   5], \
                           y: [1.2, 2.2, 3.2, 4.2, 5.2]})
    if step % 200 == 0:
        print("step: %s\tW: %s\tb: %s\tloss: %s" % (step, curr_W, curr_b, curr_loss))
'''
step: 2000W: [1.0000002]    b: [0.19999917] loss: 1.0942358e-12
'''

# Testing our model with new training data
print("Testing our model with new training data...")
print(sess.run(linear_model, feed_dict={x: [5]}))
print(sess.run(linear_model, feed_dict={x: [2.5]}))
print(sess.run(linear_model, feed_dict={x: [1.5, 3.5]}))
'''
[5.2000003]
[2.6999996]
[1.6999996 3.7      ]
'''
