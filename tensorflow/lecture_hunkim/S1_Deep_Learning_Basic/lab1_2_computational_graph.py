################################################################################
# lab1-1 : Hello Tehnsorflow
################################################################################

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

node1 = tf.constant(3.0, tf.float32)
node2 = tf.constant(4.0, tf.float32)
node3 = tf.add(node1, node2)

print("node1: ", node1)
print("node2: ", node2)
print("node3: ", node3)

sess = tf.Session()
print("sess.run(node1: ", sess.run(node1))
print("sess.run(node2: ", sess.run(node2))
print("sess.run(node3: ", sess.run(node3))
