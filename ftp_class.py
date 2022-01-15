import ftplib
import glob
import os
import datetime

ftp_ip = ""
ftp_user = ""
ftp_password = ""
ftp_share = ""


class FTPClient:
    def __init__(self, ip, user, passwd, folder):
        self.ip = ip
        self.user = user
        self.passwd = passwd
        self.folder = folder
        self._session = None

    def __enter__(self):
        self.session = ftplib.FTP(ftp_ip, ftp_user, ftp_password, timeout=20)
        self.session.cwd(self.folder)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.session.quit()

    def _mkdir_and_enter(self, dir_name):
        if dir_name not in self.session.nlst():
            self.session.mkd(dir_name)
        self.session.cwd(dir_name)

    def _make_dirs(self, folder_path):
        for f in folder_path.split("/"):
            self._mkdir_and_enter(f)

    def upload_folder(self, folder_path):
        """
        Uploads local folder contents to FTP server.

        Creates remote directory tree if necessary.

        :param folder_path: local folder path
        """
        self._make_dirs(folder_path)
        for file_path in glob.glob(f'{folder_path}/*'):
            with open(file_path, 'rb') as f:
                self.session.storbinary(f"STOR {os.path.basename(file_path)}", f)


def make_dirs_if_not_exist(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


now = datetime.datetime.now()
local_folder = now.strftime("%Y/%m/%d")
make_dirs_if_not_exist(local_folder)
with open(f'{local_folder}/myfile.txt', 'w') as fp:
    pass

with FTPClient(ftp_ip, ftp_user, ftp_password, ftp_share) as ftp_client:
    ftp_client.upload_folder(local_folder)
