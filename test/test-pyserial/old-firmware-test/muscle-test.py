# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 18:53:24 2024

This program will log the data from M1/M2 on the TF-Node device during a run duration.
It utilizes the serial string command structure of tf-node-firmware-v1.0
It also utilizes the log() feature from the same firmware

Finally, the experiment will display graphs of the results

Summary
This script sets up a serial connection with an Arduino, logs data during a predefined experiment, parses the data, and visualizes it in real-time using Matplotlib. The experiment is controlled via serial commands, and the data is continuously updated and displayed in a set of subplots. The script demonstrates how to handle real-time data visualization and serial communication in Python.

@author: Mark Dannemiller


Currently, this script needs to be ran with the node LOG_TIME >= 500 ms
"""

# Importing Libraries 
import serial 
import time
from timeit import default_timer
import matplotlib.pyplot as plt

arduino = serial.Serial(port='COM8', baudrate=115200, timeout=1)


def send_command(x):
    arduino.write(bytes(x + '\n', 'utf-8'))
    time.sleep(0.05)
    
setpoint_m1 = 0.35
setpoint_m2 = 0.35

# Muscle 1 run test script
send_command("set-enable all false")
send_command("set-mode all ohms")
send_command("set-setpoint m1 percent" + str(setpoint_m1))
send_command("set-setpoint m2 percent" + str(setpoint_m2))
time.sleep(0.1)
send_command("log-mode all 1")

send_command("set-enable m2 false")
send_command("set-enable m1 true")

time.sleep(10)

    
send_command("set-enable all false")

arduino.close()
