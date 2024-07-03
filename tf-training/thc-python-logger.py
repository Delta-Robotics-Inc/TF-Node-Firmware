# -*- coding: utf-8 -*-
"""
Created on Tue May 28 12:22:55 2024

This script is to be used in conjunction with minima-thermocouple.ino arduino script to log thermocouple data

@author: Mark Dannemiller
"""

import serial
from timeit import default_timer

arduino = serial.Serial(port='COM7', baudrate=115200, timeout=.1)

def get_data():
    data = str(arduino.readline())
    data.rstrip()
    print(data)
    return data

# Experiment Params
exp_time = 10.0
start = default_timer()
elapsed = 0
data = []

# Code to collect data and append to list
while(elapsed < exp_time):
    elapsed = default_timer()-start
    data.append(get_data(), elapsed)


arduino.close()

# Probably save data to .csv