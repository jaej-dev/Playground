################################################################################
# lab4-3 : Filie Input Linear Regression via tensorflow reader
# 
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

tf.set_random_seed(777) # for reproducibility

filename_queue = tf.train.string_input_producer(
    ['data-01-test-score.csv'], shuffle=False, name='filename_queue')

reader = tf.TextLineReader()
key, value = reader.read(filename_queue)

# Default values, in case of empty columns.
# Also specifies the type of the decoded result
record_defaults = [[0.], [0.], [0.], [0.]]
xy = tf.decode_csv(value, record_defaults=record_defaults)

# Collect batches of csv in
train_x_batch, train_y_batch = \
    tf.train.batch([xy[0:-1], xy[-1:]], batch_size=10)

# Placeholders for a tensor that will be always fed.
x = tf.placeholder(tf.float32, shape=[None, 3])    
y = tf.placeholder(tf.float32, shape=[None, 1])

W = tf.Variable(tf.random_normal([3, 1], name='weight'))
b = tf.Variable(tf.random_normal([1], name='bias'))

# Hypothesis
linear_model = tf.matmul(x, W) + b

# Simplified cost/loss function
cost = tf.reduce_mean(tf.square(linear_model - y))

# Minimize
optimizer = tf.train.GradientDescentOptimizer(learning_rate=1e-5)
train = optimizer.minimize(cost)

# Launch the graph in a session
init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)

# Start populating the filename queue
coord = tf.train.Coordinator()
threads = tf.train.start_queue_runners(sess=sess, coord=coord)

# Machine learning
for step in range(10001):
    x_batch, y_batch = sess.run([train_x_batch, train_y_batch])
    cost_val, hy_val, _ = sess.run(
            [cost, linear_model, train], feed_dict={x: x_batch, y: y_batch})
    if step % 5000 == 0:
        print(step, "Cost: ", cost_val, "\nPrediction: ", hy_val)

coord.request_stop()
coord.join(threads)

# ask my score
print("Your score will be ", sess.run(linear_model,
    feed_dict={x: [[100, 70, 101]]}))

print("Other scores will be ", sess.run(linear_model,
    feed_dict={x: [[60, 70, 110], [90, 100, 80]]}))
