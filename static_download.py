import datetime
import os
import shutil
import traceback
import logging
import logging.config

from camera import CamClient
from nas import FTPClient

ftp_ip = ""
ftp_user = ""
ftp_password = ""
ftp_share = "/cams/Thermal/Mobotix"

cam_ip = ""
cam_user = ""
cam_passwd = ""
mxtherm = "src/build/mxtherm"

dname = os.path.dirname(__file__)
os.chdir(dname)

logging.config.fileConfig("logging.conf", disable_existing_loggers=False)
logging.getLogger("urllib3").setLevel(logging.WARNING)
logger = logging.getLogger(__name__)


def mkdir_if_not_exists(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


cam_client = CamClient(cam_ip, cam_user, cam_passwd, mxtherm)
cam_client.enable_thermal_raw_data()

utc_now = datetime.datetime.now(tz=datetime.timezone.utc)
utc_now_str = utc_now.strftime("%Y%m%d_%H%M%S")
local_folder = f'static/{utc_now.strftime("%Y/%m/%d")}'
base_name = f"{local_folder}/thermal_{utc_now_str}"


def main():
    logger.info(f"{'-' * 15} START {'-' * 15}")
    mkdir_if_not_exists(local_folder)
    cam_client.download_img(f"{base_name}.png")
    cam_client.download_thermal(f"{base_name}.csv")
    with FTPClient(ftp_ip, ftp_user, ftp_password, ftp_share) as ftp_client:
        ftp_client.upload_folder(local_folder)
    shutil.rmtree(local_folder)
    logger.info(f"{'-' * 15} SUCCESS {'-' * 15}")


if __name__ == "__main__":
    try:
        main()
    except:
        logger.error("uncaught exception: %s", traceback.format_exc())
