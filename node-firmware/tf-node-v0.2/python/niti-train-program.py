import thermoflex as tf




nodelist = tf.discover([105]) #input the product id and returns a list of nodes available
node0 = nodelist[0]

# Example of how to characaterize muscles. 
muscle1 = tf.muscle(idnum = 0, resist= 300, diam= 2, length= 150)
muscle2 = tf.muscle(idnum = 1, resist= 290, diam= 2, length= 145)

node0.setMuscle(0, muscle1) #takes the mosfet number muscle params to muscle
node0.setMuscle(1, muscle2)

tf.command_t.log_mode(logmode = 2)  # Set all muscles to fast logmode


# Set output path and mode (Like Binary vs UTF-8)
output_path = "C:/Users/Mark/Documents/TF-Tests/test1.txt"

node0.logtoFile(output_path, state = True)

m_to_train = muscle1  # Just set to the muscle port that should be trained

# Muscle setup
m_to_train.setMode(PERCENT)  # Train mode does not work yet.  This is the best way for now until we meet and discuss how train mode will work.

m_to_train.setSetpoint(PERCENT, 0.1)  # Dial this value in but start low!  Keep in mind that smoking should occur sometime near the end of the 50 seconds when this value is tuned in.

# Specify training program wait values
wait1 = 50.0
wait2 = 10.0

# Test Control Script
m_to_train.enable()
tf.update(node0, wait1)  # Internally calls tf.update() until a timer has surpassed 1.0 second
node0.stop() # Disable all at end of program (or disable just m_to_train)
tf.update(node0,wait2)  # Continue collecting data until the end of program

tf.endAll() # Closes node device (serial.close())


# This is a new feature, but it would create a plot like I created in my niti-train-program.py based on the data stored to this text file.  You can parse it to create the sensor data arrays and then plot using the exact same method I used in my script
#tf.plotting(output_path)
