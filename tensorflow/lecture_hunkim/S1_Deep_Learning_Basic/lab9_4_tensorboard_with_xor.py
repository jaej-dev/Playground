################################################################################
# lab9-4 : Tensorboard with xor
#
# step 1: From TF graph, decide which tensors you want to log
# step 2: Merge all summaries
# step 3: Create writer and add graph
# step 4: Run summary merge and add_summary
# step 5: Launch the Tensorboard
#         $ tensorboard --logdir=./logs/xor_logs_r0_01
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import random

tf.set_random_seed(777) # for reproducibility
learning_rate = 0.1    

x_data = [[0, 0],
          [0, 1],
          [1, 0],
          [1, 1]]
y_data = [[0],
          [1],
          [1],
          [0]]

x_data = np.array(x_data, dtype=np.float32)
y_data = np.array(y_data, dtype=np.float32)

X = tf.placeholder(tf.float32, shape=[None, 2], name='x-input')
Y = tf.placeholder(tf.float32, shape=[None, 1], name='y-input')

with tf.name_scope("layer1"):
    W1 = tf.Variable(tf.random_normal([2, 2], name='weight1'))
    b1 = tf.Variable(tf.random_normal([2], name='bias1'))
    layer1 = tf.sigmoid(tf.matmul(X, W1) + b1)

    w1_hist = tf.summary.histogram("weights1", W1)
    b1_hist = tf.summary.histogram("baises1", b1)
    layer1_hist = tf.summary.histogram("layer1", layer1)

with tf.name_scope("layer2"):    
    W2 = tf.Variable(tf.random_normal([2, 1], name='weight2'))
    b2 = tf.Variable(tf.random_normal([1], name='bias2'))
    hypothesis = tf.sigmoid(tf.matmul(layer1, W2) + b2)

    w2_hist = tf.summary.histogram("weights2", W2)
    b2_hist = tf.summary.histogram("baises2", b2)
    hypothesis_hist = tf.summary.histogram("hypothesis", hypothesis)

# Cost / loss function
with tf.name_scope("cost"):
    cost = -tf.reduce_mean(Y * tf.log(hypothesis) + (1 - Y) *
        tf.log(1 - hypothesis))
    cost_summ = tf.summary.scalar("cost", cost)

with tf.name_scope("train"):
    train = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# Accuracy computation
# True if hypothesis > 0.5 else False
predicted = tf.cast(hypothesis > 0.5, dtype=tf.float32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(predicted, Y), dtype=tf.float32))
accuracy_summ = tf.summary.scalar("accuracy", accuracy)

# Launch graph
with tf.Session() as sess:
    # Tensorboard --logdir=./logs/xor_logs
    merged_summary = tf.summary.merge_all()
    writer = tf.summary.FileWriter("./logs/xor_logs_r0_01")
    writer.add_graph(sess.graph) # show the graph

    # Initialize tensorflow variables
    sess.run(tf.global_variables_initializer())

    for step in range(10001):
        summary, _ = sess.run([merged_summary, train], feed_dict={
            X: x_data, Y: y_data})
        writer.add_summary(summary, global_step=step)

        if step % 5000 == 0:
             print(step, sess.run(cost, feed_dict={X: x_data, Y: y_data}),
                sess.run([W1, W2]))
    
    # Accuracy report
    h, c, a = sess.run([hypothesis, predicted, accuracy], feed_dict={
        X: x_data, Y: y_data})
    print("\nHypothesis:\n", h, "\nCorrect:\n", c, "\nAccuracy:\n", a)
