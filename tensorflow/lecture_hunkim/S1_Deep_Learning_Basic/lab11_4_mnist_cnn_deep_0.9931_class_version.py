################################################################################
# lab11-4 : minist by deep learning convolution nerual network class version
#           accuracy : 0.9931
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

# Model class
class Model:
    def __init__(self, sess, name):
        self.sess = sess
        self.name = name
        self._build_net()

    def _build_net(self):
        with tf.variable_scope(self.name):
            # dropout (keep_prob) rate 0.7~0.5 on training,
            # build should be 1 for testing
            self.keep_prob = tf.placeholder(tf.float32)

            # Input placeholders 
            self.X = tf.placeholder(tf.float32, shape=[None, 784]) # imgage = 28x28 = 784 pixel
            X_img = tf.reshape(self.X, [-1, 28, 28, 1]) # all images x 28x28 x 1(256 gray)
            self.Y = tf.placeholder(tf.float32, shape=[None, nb_classes])

            # L1 image input shape -> (?, 28, 28, 1)
            # Filter1 shape design -> (3x3 x 1(256gray) x 32EA filters)
            # Convolution shape    -> (?, 28, 28, 32) because padding set to 'SAME'
            # Poolig shape         -> (?, 14, 14, 32) because pooling kernel size set to 2x2
            W1 = tf.Variable(tf.random_normal([3, 3, 1, 32], stddev=0.01))
            L1 = tf.nn.conv2d(X_img, W1, strides=[1, 1, 1, 1], padding='SAME')
            L1 = tf.nn.relu(L1)
            L1 = tf.nn.max_pool(L1, ksize=[1 ,2, 2, 1], strides=[1, 2, 2, 1],
                    padding='SAME')
            L1 = tf.nn.dropout(L1, keep_prob=self.keep_prob)

            # L2 image input shape -> (?, 14, 14, 32)
            # Filter2 shape design -> (3x3 x 32(channel) x 64EA filters)
            # Convolution shape    -> (?, 14, 14, 64) because padding set to 'SAME'
            # Poolig shape         -> (?, 7, 7, 64) because pooling kernel size set to 7x7
            W2 = tf.Variable(tf.random_normal([3, 3, 32, 64], stddev=0.01))
            L2 = tf.nn.conv2d(L1, W2, strides=[1, 1, 1, 1], padding='SAME')
            L2 = tf.nn.relu(L2)
            L2 = tf.nn.max_pool(L2, ksize=[1 ,2, 2, 1], strides=[1, 2, 2, 1],
                    padding='SAME')
            L2 = tf.nn.dropout(L2, keep_prob=self.keep_prob)

    
            # L3 image input shape -> (?, 7, 7, 64)
            # Filter2 shape design -> (3x3 x 64(channel) x 128EA filters)
            # Convolution shape    -> (?, 7, 7, 128) because padding set to 'SAME'
            # Poolig shape         -> (?, 4, 4, 128) because pooling kernel size set to 4x4
            W3 = tf.Variable(tf.random_normal([3, 3, 64, 128], stddev=0.01))
            L3 = tf.nn.conv2d(L2, W3, strides=[1, 1, 1, 1], padding='SAME')
            L3 = tf.nn.relu(L3)
            L3 = tf.nn.max_pool(L3, ksize=[1 ,2, 2, 1], strides=[1, 2, 2, 1],
                    padding='SAME')
            L3 = tf.nn.dropout(L3, keep_prob=self.keep_prob)
            L3_flat = tf.reshape(L3, [-1, 4 * 4 * 128]) # for fully connected layer!!

            # FC 1st 4x4x128 inputs to 625 outputs
            W4 = tf.get_variable("W4", shape=[4 * 4 * 128, 625],
                    initializer=tf.contrib.layers.xavier_initializer())
            b4 = tf.Variable(tf.random_normal([625]))
            L4 = tf.nn.relu(tf.matmul(L3_flat, W4) + b4)
            L4 = tf.nn.dropout(L4, keep_prob=self.keep_prob)

            # FC 2nd 625 inputs to 10(nb_classes)   
            W5 = tf.get_variable("W5", shape=[625, nb_classes],
                    initializer=tf.contrib.layers.xavier_initializer())
            b5 = tf.Variable(tf.random_normal([nb_classes]))
            self.logits = tf.matmul(L4, W5) + b5

        # Define cost/loss & optimizer
        self.cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(
                        logits=self.logits, labels=self.Y))
        self.optimizer = tf.train.AdamOptimizer(
                            learning_rate=learning_rate).minimize(self.cost)

        # Accuracy
        correct_prediction = tf.equal(tf.argmax(self.logits, 1), tf.argmax(
                                self.Y, 1))
        self.accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    def predict(self, x_test, keep_prop=1.0):
        return self.sess.run(self.logits, feed_dict={self.X: x_test,
                self.keep_prob: keep_prop})

    def get_accuracy(self, x_test, y_test, keep_prop=1.0):
        return self.sess.run(self.accuracy, feed_dict={self.X: x_test,
                self.Y: y_test, self.keep_prob: keep_prop})

    def train(self, x_data, y_data, keep_prop=0.7):
        return self.sess.run([self.cost, self.optimizer], feed_dict={
                self.X: x_data, self.Y: y_data, self.keep_prob: keep_prop})

    def prediction_self_test(self, keep_prop=1.0):
        # Get one and predict
        r = random.randint(0, mnist.test.num_examples - 1)
        print("\nTest one label and prediction...")
        print("Label:     \t",
            self.sess.run(tf.argmax(mnist.test.labels[r:r + 1], 1)))
        print("Prediction:\t",
            self.sess.run(tf.argmax(self.logits, 1),
            feed_dict={self.X: mnist.test.images[r:r + 1],
               self.keep_prob: keep_prop}))
    

# Initialize
sess = tf.Session()
m1 = Model(sess, "m1")
sess.run(tf.global_variables_initializer())

# Train model
print("Learning started. It takes sometime...")
for epoch in range(training_epochs):
    avg_cost = 0
    total_batch = int(mnist.train.num_examples / batch_size)

    for i in range(total_batch):
        batch_xs, batch_ys = mnist.train.next_batch(batch_size)
        c, _ = m1.train(batch_xs, batch_ys)
        avg_cost += c / total_batch

    print("[Epoch: {:>4}]\tCost: {:>.9}".format(epoch + 1, avg_cost))

print("Learning finished!!")

# Test model and check accuracy
print("\nAccuracy:", m1.get_accuracy(mnist.test.images, mnist.test.labels))

# Get one and predict
m1.prediction_self_test()
