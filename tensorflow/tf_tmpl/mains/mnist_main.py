################################################################################
# Machine Learining for MNIST
#
# File name : mnist_main.py
# Author    : jwjang
# History   :
################################################################################
import sys
sys.path.extend(['..'])

import tensorflow as tf

from data_loader.mnist_loader import MnistLoader
from models.mnist_model import MnistModel
from trainers.mnist_trainer import MnistTrainer

from utils.config import process_config
from utils.dirs import create_dirs
from utils.logger import DefinedSummarizer
from utils.utils import get_args

def main():

    print("start... mnist_main.py")

    try:
        args = get_args()
        config = process_config(args.config)

    except:
        print("missing or invalid arguments")
        exit(0)

    # create the experiments dirs
    create_dirs([config.summary_dir, config.checkpoint_dir])

    # create tensorflow session
    sess = tf.Session()

    # create data generator
    data_loader = MnistLoader(config)

    # create machine learning model
    model = MnistModel(data_loader, config)

    # create tensorboard logger
    logger = DefinedSummarizer(sess, config,
             scalar_tags=['train/loss_per_epoch', 'train/acc_per_epoch',
                          'test/loss_per_epoch','test/acc_per_epoch'])

    # create trainer
    trainer = MnistTrainer(sess, model, config, logger, data_loader)

    # here you train your model
    trainer.train()

    # delete tensorflow session
    sess.close()

    # program exit
    print("exit... mnist_main.py")

if __name__ == '__main__':
    main()
