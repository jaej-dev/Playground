################################################################################
# lab11-2 : minist by convolution nerual network
#           accuracy : 0.9877
################################################################################
from tensorflow.contrib.layers import fully_connected, batch_norm, dropout
from tensorflow.contrib.framework import arg_scope
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
learning_rate = 0.001 # we can use large learning rate using Batch Normalization
training_epochs = 15
batch_size = 100
nb_classes = 10 # 0 ~ 9 digits recognition = 10 classed

# Input placeholders 
X = tf.placeholder(tf.float32, shape=[None, 784]) # imgage = 28x28 = 784 pixel
X_img = tf.reshape(X, [-1, 28, 28, 1]) # all images x 28x28 x 1(256 gray)
Y = tf.placeholder(tf.float32, shape=[None, nb_classes])

# L1 image input shape -> (?, 28, 28, 1)
# Filter1 shape design -> (3x3 x 1(256gray) x 32EA filters)
# Convolution shape    -> (?, 28, 28, 32) because padding set to 'SAME'
# Poolig shape         -> (?, 14, 14, 32) because pooling kernel size set to 2x2
W1 = tf.Variable(tf.random_normal([3, 3, 1, 32], stddev=0.01))
L1 = tf.nn.conv2d(X_img, W1, strides=[1, 1, 1, 1], padding='SAME')
L1 = tf.nn.relu(L1)
L1 = tf.nn.max_pool(L1, ksize=[1 ,2, 2, 1], strides=[1, 2, 2, 1],
        padding='SAME')

# L2 image input shape -> (?, 14, 14, 32)
# Filter2 shape design -> (3x3 x 32(32channel) x 64EA filters)
# Convolution shape    -> (?, 14, 14, 64) because padding set to 'SAME'
# Poolig shape         -> (?, 7, 7, 64) because pooling kernel size set to 7x7
W2 = tf.Variable(tf.random_normal([3, 3, 32, 64], stddev=0.01))
L2 = tf.nn.conv2d(L1, W2, strides=[1, 1, 1, 1], padding='SAME')
L2 = tf.nn.relu(L2)
L2 = tf.nn.max_pool(L2, ksize=[1 ,2, 2, 1], strides=[1, 2, 2, 1],
        padding='SAME')
L2_flat = tf.reshape(L2, [-1, 7 * 7 * 64]) # for fully connected layer!!

# Final FC 7x7x64 inputs to 10(nb_classes)
W3 = tf.get_variable("W3", shape=[7 * 7 * 64, nb_classes],
        initializer=tf.contrib.layers.xavier_initializer())
b = tf.Variable(tf.random_normal([nb_classes]))
logits = tf.matmul(L2_flat, W3) + b

# Define cost/loss & optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(
    logits=logits, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# Initialize
sess = tf.Session()
sess.run(tf.global_variables_initializer())

# Train model
print("Learning started. It takes sometime...")
for epoch in range(training_epochs):
    avg_cost = 0
    total_batch = int(mnist.train.num_examples / batch_size)

    for i in range(total_batch):
        batch_xs, batch_ys = mnist.train.next_batch(batch_size)
        feed_dict = {X: batch_xs, Y: batch_ys}
        c, _ = sess.run([cost, optimizer], feed_dict=feed_dict)
        avg_cost += c / total_batch

    print("[Epoch: {:>4}]\tCost: {:>.9}".format(epoch + 1, avg_cost))

print("Learning finished!!")

# Test model and check accuracy
correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print("\nAccuracy:", sess.run(accuracy, feed_dict={X: mnist.test.images,
    Y: mnist.test.labels}))

# Get one and predict
r = random.randint(0, mnist.test.num_examples - 1)
print("\nTest one label and prediction...")
print("Label:     \t", sess.run(tf.argmax(mnist.test.labels[r:r + 1], 1)))
print("Prediction:\t", sess.run(tf.argmax(logits, 1), feed_dict={
    X: mnist.test.images[r:r + 1]}))
