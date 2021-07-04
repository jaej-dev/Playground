################################################################################
# MNIST loader
#
# File name : mnist_loader.py
# Author    : jwjang
# History   :
################################################################################
import tensorflow as tf
import os
import numpy as np
import gzip
import pickle
from os import path
from urllib import request
from tqdm import tqdm


class MnistLoader:
    filename = [
        ["x_train","train-images-idx3-ubyte.gz"],
        ["x_test","t10k-images-idx3-ubyte.gz"],
        ["y_train","train-labels-idx1-ubyte.gz"],
        ["y_test","t10k-labels-idx1-ubyte.gz"]
    ]

    def __init__(self, config):
        self.config = config

        self.init()
        self.load()
        self.x_train = self.data_pkl['x_train']
        self.y_train = self.data_pkl['y_train']
        self.x_test = self.data_pkl['x_test']
        self.y_test = self.data_pkl['y_test']


        self.train_len = self.x_train.shape[0]
        self.test_len = self.x_test.shape[0]

        self.num_iterations_train = (self.train_len+self.config.batch_size-1) //\
                                     self.config.batch_size
        self.num_iterations_test = (self.test_len+self.config.batch_size-1) //\
                                    self.config.batch_size

        print("x_train:", self.x_train.shape, self.x_train.dtype)
        print("y_train:", self.y_train.shape, self.y_train.dtype)
        print("x_test:", self.x_test.shape, self.x_test.dtype)
        print("y_test:", self.y_test.shape, self.y_test.dtype)
        print("train_len:", self.train_len)
        print("test_len:", self.test_len)
        print("num_iterations_train:", self.num_iterations_train)
        print("num_iterations_test:", self.num_iterations_test)

        print("Data loaded successfully..")

        self.features_placeholder = None
        self.labels_placeholder = None
        self.dataset = None
        self.iterator = None
        self.init_iterator_op = None
        self.next_batch = None

        self.build_dataset()

    def download_mnist(self):
        base_url = "http://yann.lecun.com/exdb/mnist/"
        for name in self.filename:
            fullfilename = os.path.join(self.config.data_dl_path, str(name[1]))
            if path.exists(fullfilename):
                print("Already exist " + fullfilename + "...")
            else:
                print("Downloading " + fullfilename + "...")
                request.urlretrieve(base_url + name[1], fullfilename)
        print("Download complete.")

    def save_mnist_pkl(self):
        # save mnist to pickle(serialization)
        if path.exists(self.config.data_pkl):
            print("Already exist " + self.config.data_pkl + "...")
            return

        mnist = {}
        for name in self.filename[:2]:
            fullfilename = os.path.join(self.config.data_dl_path, str(name[1]))
            with gzip.open(fullfilename, 'rb') as f:
                mnist[name[0]] = np.frombuffer(f.read(), np.uint8,
                                 offset=16).reshape(-1,28*28)
        for name in self.filename[-2:]:
            fullfilename = os.path.join(self.config.data_dl_path, str(name[1]))
            with gzip.open(fullfilename, 'rb') as f:
                mnist[name[0]] = np.frombuffer(f.read(), np.uint8, offset=8)

        if not path.exists(self.config.data_pkl_path):
            os.makedirs(self.config.data_pkl_path)
        with open(self.config.data_pkl, 'wb') as f:
            pickle.dump(mnist,f)
        print("Save complete.")

    def init(self):
        self.download_mnist()
        self.save_mnist_pkl()

    def load(self):
        with open(self.config.data_pkl,'rb') as f:
            self.data_pkl = pickle.load(f)

    def build_dataset(self):
        with tf.device('/cpu:0'):
            # Step1: make a dataset dynamicaly
            self.features_placeholder = \
                tf.placeholder(tf.float32, [None]+list(self.x_train.shape[1:]))
            self.labels_placeholder = tf.placeholder(tf.int32, [None, ])

            self.dataset = tf.data.Dataset.from_tensor_slices(
                (self.features_placeholder, self.labels_placeholder))
            self.dataset = self.dataset.batch(self.config.batch_size)
            
            # Step2: make a iterator
            self.iterator = tf.data.Iterator.from_structure(
                self.dataset.output_types, self.dataset.output_shapes)

            self.init_iterator_op = \
                self.iterator.make_initializer(self.dataset)

            self.next_batch = self.iterator.get_next()

            print("featres_placeholder: ", self.features_placeholder.shape)
            print("labels_placeholder: ", self.labels_placeholder.shape)
            print("X_batch shape dtype: ", self.next_batch[0].shape)
            print("Y_batch shape dtype: ", self.next_batch[1].shape)

    def initialize(self, sess, is_train):
        if is_train:
            idx = np.random.choice(self.train_len, self.train_len,
                  replace=False)
            self.x_train = self.x_train[idx]
            self.y_train = self.y_train[idx]

            # Step3: using dataset
            sess.run(self.init_iterator_op,
                feed_dict={self.features_placeholder: self.x_train,
                           self.labels_placeholder: self.y_train})
        else:
            sess.run(self.init_iterator_op,
                     feed_dict={self.features_placeholder: self.x_test,
                                self.labels_placeholder: self.y_test})


    def get_input(self):
        return self.next_batch            


def main():
    print("mnist_loader")

    class Config:
        image_height = 28
        image_width = 28
        num_classes = 10
        batch_size = 100
        data_dl_path = "/home/meere1/ws/alpha/Playground/tf_tmpl/data/"
        data_pkl_path = "/home/meere1/ws/alpha/Playground/tf_tmpl/data/mnist-python/"
        data_pkl = "/home/meere1/ws/alpha/Playground/tf_tmpl/data/mnist-python/mnist.pkl"

    tf.reset_default_graph()

    sess = tf.Session()

    data_loader = MnistLoader(Config)

    x, y = data_loader.next_batch

    data_loader.initialize(sess, is_train=True)

    out_x, out_y = sess.run([x, y])

    print("train out_x:", out_x.shape, out_x.dtype)
    print("train out_y:", out_y.shape, out_y.dtype)

    data_loader.initialize(sess, is_train=False)

    out_x, out_y = sess.run([x, y])

    print("test out_x:", out_x.shape, out_x.dtype)
    print("test out_y:", out_y.shape, out_y.dtype)
    
    sess.close()

if __name__ == '__main__':
    main()
