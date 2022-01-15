import ftplib
import datetime
import os
import glob

ftp_ip = ""
ftp_user = ""
ftp_password = ""
ftp_folder = ""


def make_dirs_if_not_exist(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


def ftp_mkdir_and_enter(session, dir_name):
    if dir_name not in session.nlst():
        session.mkd(dir_name)
    session.cwd(dir_name)


def ftp_make_dirs_and_enter(session, folder_path):
    for f in folder_path.split("/"):
        ftp_mkdir_and_enter(session, f)


def ftp_upload_folder_contents(session, folder_path):
    for fpath in glob.glob(f'{folder_path}/*'):
        with open(fpath, 'rb') as f:
            session.storbinary(f"STOR {os.path.basename(fpath)}", f)


now = datetime.datetime.now()
folder_path = now.strftime("%Y/%m/%d")
make_dirs_if_not_exist(folder_path)
with open(f'{folder_path}/myfile.txt', 'w') as fp:
    pass


def upload_to_ftp(ftp_ip, ftp_user, ftp_password, ftp_folder, local_folder):
    with ftplib.FTP(ftp_ip, ftp_user, ftp_password, timeout=20) as session:
        session.cwd(ftp_folder)
        ftp_make_dirs_and_enter(session, local_folder)
        ftp_upload_folder_contents(session, local_folder)


upload_to_ftp(ftp_ip, ftp_user, ftp_password, ftp_folder, folder_path)
