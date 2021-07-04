################################################################################
# lab10-6 : minist by nerual network with high level tensorflow api
#           accuracy : 0.9850
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
learning_rate = 0.01 # we can use large learning rate using Batch Normalization
training_epochs = 15
batch_size = 100
keep_prob = 0.7
nb_classes = 10 # 0 ~ 9 digits recognition = 10 classed

# Input placeholders 
X = tf.placeholder(tf.float32, shape=[None, 784]) # imgage = 28*28 = 784 pixel
Y = tf.placeholder(tf.float32, shape=[None, nb_classes])
train_mode = tf.placeholder(tf.bool, name='train_mode')

# Layer output size
hidden_output_size = 512
final_output_size = nb_classes

# Weight initializer: xavier
xavier_init = tf.contrib.layers.xavier_initializer()

# Normalizer: batch normalization    
bn_params = {
    'is_training': train_mode,
    'decay': 0.9,
    'updates_collections': None
}

# We can build short code using 'arg_scope' to avoid duplicate code
# same function with different arguments
with arg_scope([fully_connected],
        activation_fn = tf.nn.relu,
        weights_initializer = xavier_init,
        biases_initializer = None,
        normalizer_fn = batch_norm,
        normalizer_params = bn_params
        ):
    hidden_layer1 = fully_connected(X, hidden_output_size, scope="h1")
    h1_drop = dropout(hidden_layer1, keep_prob, is_training=train_mode)
    hidden_layer2 = fully_connected(h1_drop, hidden_output_size, scope="h2")
    h2_drop = dropout(hidden_layer2, keep_prob, is_training=train_mode)
    hidden_layer3 = fully_connected(h2_drop, hidden_output_size, scope="h3")
    h3_drop = dropout(hidden_layer3, keep_prob, is_training=train_mode)
    hidden_layer4 = fully_connected(h3_drop, hidden_output_size, scope="h4")
    h4_drop = dropout(hidden_layer4, keep_prob, is_training=train_mode)
    hypothesis = fully_connected(h4_drop, final_output_size,
            activation_fn=None, scope="hypothesis")

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
        feed_dict_train = {X: batch_xs, Y: batch_ys, train_mode: True}
        feed_dict_cost = {X: batch_xs, Y: batch_ys, train_mode: False}
        opt = sess.run(optimizer, feed_dict=feed_dict_train)
        c = sess.run(cost, feed_dict=feed_dict_cost)
        avg_cost += c / total_batch

    print("[Epoch: {:>4}]\tCost: {:>.9}".format(epoch + 1, avg_cost))

print("Learning finished!!")

# Test model and check accuracy
correct_prediction = tf.equal(tf.argmax(hypothesis, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print("\nAccuracy:", sess.run(accuracy, feed_dict={X: mnist.test.images,
    Y: mnist.test.labels, train_mode: False}))

# Get one and predict
r = random.randint(0, mnist.test.num_examples - 1)
print("\nTest one label and prediction...")
print("Label:     \t", sess.run(tf.argmax(mnist.test.labels[r:r + 1], 1)))
print("Prediction:\t", sess.run(tf.argmax(hypothesis, 1), feed_dict={
    X: mnist.test.images[r:r + 1], train_mode: False}))
