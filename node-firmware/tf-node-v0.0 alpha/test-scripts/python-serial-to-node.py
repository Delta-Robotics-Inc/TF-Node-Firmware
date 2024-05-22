# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 18:53:24 2024

@author: priva
"""

# Importing Libraries 
import serial 
import time 
arduino = serial.Serial(port='COM6', baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x + '\n',   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return   data

# Muscle 1 run test script
print(write_read("set-enable all false"))
time.sleep(1.0)
print(write_read("set-setpoint all percent 1.0"))
print(write_read("set-enable m1 true"))
time.sleep(3.0)
print(write_read("set-enable all false"))

'''
print(write_read("set-enable m1 false"))
print(write_read("set-enable m2 true"))
time.sleep(1.0)
print(write_read("set-enable m2 false"))
print(write_read("set-enable m3 true"))
time.sleep(1.0)
print(write_read("set-enable all false"))
'''

arduino.close()