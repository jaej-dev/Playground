################################################################################
# lab10-1 : minist by softmax cross entropy
#           accuracy : 0.9023
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

# Input placeholders, MNIST data image of shape 28 * 28 = 784 pixel
X = tf.placeholder(tf.float32, shape=[None, 784])
# 0 ~ 9 digits recognition = 10 classed
Y = tf.placeholder(tf.float32, shape=[None, nb_classes])

# Weights and bias for NN layers
W = tf.Variable(tf.random_normal([784, nb_classes]))
b = tf.Variable(tf.random_normal([nb_classes]))

# Hypothesis using softmax
hypothesis = tf.matmul(X, W) + b

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
        feed_dict={X: batch_xs, Y: batch_ys}
        c, _ = sess.run([cost, optimizer], feed_dict=feed_dict)
        avg_cost += c / total_batch
    print("Epoch:", "%04d" % (epoch + 1), "\tCost:", "{:.9f}".format(avg_cost))

print("Learning finished!!")

# Test model and check accuracy
correct_prediction = tf.equal(tf.argmax(hypothesis, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print("\nAccuracy:", sess.run(accuracy, feed_dict={X: mnist.test.images,
    Y: mnist.test.labels}))

# Get one and predict
r = random.randint(0, mnist.test.num_examples - 1)
print("\nTest one label and prediction...")
print("Label:     \t", sess.run(tf.argmax(mnist.test.labels[r:r + 1], 1)))
print("Prediction:\t", sess.run(tf.argmax(hypothesis, 1), feed_dict={
    X: mnist.test.images[r:r + 1]}))
