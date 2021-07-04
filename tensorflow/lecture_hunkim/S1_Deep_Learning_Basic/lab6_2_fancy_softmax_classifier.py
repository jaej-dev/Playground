################################################################################
# lab6-2 : Fancy Softmax Classifier
#          cross_entropy, one_hot, reshape
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

# for reproducibility
tf.set_random_seed(777) 

# Predicting animal type based on various features
xy = np.loadtxt('data-04-zoo.csv', delimiter=',', dtype=np.float32)
x_data = xy[:, 0:-1]
y_data = xy[:, [-1]]

# print(x_data.shape, y_data.shape)

nb_classes = 7  # 0 ~ 6

# Predicting animal type based on various features
x = tf.placeholder(tf.float32, shape=[None, 16])
y = tf.placeholder(tf.int32, shape=[None, 1])  # 0 ~ 6

# One hot
y_one_hot = tf.one_hot(y, nb_classes)
print("one_hot", y_one_hot)
y_one_hot = tf.reshape(y_one_hot, [-1, nb_classes])
print("reshape", y_one_hot)

# Model parameters
W = tf.Variable(tf.random_normal([16, nb_classes]), name='weight')
b = tf.Variable(tf.random_normal([nb_classes]), name='bias')

# tf.nn.softmax computes softmax activations
# softmax = exp(logits) / reduce_sum(exp(logits), dim)
logits = tf.matmul(x, W) + b
hypothesis = tf.nn.softmax(logits)

# Cross entropy cost/loss
cost_i = tf.nn.softmax_cross_entropy_with_logits_v2(logits=logits,
    labels=y_one_hot)
cost = tf.reduce_mean(cost_i)

# Minimize
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.1).minimize(cost)

# Prediction    
prediction = tf.argmax(hypothesis, 1) # probabilities to labels, 0 ~ 6
correct_prediction = tf.equal(prediction, tf.argmax(y_one_hot, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

# Launch graph
with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())

    for step in range(2001):
        sess.run(optimizer, feed_dict={x: x_data, y: y_data})
        if step % 1000 == 0:
            loss, acc = sess.run([cost, accuracy], feed_dict={
                x: x_data, y: y_data})
            print("Step: {:5}\tLoss: {:.3f}\tAcc: {:.2%}".format(
                step, loss, acc))

    # Let's see if we can predict
    pred = sess.run(prediction, feed_dict={x: x_data})
    # y_data: (N,1) = flatten => (N, ) matches pred.shape
    for p, y in zip(pred, y_data.flatten()):
        print("[{}] Prediction: {} True Y: {}".format(p == int(y), p, int(y)))
