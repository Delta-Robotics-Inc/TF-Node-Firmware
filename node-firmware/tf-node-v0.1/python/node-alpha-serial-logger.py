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

def write_read(x):
    arduino.write(bytes(x + '\n',   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data

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
        amps_line = buffer[10].split(' ')
        vld_line = buffer[11].split(' ')
        ohms_line = buffer[12].split(' ')
        m1_enabled_line = buffer[6].split(' ') #**************
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
    
mode = "percent" 

# Muscle 1 run test script
print(write_read("set-enable all false"))
print(write_read("set-mode all " + mode))
time.sleep(0.5)
print(write_read("log-mode node 1"))
print(write_read("log-mode m2 1"))
print(write_read("log-mode m1 0"))
print(write_read("set-setpoint all " + mode + " 0.25")) #0.35 is a good value
print(write_read("set-enable m2 true"))
start = default_timer()
while(default_timer() - start < 1.0):
    get_data()
arduino.write(bytes("set-enable all false" + '\n',   'utf-8'))
start = default_timer()
while(default_timer() - start < 5.0):
    get_data()


arduino.close()


# We can divide the elements of supply voltage (volts) by the corresponding elements of current data (amps), to get resistance (ohms)
# R = V/I // Multiply by 1000 for result in mOhms
#resist_data = [(V / I) * 1000 for V, I in zip(vbatt_data, amps_data)]

print(time_data)
print(m1_en_data)
print(vbatt_data)
print(amps_data)
print(vld_data)
print(resist_data)


# Plotting the Vbatt graph
#vbatt_graph = plt.plot(time_data, vbatt_data)

# Creating a figure and axis object
fig1, ax1 = plt.subplots()

# Assuming the data is sorted by time_data
for i in range(len(time_data) - 1):
    if(time_data[i] < time_data[i+1]):
        if m1_en_data[i]:
            ax1.plot(time_data[i:i+2], vbatt_data[i:i+2], 'r-')  # Red line for True
        else:
            ax1.plot(time_data[i:i+2], vbatt_data[i:i+2], 'b-')  # Blue line for False

# Setting x-axis labels to show every 10th label
#ax1.set_xticks(ticks=time_data[::50])
# Setting the range for the x-axis and y-axis
ax1.set_xlim(0, 25000)
ax1.set_ylim(0, 24)

# Adding title and labels
ax1.set_title('V_Supp Data')
ax1.set_xlabel('Log Time (ms)')
ax1.set_ylabel('Supply (V)')

# Displaying the plot
plt.show()



# Plotting the Current Data Graph
# Creating a figure and axis object
fig2, ax2 = plt.subplots()

# Assuming the data is sorted by time_data
for i in range(len(time_data) - 1):
    if(time_data[i] < time_data[i+1]):
        if m1_en_data[i]:
            ax2.plot(time_data[i:i+2], amps_data[i:i+2], 'r-')  # Red line for True
        else:
            ax2.plot(time_data[i:i+2], amps_data[i:i+2], 'b-')  # Blue line for False

# Setting x-axis labels to show every 10th label
#ax2.set_xticks(ticks=time_data[::50])
# Setting the range for the x-axis and y-axis
ax2.set_xlim(0, 25000)
ax2.set_ylim(0, 30)

# Adding title and labels
ax2.set_title('M2 Current Data')
ax2.set_xlabel('Log Time (ms)')
ax2.set_ylabel('Current (A)')

# Displaying the plot
plt.show()

# Plotting the VLD graph
#vbatt_graph = plt.plot(time_data, vbatt_data)

# Creating a figure and axis object
fig3, ax3 = plt.subplots()

# Assuming the data is sorted by time_data
for i in range(len(time_data) - 1):
    if(time_data[i] < time_data[i+1]):
        if m1_en_data[i]:
            ax3.plot(time_data[i:i+2], vld_data[i:i+2], 'r-')  # Red line for True
        else:
            ax3.plot(time_data[i:i+2], vld_data[i:i+2], 'b-')  # Blue line for False

# Setting x-axis labels to show every 10th label
#ax1.set_xticks(ticks=time_data[::50])
# Setting the range for the x-axis and y-axis
ax3.set_xlim(0, 25000)
ax3.set_ylim(0, 24)

# Adding title and labels
ax3.set_title('V_LD Data')
ax3.set_xlabel('Log Time (ms)')
ax3.set_ylabel('Supply (V)')

# Displaying the plot
plt.show()



# Plotting the Resistance Data Graph
# Creating a figure and axis object
fig3, ax3 = plt.subplots()

# Assuming the data is sorted by time_data
for i in range(len(time_data) - 1):
    if(time_data[i] < time_data[i+1]):
        if m1_en_data[i]:
            ax3.plot(time_data[i:i+2], resist_data[i:i+2], 'r-')  # Red line for True
        else:
            ax3.plot(time_data[i:i+2], resist_data[i:i+2], 'b-')  # Blue line for False

# Setting x-axis labels to show every 10th label
#ax3.set_xticks(ticks=time_data[::50])
# Setting the range for the x-axis and y-axis
ax3.set_xlim(0, 25000)
ax3.set_ylim(0, 3000)

# Adding title and labels
ax3.set_title('M2 Resistance Data')
ax3.set_xlabel('Log Time (ms)')
ax3.set_ylabel('Resistance (mΩ)')

# Displaying the plot
plt.show()

