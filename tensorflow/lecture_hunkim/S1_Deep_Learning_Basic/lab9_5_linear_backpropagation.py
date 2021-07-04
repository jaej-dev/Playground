################################################################################
# lab9-5 : linear back propagation
#
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import random

tf.set_random_seed(777) # for reproducibility

# tf graph input
x_data = [[1.],
          [2.],
          [3.]]
y_data = [[1.],
          [2.],
          [3.]]

# Placeholders for a tensor that will be always fed.
X = tf.placeholder(tf.float32, shape=[None, 1])
Y = tf.placeholder(tf.float32, shape=[None, 1])

# Set wrong model weights
W = tf.Variable(tf.random_normal([1, 1], name='weight'))
b = tf.Variable(5.)

# Forward propagation
hypothesis = tf.matmul(X, W) + b

# Difference
assert hypothesis.shape.as_list() == Y.shape.as_list()
diff = (hypothesis - Y)

# Backward propagation (chain rule)
d_l1 = diff
d_b = d_l1
d_w = tf.matmul(tf.transpose(X), d_l1)
print("\nX:\n", X, "\nW:\n", W, "\nd_l1:\n", d_l1, "\nd_w:\n", d_w)

# Updating network using gradients
learning_rate = 0.1
step = [
    tf.assign(W, W - learning_rate * d_w),
    tf.assign(b, b - learning_rate * tf.reduce_mean(d_b)),
]

# Running and testing the traning process
RMSE = tf.reduce_mean(tf.square((Y - hypothesis)))

sess = tf.InteractiveSession()
init = tf.global_variables_initializer()
sess.run(init)

print("--------------------------- Step and RMSE ----------------------------")
for i in range(1001):
    if i % 500 == 0:
        print(i, sess.run([step, RMSE], feed_dict={X: x_data, Y: y_data}))

print("---------------------------- Hypothesis ------------------------------")
print(sess.run(hypothesis, feed_dict={X: x_data}))
