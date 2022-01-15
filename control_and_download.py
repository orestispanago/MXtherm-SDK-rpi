#!/usr/bin/env python3

import serial
import time
import datetime
import urllib.request
import os
import ftplib
import shutil

ftp_ip = ""
ftp_user = ""
ftp_password = ""
ftp_dir = ""

cameraIP = ""
camera_user = ""
camera_password = ""
cameraYPositions = ["y90", "Y45", "y90", "y135", "y90"]
cameraXPositions = ["x90", "x180", "x90"]
cameraUrl = f"http://{cameraIP}/cgi-bin/image.jpg"

thermal_downloader = "./src/build/mxtherm"
cameraSDKCommand = f"{thermal_downloader} --ip {cameraIP} --user {camera_user} --pass {camera_password}"

def mkdir_if_not_exists(dir_path):
    if not os.path.exists(dir_path):
        os.mkdir(dir_path)


def connect_to_arduino():
    arduinoConnection = False
    for i in range(3):
        if (arduinoConnection == False):
            try:
                arduinoDevice = serial.Serial(
                    '/dev/ttyUSB0', baudrate=9600, timeout=1)
                arduinoDevice.flush()
                arduinoConnection = True
                print("Connection with Arduino established!\n")
                return arduinoDevice
            except:
                time.sleep(1)


def upload_ftp(filePathToSend):
    session = ftplib.FTP(ftp_ip, ftp_user, ftp_password, timeout=20)
    file = open(filePathToSend, 'rb')
    fileToSend = os.path.basename(filePathToSend)
    sendFileCommand = f"STOR {ftp_dir}/{fileToSend}"
    session.storbinary(sendFileCommand, file)
    file.close()
    session.quit()


def move_cam(pos):
    raspberryCommand = pos + "\n"
    while True:
        arduinoDevice.write(raspberryCommand.encode())
        arduinoResponse = arduinoDevice.readline().decode('utf-8').rstrip()
        if(pos in arduinoResponse):
            print(arduinoResponse)
            break


def download_photo(cameraUrl, folder, x, y):
    pic_path = f"{folder}/_x{x}_y{y}.jpg"
    urllib.request.urlretrieve(cameraUrl, pic_path)


def move_cam_and_download(folder):
    for x in cameraXPositions:
        move_cam(x)
        if(x != "x90"):
            for y in cameraYPositions:
                move_cam(y)
                if(y != "y90"):
                    download_photo(folder, x, y)
                    cameraSDKCommand = f"{cameraSDKCommand} --output {folder}/{x}_{y}.csv"
                    os.system(cameraSDKCommand)


arduinoDevice = connect_to_arduino()

now = datetime.datetime.now()
folderPath = now.strftime("%Y/%m/%d/%H/%M")

mkdir_if_not_exists(folderPath)
move_cam_and_download(folderPath)
upload_ftp(folderPath)
shutil.rmtree(folderPath)