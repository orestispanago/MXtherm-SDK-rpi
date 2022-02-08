import datetime
import os
import shutil

from aimpoint import AimPoint
from camera import CamClient
from motors import MotorController
from nas import FTPClient

ftp_ip = ""
ftp_user = ""
ftp_password = ""
ftp_share = ""

cam_ip = ""
cam_user = ""
cam_passwd = ""
mxtherm = "src/build/mxtherm"


def mkdir_if_not_exists(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


now = datetime.datetime.utcnow()
print(now)
local_folder = now.strftime("%Y/%m/%d/%H/%M")

motor_controller = MotorController()
mkdir_if_not_exists(local_folder)

cam_client = CamClient(cam_ip, cam_user, cam_passwd, mxtherm)
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

with FTPClient(ftp_ip, ftp_user, ftp_password, ftp_share) as ftp_client:
    ftp_client.upload_folder(local_folder)

shutil.rmtree(local_folder)
