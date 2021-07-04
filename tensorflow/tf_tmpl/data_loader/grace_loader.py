################################################################################
# Data loader for jpg images
#
# File name : grace_loader.py
# Author    : jwjang
# History   :
################################################################################
import tensorflow as tf
import cv2
import pickle
import numpy as np
import gzip
import os
from os import path
from os.path import isfile, join
from os import listdir
from random import shuffle
from shutil import copyfile
from urllib import request
from tqdm import tqdm


class GraceLoader:
    def __init__(self, config):
        self.config = config
        self.data_path = self.config.data_path
        self.data_labels = None
        self.data_labels_len = None
        self.data_features = None
        self.train_len = None
        self.test_len = None
        self.num_iterations_train = None
        self.num_iterations_test = None
        self.x_train = None
        self.y_train = None
        self.x_test = None
        self.y_test = None
        self.features_placeholder = None
        self.labels_placeholder = None
        self.dataset = None
        self.iterator = None
        self.init_iterator_op = None
        self.next_batch = None
       
        self.seperate_data()
        self.load()
        self.build_dataset()

    def seperate_data(self):
        data_dir = self.config.data_path
        count = 0
        print("Start seperate data...")
        for filename in listdir(data_dir):
            if isfile(join(data_dir, filename)):
                tokens = filename.split('.')
                if tokens[-1] == 'jpg':
                    count += 1
                    image_path = join(data_dir, filename)
                    if not os.path.exists(join(data_dir, tokens[0])):
                        os.makedirs(join(data_dir, tokens[0]))
                        print("New label directory created...", tokens[0])
                    copyfile(image_path,
                             join(join(data_dir, tokens[0]), filename))
                    os.remove(image_path)
        print(count, "data files are seperated...")

    def load(self):
        self.data_labels = self.get_data_labels()
        self.data_features = self.get_data_paths()
        self.data_labels_len = len(self.data_labels)
        min_count = 0
        for i in range(len(self.data_labels)):
            if i == 0:
                min_count = len(self.data_features[i])
                continue
            else:
                if len(self.data_features[i]) < min_count:
                    min_count = len(self.data_features[i])
        
        self.train_len = int(min_count * (1.0 - self.config.test_data_rate) * \
                             self.data_labels_len)
        self.test_len =  int(min_count * self.config.test_data_rate) * \
                             self.data_labels_len
        self.num_iterations_train = (self.train_len+self.config.batch_size-1) //\
                                     self.config.batch_size
        self.num_iterations_test = (self.test_len+self.config.batch_size-1) //\
                                    self.config.batch_size

        self.x_train, self.y_train, self.x_test, self.y_test = self.get_data()

        print("Number of classes:", self.data_labels_len)
        print("Number of each classe's image data:", min_count)
        print("Train len:", self.train_len)
        print("Test len:", self.test_len)
        print("Train iteration len:", self.num_iterations_train)
        print("Test iteration len:", self.num_iterations_test)
        print("x_train:", self.x_train.shape, self.x_train.dtype)
        print("y_train:", self.y_train.shape, self.y_train.dtype)
        print("x_test:", self.x_test.shape, self.x_test.dtype)
        print("y_test:", self.y_test.shape, self.y_test.dtype)

    def get_data_labels(self):
        # store directory name search as cat and dog, it's label
        data_labels = [] 
        for filename in listdir(self.data_path):
            if not isfile(join(self.data_path, filename)):
                data_labels.append(filename)
        #print("data_labels:", data_labels)
        return data_labels

    def get_data_paths(self):
        # store can and dog's fully image path name 
        data_paths = []
        for label in self.data_labels:
            img_lists = []
            path = join(self.data_path, label)
            for filename in listdir(path):
                tokens = filename.split('.')
                if tokens[-1] == 'jpg':
                    image_path=join(path, filename)
                    img_lists.append(image_path)
            shuffle(img_lists)
            data_paths.append(img_lists)
        return data_paths

    def img_resize(self, img, size):
        h, w = img.shape[:2]

        sh, sw = size
        # interpolation method
        if h > sh or w > sw:  # shrinking image
            interp = cv2.INTER_AREA
        else: # stretching image
            interp = cv2.INTER_CUBIC

        # aspect ratio of image
        aspect = w/h

        # padding
        if aspect > 1: # horizontal image
            new_shape = list(img.shape)
            new_shape[0] = w
            new_shape[1] = w
            new_shape = tuple(new_shape)
            new_img = np.zeros(new_shape, dtype = np.uint8)
            h_offset = int((w-h)/2)
            new_img[h_offset:h_offset + h, :, :] = img.copy()

        elif aspect < 1: # vertical image
            new_shape = list(img.shape)
            new_shape[0] = h
            new_shape[1] = h
            new_shape = tuple(new_shape)
            new_img = np.zeros(new_shape, dtype = np.uint8)
            w_offset = int((h-w) / 2)
            new_img[:, w_offset:w_offset + w, :] = img.copy()
        else:
            new_img = img.copy()
        # scale and pad
        scaled_img = cv2.resize(new_img, size, interpolation=interp)
        return scaled_img

    def get_data(self):
        batch_size = self.config.batch_size
        image_size = (self.config.image_height, self.config.image_width)
        convert_gray = self.config.convert_gray_scale

        x_train = []
        y_train = []
        x_test = []
        y_test = []
        empty = False
        counter = 0

        print("Image data loading started...")

        while True:
            for i in range(len(self.data_labels)):
                """
                label = np.zeros(len(self.data_labels), dtype = int)
                label[i] = 1
                """
                label = i
                if len(self.data_features[i]) < counter + 1:
                    empty = True
                    continue
                empty = False
                img = cv2.imread(self.data_features[i][counter])
                img = self.img_resize(img, image_size)
                if convert_gray:
                    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
                    img = np.reshape(img, (img.shape[0], img.shape[1], 1))
                if counter < (self.train_len / self.data_labels_len):
                    x_train.append(img)
                    y_train.append(label)
                else:
                    x_test.append(img)
                    y_test.append(label)
                    
            counter+=1

            if empty:
                break

        print("Image data loading finished!")

        return np.array(x_train, dtype = np.uint8), \
               np.array(y_train, dtype = np.uint8), \
               np.array(x_test, dtype = np.uint8), \
               np.array(y_test, dtype = np.uint8)

    def build_dataset(self):
        with tf.device('/cpu:0'):
            # Step1: make a dataset dynamicaly
            self.features_placeholder = \
                tf.placeholder(tf.float32, [None]+list(self.x_train.shape[1:]))
            """
            self.labels_placeholder = tf.placeholder(tf.int32, [None, ])
            """
            self.labels_placeholder = \
                tf.placeholder(tf.int32, [None]+list(self.y_train.shape[1:]))

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
    print("grace_loader.py")

    class Config:
        convert_gray_scale = False
        image_height = 100
        image_width = 100
        num_classes = 2
        batch_size = 10
        test_data_rate = 0.2
        data_path = "/home/meere1/ws/alpha/Playground/tf_tmpl/data/catdog"


    tf.reset_default_graph()

    sess = tf.Session()

    data_loader = GraceLoader(Config)

    x, y = data_loader.next_batch

    data_loader.initialize(sess, is_train=True) # memory exceed and task killed
    out_x, out_y = sess.run([x, y])
    print("train out_x:", out_x.shape, out_x.dtype)
    print("train out_y:", out_y.shape, out_y.dtype)

    data_loader.initialize(sess, is_train=False) # moeory exceed only
    out_x, out_y = sess.run([x, y])
    print("test out_x:", out_x.shape, out_x.dtype)
    print("test out_y:", out_y.shape, out_y.dtype)
    
    sess.close()


if __name__ == '__main__':
    main()
