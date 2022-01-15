import os
import shutil


def make_dirs_if_not_exist(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


def delete_dir(folder_path):
    shutil.rmtree(folder_path)
