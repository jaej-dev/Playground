################################################################################
# Machine learining for jpg image recognition
#
# File name : grace_main.py
# Author    : jwjang
# History   : initial release
################################################################################
import sys
sys.path.extend(['..'])

import tensorflow as tf

from data_loader.grace_loader import GraceLoader
from models.grace_model import GraceModel
from trainers.grace_trainer import GraceTrainer

from utils.config import process_config
from utils.dirs import create_dirs
from utils.logger import DefinedSummarizer
from utils.utils import get_args

def main():

    print("start... grace_main.py")

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
    data_loader = GraceLoader(config)

    # create machine learning model
    model = GraceModel(data_loader, config)

    # create tensorboard logger
    logger = DefinedSummarizer(sess, config,
             scalar_tags=['train/loss_per_epoch', 'train/acc_per_epoch',
                          'test/loss_per_epoch','test/acc_per_epoch'])

    # create trainer
    trainer = GraceTrainer(sess, model, config, logger, data_loader)

    # here you train your model
    trainer.train()

    # delete tensorflow session
    sess.close()

    # program exit
    print("exit... grace_main.py")

if __name__ == '__main__':
    main()
