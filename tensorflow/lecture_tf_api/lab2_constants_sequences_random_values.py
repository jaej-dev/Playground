################################################################################
# Tensorflow Python API Guides
# Lab2 : Constants, Sequences and Random Values
#########################a######################################################
import tensorflow as tf

tf.set_random_seed(777)
a = tf.random_uniform([1])
b = tf.random_normal([1])

print("--- session 1 ---")
with tf.Session() as sess1:
    print(sess1.run(a))
    print(sess1.run(a))
    print(sess1.run(b))
    print(sess1.run(b))

    print("--- session 2 ---")
with tf.Session() as sess2:
    print(sess2.run(a))
    print(sess2.run(a))
    print(sess2.run(b))
    print(sess2.run(b))
