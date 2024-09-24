# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 18:53:24 2024

Code tested in Spyder Anaconda environment.

This program utilizes the resistance control mode with the early version of our logging functions.
Tested firmware is v0.2 release on the Node DEV 0.2 board

Summary
This script sets up a serial connection with an Arduino, logs data during a predefined experiment, parses the data, and visualizes it in real-time using Matplotlib. The experiment is controlled via serial commands, and the data is continuously updated and displayed in a set of subplots. The script demonstrates how to handle real-time data visualization and serial communication in Python.

@author: Mark Dannemiller
"""

# Importing Libraries 
import serial 
import time
from timeit import default_timer
import matplotlib.pyplot as plt

arduino = serial.Serial(port='COM7', baudrate=115200, timeout=1)
buffer = []

# This experiment setup will log the arduino clock, MX enabled data, and power dissipation per frame
time_data = []
m1_en_data = []
vbatt_data = []
vld_data = []
amps_data = []
resist_data = []
pwm_data = []
    

def send_command(x):
    arduino.write(bytes(x + '\n', 'utf-8'))
    time.sleep(0.05)

def get_data():
    global buffer
    try:
        buffer = arduino.readline().decode('utf-8').strip()  # Properly decode and strip the data
        if not buffer:
            return  # Skip if data is empty
        print(buffer)
        parse_data()
        #update_plot(10)
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")

# These lines are specific to M1/M2 under tf-node-firmware V1.0 log() function
# "log-mode node 1" and "log-mode m<x> 1" must be set
def parse_data():
    global buffer
    try:
        buffer_split = buffer.split(' ')

        log_time = int(buffer_split[0].strip("\\n'"))
        vbatt = float(buffer_split[1].rstrip("\\n'"))
        vld = float(buffer_split[12].rstrip("\\n'"))
        amps = float(buffer_split[11].rstrip("\\n'"))
        ohms = float(buffer_split[13].rstrip("\\n'"))
        m1_enabled = True if "1" in buffer_split[7] else False
        pwm = float(buffer_split[10].rstrip("\\n'"))

        time_data.append(log_time)
        m1_en_data.append(m1_enabled)
        vbatt_data.append(vbatt)
        vld_data.append(vld)
        amps_data.append(amps)
        resist_data.append(ohms)
        pwm_data.append(pwm)
    except Exception as e:
        print(f"Error parsing data: {e}")
    buffer = []


def plot_data(ax, title, x_label, y_label, x_arr, y_arr, y_range, history):
    ax.clear()
    start_index = len(x_arr) - history
    if(start_index < 0 or history < 0):
        start_index = 0
        start_range = 0
    else:
        start_range = x_arr[start_index]
        
    for i in range(start_index, len(x_arr) - 1):
        if x_arr[i] < x_arr[i + 1]:
            #color = 'r-' if m1_en_data[i] else 'b-' # Color based on enabled
            
            # Normalize pwm_data to be between 0 and 1
            normalized_value = pwm_data[i] / 255.0
            # Get color from the 'cool' colormap (which ranges from blue to red)
            color = plt.cm.cool(0.5)
            ax.plot(x_arr[i:i + 2], y_arr[i:i + 2], color)

    
    ax.set_xlim(start_range, x_arr[-1] + 1000)  # Adding a small margin of 1000 ms for better visualization    
    ax.set_ylim(0, y_range)
    ax.set_title(title)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)


# Initialize the plot
plt.style.use("fivethirtyeight")
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(10, 8))

def update_plot(history):
    if len(time_data) > 0:
        plot_data(ax1, 'VBatt Data', 'Log Time (ms)', 'Voltage (V)', time_data, vbatt_data, 24, history)
        plot_data(ax2, 'V_LD Data', 'Log Time (ms)', 'Voltage (V)', time_data, vld_data, 30, history)
        plot_data(ax3, 'Amps Data', 'Log Time (ms)', 'Current (A)', time_data, amps_data, 100, history)
        plot_data(ax4, 'M2 Resist Data', 'Log Time (ms)', 'Resistance (mΩ)', time_data, resist_data, 1000, history)

        fig.tight_layout()
        plt.pause(0.001)

def delay(wait_time):
    start = default_timer()
    while default_timer() - start < wait_time:
        get_data()

# Experiment setup
# Used with a 12.5 V battery @ 20 Hz
wait1 = 300.0
wait2 = 5.0

control_mode = "ohms"
setpoint1 = "170"
m_port = "m1"

# Muscle run in resistance mode
send_command("set-enable all false")
send_command("set-mode all " + control_mode)
send_command("set-setpoint all " + control_mode + " " + setpoint1)
time.sleep(0.5)
send_command("log-mode all 0")
send_command("log-mode node 2")
send_command("log-mode " + m_port + " 2")
send_command("set-enable " + m_port + " true")

delay(wait1)
send_command("set-enable all false")
delay(wait2) # Disable and then continue monitoring for a bit

arduino.close()

# Plotting and Printing data
print(time_data)
print(m1_en_data)
print(vbatt_data)
print(amps_data)
print(vld_data)
print(resist_data)
print(pwm_data)

update_plot(-1)