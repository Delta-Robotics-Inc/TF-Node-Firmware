import thermoflex as tf

nodenet_list = tf.discover([105])
nodenet = nodenet_list[0]
nodenet.refreshDevices() # call this to get all nodes in network

my_node = nodenet.getDevice([0x01, 0x02, 0x03])

# Example of how to characaterize muscles. 
muscle1 = tf.muscle(idnum = 0, resist= 300, diam= 2, length= 150)
muscle2 = tf.muscle(idnum = 1, resist= 290, diam= 2, length= 145)

my_node.setMuscle(0, muscle1) #takes the mosfet number muscle params to muscle
my_node.setMuscle(1, muscle2)

muscle1.set_mode("percent")
muscle1.set_setpoint(50)
muscle1.set_enable(True)