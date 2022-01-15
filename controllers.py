import serial
import time


def rotate_cam(axis, degrees):
    print(f"Rotated cam at: {axis}, {degrees} degrees")


def download_photo(folder, x, y):
    print(f"Downloaded photo: {folder}/{x}_{y}.png")


class ArduinoClient:
    def __init__(self):  # init may not be necessary
        self.device = None

    def connect(self):
        arduino_connection = False
        for i in range(3):
            if not arduino_connection:
                try:
                    self.device = serial.Serial(
                        '/dev/ttyUSB0', baudrate=9600, timeout=1)
                    self.device.flush()
                    arduino_connection = True
                    print("Connection with Arduino established!\n")
                    return self.device
                except:
                    time.sleep(1)

    def rotate_cam(self, pos):
        raspberry_command = pos + "\n"
        while True:
            self.write(raspberry_command.encode())
            arduino_response = self.device.readline().decode('utf-8').rstrip()
            if pos in arduino_response:
                print(arduino_response)
                break
