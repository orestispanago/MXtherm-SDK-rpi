import time

import serial


class MotorController:
    def __init__(self):
        self.connected = False
        self.arduino_device = None
        for i in range(3):
            if self.connected == False:
                try:
                    arduino_device = serial.Serial(
                        "/dev/ttyUSB0", baudrate=9600, timeout=1
                    )
                    arduino_device.flush()
                    self.connected = True
                    print("Connection with Arduino established!\n")
                    self.arduino_device = arduino_device
                    break
                except:
                    time.sleep(1)
        if not self.connected:
            exit("Attempted USB connection to Arduino 3 times and failed!\n")

    def rotate_cam(self, axis, angle):
        cmd = axis + angle + "\n"
        print("Position: " + axis + angle)
        while True:
            self.arduino_device.write(cmd.encode())
            arduino_response = (
                self.arduino_device.readline().decode("utf-8").rstrip()
            )
            if axis + angle in arduino_response:
                print(arduino_response)
                break
