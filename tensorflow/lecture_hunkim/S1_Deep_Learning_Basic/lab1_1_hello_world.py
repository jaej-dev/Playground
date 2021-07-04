################################################################################
# lab1-1 : Hello Tehnsorflow
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

# Create a constant op
# This op is added as a node to the default graph
hello = tf.constant("Hello, Tensorflow!")

# seart a TF session
sess = tf.Session()

# run the op and get result
print(sess.run(hello))
