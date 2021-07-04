################################################################################
# lab11-6 : minist by convolution nerual network with ensemble layers
#           accuracy : 0.9946
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
training_epochs = 20
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
            self.training = tf.placeholder(tf.bool)

            # Input placeholders 
            self.X = tf.placeholder(tf.float32, shape=[None, 784]) # imgage = 28x28 = 784 pixel
            X_img = tf.reshape(self.X, [-1, 28, 28, 1]) # all images x 28x28 x 1(256 gray)
            self.Y = tf.placeholder(tf.float32, shape=[None, nb_classes])

            # Convolutional layer #1
            conv1 = tf.layers.conv2d(inputs=X_img, filters=32, 
                    kernel_size=[3, 3], padding="SAME", activation=tf.nn.relu)
            
            # Pooling layer #1
            pool1 = tf.layers.max_pooling2d(inputs=conv1, pool_size=[2, 2],
                    padding="SAME", strides=2)
            dropout1 = tf.layers.dropout(inputs=pool1, rate=0.3, 
                    training=self.training)

            # Convolutional layer #2
            conv2 = tf.layers.conv2d(inputs=dropout1, filters=64, 
                    kernel_size=[3, 3], padding="SAME", activation=tf.nn.relu)
            
            # Pooling layer #2
            pool2 = tf.layers.max_pooling2d(inputs=conv2, pool_size=[2, 2],
                    padding="SAME", strides=2)
            dropout2 = tf.layers.dropout(inputs=pool2, rate=0.3, 
                    training=self.training)

            # Convolutional layer #3
            conv3 = tf.layers.conv2d(inputs=dropout2, filters=128,
                    kernel_size=[3, 3], padding="SAME", activation=tf.nn.relu)
            
            # Pooling layer #3
            pool3 = tf.layers.max_pooling2d(inputs=conv3, pool_size=[2, 2],
                    padding="SAME", strides=2)
            dropout3 = tf.layers.dropout(inputs=pool3, rate=0.3, 
                    training=self.training)

            # Dense layer with RELU
            flat = tf.reshape(dropout3, [-1, 4 * 4* 128])
            dense4 = tf.layers.dense(inputs=flat, units=625,
                    activation=tf.nn.relu)
            dropout4 = tf.layers.dropout(dense4, rate=0.5,
                    training=self.training)

            # Logits (no activation) layer : L5 final FC 625 inputs to 10(nb_classes)
            self.logits = tf.layers.dense(inputs=dropout4, units=nb_classes)

        # Define cost/loss & optimizer
        self.cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(
                        logits=self.logits, labels=self.Y))
        self.optimizer = tf.train.AdamOptimizer(
                        learning_rate=learning_rate).minimize(self.cost)

        # Accuracy
        correct_prediction = tf.equal(tf.argmax(self.logits, 1), tf.argmax(
                                self.Y, 1))
        self.accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    def predict(self, x_test, training=False):
        return self.sess.run(self.logits, feed_dict={self.X: x_test,
                self.training: training})

    def get_accuracy(self, x_test, y_test, training=False):
        return self.sess.run(self.accuracy, feed_dict={self.X: x_test,
                self.Y: y_test, self.training: training})

    def train(self, x_data, y_data, training=True):
        return self.sess.run([self.cost, self.optimizer], feed_dict={
                self.X: x_data, self.Y: y_data, self.training: training})

    def prediction_self_test(self, training=False):
        # Get one and predict
        r = random.randint(0, mnist.test.num_examples - 1)
        print("\nTest one label and prediction...")
        print("Label:     \t",
            self.sess.run(tf.argmax(mnist.test.labels[r:r + 1], 1)))
        print("Prediction:\t",
            self.sess.run(tf.argmax(self.logits, 1),
            feed_dict={self.X: mnist.test.images[r:r + 1],
               self.training: training}))
    

# Initialize
sess = tf.Session()

models = []
num_models = 3
for m in range(num_models):
    models.append(Model(sess, "model" + str(m)))

sess.run(tf.global_variables_initializer())

# Train model
print("Learning started. It takes sometime...")
for epoch in range(training_epochs):
    avg_cost_list = np.zeros(len(models))
    total_batch = int(mnist.train.num_examples / batch_size)

    for i in range(total_batch):
        batch_xs, batch_ys = mnist.train.next_batch(batch_size)

        for m_idx, m in enumerate(models):
            c, _ = m.train(batch_xs, batch_ys)
            avg_cost_list[m_idx] += c / total_batch

    print("[Epoch:", "%04d" % (epoch + 1), "\tCost:", avg_cost_list)

print("Learning finished!!")

# Test model and check accuracy
test_size = len(mnist.test.labels)
predictions = np.zeros([test_size, 10])
for m_idx in enumerate(models):
    print("\nModel index:", m_idx, "\tAccuracy:", m.get_accuracy(
                mnist.test.images, mnist.test.labels))
    p = m.predict(mnist.test.images)
    predictions += p

ensemble_correct_prediction = tf.equal(
        tf.argmax(predictions, 1), tf.argmax(mnist.test.labels, 1))
ensemble_accuracy = tf.reduce_mean(
        tf.cast(ensemble_correct_prediction, tf.float32))
print("Ensemble accuracy:", sess.run(ensemble_accuracy))
