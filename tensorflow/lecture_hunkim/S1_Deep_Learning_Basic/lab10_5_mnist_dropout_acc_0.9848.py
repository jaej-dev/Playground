################################################################################
# lab10-5 : minist by nerual network with dropout
#           accuracy : 0.9848
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import random

# for reproducibility
tf.set_random_seed(777) 

# Import MNIST data
# Check out https://www.tensorflow.org/get_started/mnist/beginners for
# more information about the mnist dataset
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets("../../mnist_data/", one_hot=True)

# Parameters
learning_rate = 0.001
training_epochs = 15
batch_size = 100
nb_classes = 10

# dropout (keep_prob) rate 0.7 on training, but should be 1 for testing!!
keep_prob = tf.placeholder(tf.float32)

# Input placeholders, MNIST data image of shape 28 * 28 = 784 pixel
X = tf.placeholder(tf.float32, shape=[None, 784])
# 0 ~ 9 digits recognition = 10 classed
Y = tf.placeholder(tf.float32, shape=[None, nb_classes])

# Weights and bias for NN layers
W1 = tf.get_variable("W1", shape=[784, 512],
        initializer=tf.contrib.layers.xavier_initializer())
b1 = tf.Variable(tf.random_normal([512]))
L1 = tf.nn.relu(tf.matmul(X, W1) + b1)
L1 = tf.nn.dropout(L1, keep_prob=keep_prob)

W2 = tf.get_variable("W2", shape=[512, 512],
        initializer=tf.contrib.layers.xavier_initializer())
b2 = tf.Variable(tf.random_normal([512]))
L2 = tf.nn.relu(tf.matmul(L1, W2) + b2)
L2 = tf.nn.dropout(L2, keep_prob=keep_prob)

W3 = tf.get_variable("W3", shape=[512, 512],
        initializer=tf.contrib.layers.xavier_initializer())
b3 = tf.Variable(tf.random_normal([512]))
L3 = tf.nn.relu(tf.matmul(L2, W3) + b3)
L3 = tf.nn.dropout(L3, keep_prob=keep_prob)

W4 = tf.get_variable("W4", shape=[512, 512],
        initializer=tf.contrib.layers.xavier_initializer())
b4 = tf.Variable(tf.random_normal([512]))
L4 = tf.nn.relu(tf.matmul(L3, W4) + b4)
L4 = tf.nn.dropout(L4, keep_prob=keep_prob)

W5 = tf.get_variable("W5", shape=[512, nb_classes],
        initializer=tf.contrib.layers.xavier_initializer())
b5 = tf.Variable(tf.random_normal([nb_classes]))
# Hypothesis using softmax
hypothesis = tf.matmul(L4, W5) + b5

# Define cost/loss & optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(
    logits=hypothesis, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# Initialize
sess = tf.Session()
sess.run(tf.global_variables_initializer())

# Train model
for epoch in range(training_epochs):
    avg_cost = 0
    total_batch = int(mnist.train.num_examples / batch_size)

    for i in range(total_batch):
        batch_xs, batch_ys = mnist.train.next_batch(batch_size)
        feed_dict={X: batch_xs, Y: batch_ys, keep_prob: 0.7}
        c, _ = sess.run([cost, optimizer], feed_dict=feed_dict)
        avg_cost += c / total_batch
    print("Epoch:", "%04d" % (epoch + 1), "\tCost:", "{:.9f}".format(avg_cost))

print("Learning finished!!")

# Test model and check accuracy
correct_prediction = tf.equal(tf.argmax(hypothesis, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print("\nAccuracy:", sess.run(accuracy, feed_dict={X: mnist.test.images,
    Y: mnist.test.labels, keep_prob: 1}))

# Get one and predict
r = random.randint(0, mnist.test.num_examples - 1)
print("\nTest one label and prediction...")
print("Label:     \t", sess.run(tf.argmax(mnist.test.labels[r:r + 1], 1)))
print("Prediction:\t", sess.run(tf.argmax(hypothesis, 1), feed_dict={
    X: mnist.test.images[r:r + 1], keep_prob: 1}))
