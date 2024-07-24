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
        ohms = float(buffer_split[13].rstrip("\\n'"))  # convert to mOhms
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
        plot_data(ax3, 'Amps Data', 'Log Time (ms)', 'Current (A)', time_data, amps_data, 24, history)
        plot_data(ax4, 'M2 Resist Data', 'Log Time (ms)', 'Resistance (mΩ)', time_data, resist_data, 3000, history)

        fig.tight_layout()
        plt.pause(0.001)

# Experiment setup
wait1 = 10.0
wait2 = 0.0
wait3 = 5.0
wait_total_ms = (wait1 + wait2 + wait3) * 1000

mode = "train"#"train"  # Set the mode, "train" to train or "percent" to test without stop
setpoint = 0.1 #310 #360 #0.1

# Muscle 1 run test script
send_command("set-enable all false")
send_command("set-mode all " + mode)
send_command("set-setpoint all " + mode + " " + str(setpoint))
time.sleep(0.5)
send_command("log-mode node 2")
send_command("log-mode m2 2")
send_command("log-mode m1 0")
send_command("set-enable m2 true")

start = default_timer()
while default_timer() - start < wait1:
    get_data()
    
send_command("set-enable all false")

start = default_timer()
while default_timer() - start < wait3:
    get_data()

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