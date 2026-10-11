import datetime
import os
import shutil

from aimpoint import AimPoint
from camera import CamClient
from config import CAM_IP, CAM_PASS, CAM_USER, FTP_IP, FTP_PASS, FTP_USER
from motors import MotorController
from nas import FTPClient

ftp_share = ""
mxtherm = "src/build/mxtherm"


def mkdir_if_not_exists(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


now = datetime.datetime.utcnow()
print(now)
local_folder = now.strftime("%Y/%m/%d/%H/%M")

motor_controller = MotorController()
mkdir_if_not_exists(local_folder)

cam_client = CamClient(CAM_IP, CAM_USER, CAM_PASS, mxtherm)
cam_client.enable_thermal_raw_data()
print(os.getcwd())
aimpoints = AimPoint.from_file("aimpoints.csv")

for aim_point in aimpoints:
    motor_controller.rotate_cam("x", aim_point.x)
    motor_controller.rotate_cam("y", aim_point.y)
    if aim_point.photo:
        base_name = f"{local_folder}/x{aim_point.x}_y{aim_point.y}"
        cam_client.download_img(f"{base_name}.png")
        cam_client.download_thermal(f"{base_name}.csv")

with FTPClient(FTP_IP, FTP_USER, FTP_PASS, ftp_share) as ftp_client:
    ftp_client.upload_folder(local_folder)

shutil.rmtree(local_folder)
