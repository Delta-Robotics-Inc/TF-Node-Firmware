import thermoflex as tf
import time

tf.set_debug_level('DEBUG') # set debug level to debug (highest level)

nodenet_list = tf.discover([105])
nodenet = nodenet_list[0]
nodenet.refreshDevices() # call this to get all nodes in network


print("Devices in network:")
for node in nodenet.node_list:
    print(node.node_id)


time.sleep(1)

# Test running the muscle
#node1 = nodenet.getDevice([0x01, 0x02, 0x03]) # Get specific node
node1 = nodenet.node_list[0]
#node1.setLogmode(3); # Readable dump

# Example of how to characaterize muscles. 
muscle1 = tf.Muscle(idnum = 0, resist= 300, diam= 2, length= 150)
muscle2 = tf.Muscle(idnum = 1, resist= 290, diam= 2, length= 145)

node1.setMuscle(0, muscle1) # Takes the mosfet number muscle params to muscle
node1.setMuscle(1, muscle2)

for i in range(5):
    print(node1.status("dump"))
    print(muscle1.muscleStatus())
    print(muscle2.muscleStatus())
    time.sleep(2)

# # Warmup if needed
# muscle1.setMode("percent")
# muscle1.setSetpoint(0.5)
# muscle1.setEnable(True)
# time.sleep(2)
# muscle1.setEnable(False)

# Main test
# m_to_move = muscle1

# m_to_move.setMode("percent")
# m_to_move.setSetpoint(1.0)
# m_to_move.setEnable(True)
# time.sleep(3)

# m_to_move.setSetpoint(0.75)
# time.sleep(10)

# Hopefully will hold in place
# muscle1.setMode("ohms")
# #muscle1.setSetpoint(175)
# muscle1.setSetpoint(760)

# for i in range(10):
#     muscle1.setEnable(True)
#     time.sleep(20)
#     muscle1.setEnable(False)
#     time.sleep(10)
time.sleep(0.5)
tf.endAll() # Will automatically close all ports and threads, and disable all muscles

'''
# Define 3 node controllers
node1 = nodenet.getDevice([0x01, 0x02, 0x03])
node2 = nodenet.getDevice([0x04, 0x05, 0x06])
node3 = nodenet.getDevice([0x07, 0x08, 0x09])

# Test getting statuss
print(node1.status("compact"))
print(node2.status("dump"))
print(node3.status("compact"))'''