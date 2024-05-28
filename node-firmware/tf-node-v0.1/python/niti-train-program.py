# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 18:53:24 2024

This program will log the data from M1 on the TF-Node device during a run duration.
It utilizes the serial string command structure of tf-node-firmware-v1.0
It also utilizes the log() feature from the same firmware

Finally, the experiment will display graphs of the results

@author: Mark Dannemiller
"""

# Importing Libraries 
import serial 
import time
from timeit import default_timer
import matplotlib.pyplot as plt


arduino = serial.Serial(port='COM7', baudrate=115200, timeout=.1)

LOG_INTERVAL = 0.1 # Interval in seconds between data logs from Node

buffer = []

# This experiment setup will log the arduino clock, M1 enabled data, and power dissipation per frame
time_data = []
m1_en_data = []
vbatt_data = []
vld_data = []
amps_data = []
resist_data = []

def send_command(x):
    arduino.write(bytes(x + '\n',   'utf-8'))
    time.sleep(0.05)

def get_data():
    data = str(arduino.readline())
    data.rstrip()
    print(data)
    if("=" in data):
        if(len(buffer) > 0):
            parse_data()
    else:
        buffer.append(data)
    return data

# These lines are specific to M1 under tf-node-firmware V1.0 log() function
# "log-mode node 1" and "log-mode m1 1" must be set
def parse_data():
    global buffer
    try:
        log_time_line = buffer[0].split(' ')
        vbatt_line = buffer[1].split(' ')
        amps_line = buffer[11].split(' ')
        vld_line = buffer[12].split(' ')
        ohms_line = buffer[13].split(' ')
        m1_enabled_line = buffer[7].split(' ') #**************
        #print(vbatt_line)
        #print(amps_line)
        log_time = int(log_time_line[2].strip("\\n'"))
        vbatt = float(vbatt_line[2].rstrip("\\n'"))
        vld = float(vld_line[2].rstrip("\\n'"))
        amps = float(amps_line[2].rstrip("\\n'"))
        ohms = float(ohms_line[2].rstrip("\\n'")) * 1000 #convert to mOhms
        m1_enabled = True if "true" in m1_enabled_line[2] else False
        #print(log_time)
        #print(m1_enabled)
        #print(vbatt)
        #print(amps)
        #print("\n")
        time_data.append(log_time)
        m1_en_data.append(m1_enabled)
        vbatt_data.append(vbatt)
        vld_data.append(vld)
        amps_data.append(amps)
        resist_data.append(ohms)
    except:
        pass
    buffer = []
    
def plot_data(title, x_label, y_label, x_arr, y_arr, x_range, y_range):
    # Creating a figure and axis object
    fig, ax = plt.subplots()

    # Assuming the data is sorted by x_arr
    for i in range(len(x_arr) - 1):
        if(x_arr[i] < x_arr[i+1]):
            if m1_en_data[i]:
                ax.plot(x_arr[i:i+2], y_arr[i:i+2], 'r-')  # Red line for True
            else:
                ax.plot(x_arr[i:i+2], y_arr[i:i+2], 'b-')  # Blue line for False

    # Setting the range for the x-axis and y-axis
    ax.set_xlim(0, x_range)
    ax.set_ylim(0, y_range)

    # Adding title and labels
    ax.set_title(title)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)

    # Displaying the plot
    plt.show()
    
    
#==============================================================================
# Training Script
#==============================================================================
    
wait1 = 25.0
wait2 = 0.0
wait3 = 5.0
wait_total_ms = (wait1 + wait2 + wait3) * 1000

mode = "manual" # Set the mode, "train" to train or "percent" to test without stop

# Muscle 1 run test script
send_command("set-enable all false")
send_command("set-mode all " + mode)
time.sleep(0.5)
send_command("log-mode node 1")
send_command("log-mode m1 1")
send_command("log-mode m2 0")
#send_command("set-setpoint all " + mode + " 0.5") #0.35 is a good value
send_command("set-enable m1 true")
start = default_timer()

while(default_timer() - start < wait1):
    get_data()
    
# Optional second period of low power
#send_command("set-setpoint all " + mode + " 0.25")
#start = default_timer()
#while(default_timer() - start < 25.0):
#    get_data()
    
send_command("set-enable all false")
start = default_timer()
while(default_timer() - start < wait3):
    get_data()


arduino.close()


#==============================================================================
# Plotting
#==============================================================================


# We can divide the elements of supply voltage (volts) by the corresponding elements of current data (amps), to get resistance (ohms)
# R = V/I // Multiply by 1000 for result in mOhms
#resist_data = [(V / I) * 1000 for V, I in zip(vbatt_data, amps_data)]

print(time_data)
print(m1_en_data)
print(vbatt_data)
print(amps_data)
print(vld_data)
print(resist_data)

# Plots
plot_data('V_Supp Data', 'Log Time (ms)', 'Supply (V)', time_data, vbatt_data, wait_total_ms, 24) # Plotting the Vbatt graph
plot_data('M2 Current Data', 'Log Time (ms)', 'Current (A)', time_data, amps_data, wait_total_ms, 30) # Plotting the Current Data Graph
plot_data('V_LD Data', 'Log Time (ms)', 'Voltage (V)', time_data, vld_data, wait_total_ms, 24) # Plotting the VLD graph
plot_data('M2 Resistance Data', 'Log Time (ms)', 'Resistance (mΩ)', time_data, resist_data, wait_total_ms, 1000) # Plotting the Resistance Data Graph
